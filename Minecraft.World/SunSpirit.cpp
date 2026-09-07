#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.global.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.h"
#include "SunSpirit.h"
#include "EntityCrystal.h"
#include "FireMinion.h"
#include "Mth.h"
#include "..\Minecraft.Client\Textures.h"
#include "..\Minecraft.Client\ServerPlayer.h"
#include "MobCategory.h"

SunSpirit::SunSpirit(Level *level) : Monster( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());

	setSize(2.5f, 3.2f);

	noPhysics = true;
	fireImmune = true;
	xpReward = Enemy::XP_REWARD_SUN_SPIRIT;		// Gave it its own xpReward level so it feels more rewarding

	originPointX = 0;
	originPointY = 0;
	originPointZ = 0;
	direction = -1;
	motionTimer = 0;
	flameCount = 0;
	ballCount = 0;
	chatCount = 0;
	rotary = random->nextFloat() * 360.0;
	velocity = 0.5;

	setBossName(app.GetString(IDS_SUN_SPIRIT));

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(0, new FloatGoal(this));
	goalSelector.addGoal(7, new LookAtPlayerGoal(this, typeid(Player), 8));
	goalSelector.addGoal(8, new RandomLookAroundGoal(this));
}

bool SunSpirit::useNewAi()
{
	return true;
}

void SunSpirit::defineSynchedData()
{
	Monster::defineSynchedData();

	entityData->define(DATA_CHAT_LINE, (byte)0);
	entityData->define(DATA_FREEZING, (byte)0);
	entityData->define(DATA_BOSS_NAME, L"");
}

void SunSpirit::registerAttributes()
{
	Monster::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(500);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.35f);
	getAttribute(SharedMonsterAttributes::KNOCKBACK_RESISTANCE)->setBaseValue(1.0);
}

void SunSpirit::aiStep()
{
	if (direction < 0)
	{
		direction = (int)xRot;
		originPointX = Mth::floor(x);
		originPointY = Mth::floor(y);
		originPointZ = Mth::floor(z);
		yRot = yRotO = (direction == 3) ? 0.0f : (direction == 0) ? 90.0f : (direction == 2) ? 180.0f : 270.0f;
		setRot(yRot, xRot);
	}

	Monster::aiStep();

	setFreezing(hurtTime > 0);

	if (getHealth() > 0.0f)
	{
		for (int i = 0; i < 2; i++)
		{
			level->addParticle(eParticleType_flame, x + (random->nextDouble() - 0.5) * bbWidth / 2, y - random->nextDouble() * 1.2, z + (random->nextDouble() - 0.5) * bbWidth / 2, 0, 0, 0);
		}

		if (!level->isClientSide)
		{
			burnEntities();
			evapWater();
		}
	}

	if (chatCount > 0) chatCount--;
}

void SunSpirit::newServerAiStep()
{
	Monster::newServerAiStep();

	y = originPointY;
	setPos(x, y, z);

	shared_ptr<LivingEntity> target = getTarget();
	if (target != nullptr && target->isAlive())
	{
		yd = 0.0;
		yBodyRot = yRot;

		bool changedCourse = false;
		if (xd >= 0.0 && x > originPointX + 8.5)
		{
			rotary = 360.0 - rotary;
			changedCourse = true;
		}
		else if (xd <= 0.0 && x < originPointX - 10.0)
		{
			rotary = 360.0 - rotary;
			changedCourse = true;
		}
		if (zd >= 0.0 && z > originPointZ + 10.0)
		{
			rotary = 180.0 - rotary;
			changedCourse = true;
		}
		else if (zd <= 0.0 && z < originPointZ - 9.0)
		{
			rotary = 180.0 - rotary;
			changedCourse = true;
		}

		if (rotary > 360.0) rotary -= 360.0;
		else if (rotary < 0.0) rotary += 360.0;

		lookAt(target, 20.0f, 20.0f);

		double angle = rotary / 57.29577951308232;
		double speed = velocity * 0.5;
		xd = Mth::sin(angle) * speed;
		zd = Mth::cos(angle) * speed;

		motionTimer++;
		if (motionTimer >= 20 || changedCourse)
		{
			if (random->nextInt(3) == 0)
			{
				rotary += (random->nextFloat() - random->nextFloat()) * 60.0;
			}
			motionTimer = 0;
		}

		flameCount++;
		if (flameCount == 40)
		{
			summonFire();
		}
		else if (flameCount >= 30.0f + getHealth() / 4.0f)
		{
			makeFireBall(1);
			flameCount = 0;
		}
	}
}

void SunSpirit::travel(float xa, float ya)
{
	move(xd, yd, zd);

	xd *= 0.91f;
	yd *= 0.91f;
	zd *= 0.91f;

	walkAnimSpeedO = walkAnimSpeed;
	double xxd = x - xo;
	double zzd = z - zo;
	float wst = Mth::sqrt(xxd * xxd + zzd * zzd) * 4;
	if (wst > 1) wst = 1;
	walkAnimSpeed += (wst - walkAnimSpeed) * 0.4f;
	walkAnimPos += walkAnimSpeed;
}

void SunSpirit::tick()
{
	Monster::tick();

	velocity = 0.5 - (getMaxHealth() + getHealth()) / 70.0 * 0.1;

	// Reset the fight if the player dies
	if (!level->isClientSide)
	{
		shared_ptr<LivingEntity> target = getTarget();
		if (target != nullptr && target->instanceof(eTYPE_PLAYER))
		{
			shared_ptr<Player> dungeonTarget = dynamic_pointer_cast<Player>(target);
			if (dungeonTarget != nullptr && (!dungeonTarget->isAlive() || dungeonTarget->removed))
			{
				resetAfterPlayerDeath(dungeonTarget);
			}
		}
	}
}

int SunSpirit::getDeathLoot()
{
	return Item::keyGold_Id;
}

void SunSpirit::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{	// Drops the Gold Key, will drop the Sun Altar too once i add it
	spawnAtLocation(Item::keyGold_Id, 1);
}

bool SunSpirit::removeWhenFarAway()
{
	return false;
}

bool SunSpirit::isPushable()
{
	return false;
}

void SunSpirit::doPush(shared_ptr<Entity> e)
{
}

void SunSpirit::pushEntities()
{
}

void SunSpirit::causeFallDamage(float distance)
{
}

bool SunSpirit::canBeAffected(MobEffectInstance *newEffect)
{
	return false;
}

bool SunSpirit::hurt(DamageSource *source, float dmg)
{
	shared_ptr<Entity> directEntity = source->getDirectEntity();
	if (directEntity != nullptr && directEntity->GetType() == eTYPE_CRYSTAL)
	{
		shared_ptr<EntityCrystal> crystal = dynamic_pointer_cast<EntityCrystal>(directEntity);
		if (crystal != nullptr && crystal->getCrystalType() == eCrystalType_ICE)
		{	// The ice crystal damage is defined in EntityCrystal.cpp
			velocity = 0.5 - (getMaxHealth() + getHealth()) / 70.0 * 0.1;

			bool flag = Monster::hurt(source, dmg);
			if (flag)
			{
				shared_ptr<FireMinion> minion = make_shared<FireMinion>(level);
				minion->moveTo(x, y, z, yRot, 0.0f);
				minion->setTarget(getTarget());
				if (!level->isClientSide)
				{
					level->addEntity(minion);
				}
			}
			return flag;
		}
		return false;
	}
	return false;
}

void SunSpirit::die(DamageSource *source)
{
	setFreezing(true);
	setChatLine(10);	// The death message

	if (!level->isClientSide)
	{
		shared_ptr<Player> p = nullptr;
		shared_ptr<LivingEntity> target = getTarget();
		if (target != nullptr && target->instanceof(eTYPE_PLAYER))
		{
			p = dynamic_pointer_cast<Player>(target);
		}
		else
		{
			p = level->getNearestPlayer(shared_from_this(), 64.0);
		}
		chatCount = 0;
		chatLine(p, IDS_SUN_SPIRIT_DIALOG_DEATH);
		chatCount = 100;
	}

	Monster::die(source);
}

bool SunSpirit::interact(shared_ptr<Player> player)
{
	if (level->isClientSide) return false;

	if (chatWithMe(player))
	{
		rotary = 57.29577951308232 * atan2(x - player->x, z - player->z);
		setTarget(dynamic_pointer_cast<LivingEntity>(player));
		return true;
	}
	return false;
}

bool SunSpirit::chatWithMe(shared_ptr<Player> player)
{
	if (chatCount > 0) return false;

	int line = getChatLine();
	if (line == 0)
	{
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_0);
		setChatLine(1);
	}
	else if (line == 1)
	{
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_1);
		setChatLine(2);
	}
	else if (line == 2)
	{
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_2);
		setChatLine(3);
	}
	else if (line == 3)
	{
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_3);
		setChatLine(4);
	}
	else if (line == 4)
	{
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_4);
		setChatLine(5);
	}
	else if (line == 5)
	{
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_5_1);
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_5_2);
		setChatLine(6);
	}
	else if (line == 6)
	{
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_6_1);
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_6_2);
		setChatLine(7);
	}
	else if (line == 7)
	{
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_7_1);
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_7_2);
		setChatLine(8);
	}
	else if (line == 8)
	{
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_8);
		setChatLine(9);
	}
	else if (line == 9)
	{
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_9);
		setChatLine(10);
		return true;
	}
	else if (line == 10 && getTarget() == nullptr)
	{
		chatLine(player, IDS_SUN_SPIRIT_DIALOG_10);
		setChatLine(9);
	}

	return false;
}

void SunSpirit::chatLine(shared_ptr<Player> player, int messageId)
{
	if (player != nullptr && player->instanceof(eTYPE_SERVERPLAYER))
	{
		dynamic_pointer_cast<ServerPlayer>(player)->sendMessage(app.GetString(messageId));
	}
}

void SunSpirit::burnEntities()
{
	vector<shared_ptr<Entity> > objects = *level->getEntities(shared_from_this(), bb->grow(0.0, 4.0, 0.0));
	for (auto& entity : objects)
	{
		if (entity->instanceof(eTYPE_LIVINGENTITY) && !entity->isFireImmune())
		{
			DamageSource *damageSource = DamageSource::mobAttack(dynamic_pointer_cast<LivingEntity>(shared_from_this()));
			entity->hurt(damageSource, 10);
			delete damageSource;
			entity->setOnFire(1);
		}
	}
}

void SunSpirit::evapWater()
{
	int var1 = Mth::floor(bb->x0 + (bb->x1 - bb->x0) / 2.0);
	int var2 = Mth::floor(bb->z0 + (bb->z1 - bb->z0) / 2.0);
	int radius = 10;

	for (int x = var1 - radius; x <= var1 + radius; x++)
	{
		for (int z = var2 - radius; z <= var2 + radius; z++)
		{
			for (int i = 0; i < 8; i++)
			{
				int y = originPointY - 2 + i;
				int tile = level->getTile(x, y, z);
				if (tile == Tile::water_Id || tile == Tile::calmWater_Id)
				{
					level->setTileAndData(x, y, z, 0, 0, Tile::UPDATE_CLIENTS);
					level->playSound(x, y, z, eSoundType_RANDOM_FIZZ, 2.0f, random->nextFloat() - random->nextFloat() * 0.2f + 1.2f);
					for (int j = 0; j < 8; j++)
					{
						level->addParticle(eParticleType_largesmoke, x + random->nextDouble(), y + 0.75, z + random->nextDouble(), 0.0, 0.0, 0.0);
					}
				}
			}
		}
	}
}

void SunSpirit::makeFireBall(int count)
{
	playSound(eSoundType_MOB_GHAST_FIREBALL, random->nextFloat() - random->nextFloat() * 0.2f + 1.2f, 1.0f);

	bool shootIceyBall = false;
	ballCount++;
	if (ballCount >= 1 + random->nextInt(2))	// Made ice crystals more common
	{
		shootIceyBall = true;
		ballCount = 0;
	}

	for (int i = 0; i < count; i++)
	{
		shared_ptr<EntityCrystal> crystal = make_shared<EntityCrystal>(level, x - xd / 2.0, y, z - zd / 2.0, shootIceyBall ? eCrystalType_ICE : eCrystalType_FIRE);
		crystal->setShootingEntity(shared_from_this());
		if (!level->isClientSide)
		{
			level->addEntity(crystal);
		}
	}
}

void SunSpirit::summonFire()
{
	int x = Mth::floor(this->x);
	int z = Mth::floor(this->z);
	int y = originPointY - 2;

	if (level->isEmptyTile(x, y, z))
	{
		level->setTileAndData(x, y, z, Tile::fire_Id, 0, Tile::UPDATE_CLIENTS);
	}
}

void SunSpirit::resetAfterPlayerDeath(shared_ptr<Player> dungeonTarget)
{	// Put it back in the center after the player dies
	setPos(originPointX + 0.5, originPointY, originPointZ + 0.5);
	xd = 0.0;
	yd = 0.0;
	zd = 0.0;

	chatCount = 0;
	chatLine(dungeonTarget, IDS_SUN_SPIRIT_DIALOG_KILLED_PLAYER);
	chatCount = 100;

	setFreezing(false);
	setTarget(nullptr);
	setHealth(getMaxHealth());
}

void SunSpirit::addAdditonalSaveData(CompoundTag *tag)
{
	Monster::addAdditonalSaveData(tag);

	tag->putInt(L"originPointX", originPointX);
	tag->putInt(L"originPointY", originPointY);
	tag->putInt(L"originPointZ", originPointZ);
	tag->putInt(L"dungeonDirection", direction);
	tag->putInt(L"chatLog", getChatLine());
	tag->putString(L"bossName", getBossName());
	tag->putBoolean(L"isFreezing", isFreezing());
}

void SunSpirit::readAdditionalSaveData(CompoundTag *tag)
{
	Monster::readAdditionalSaveData(tag);

	if (tag->contains(L"originPointX")) originPointX = tag->getInt(L"originPointX");
	if (tag->contains(L"originPointY")) originPointY = tag->getInt(L"originPointY");
	if (tag->contains(L"originPointZ")) originPointZ = tag->getInt(L"originPointZ");
	if (tag->contains(L"dungeonDirection")) direction = tag->getInt(L"dungeonDirection");
	if (tag->contains(L"chatLog")) setChatLine(tag->getInt(L"chatLog"));
	if (tag->contains(L"bossName")) setBossName(tag->getString(L"bossName"));
	if (tag->contains(L"isFreezing")) setFreezing(tag->getBoolean(L"isFreezing"));
}

int SunSpirit::getChatLine()
{
	return (int)entityData->getByte(DATA_CHAT_LINE);
}

void SunSpirit::setChatLine(int lineNumber)
{
	chatCount = 60;		// Default delay is 100, i set it to 60 for convenience
	entityData->set(DATA_CHAT_LINE, (byte)lineNumber);
}

bool SunSpirit::isFreezing()
{
	return entityData->getByte(DATA_FREEZING) == 1;
}

void SunSpirit::setFreezing(bool isFreezing)
{
	entityData->set(DATA_FREEZING, isFreezing ? (byte)1 : (byte)0);
}

void SunSpirit::setBossName(const wstring &name)
{
	entityData->set(DATA_BOSS_NAME, name);
}

wstring SunSpirit::getBossName()
{
	return entityData->getString(DATA_BOSS_NAME);
}

void SunSpirit::setOriginPosition(int x, int y, int z)
{
	originPointX = x;
	originPointY = y;
	originPointZ = z;
}
