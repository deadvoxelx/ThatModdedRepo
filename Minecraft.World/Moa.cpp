#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.goal.target.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.projectile.h"
#include "net.minecraft.world.level.pathfinder.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.stats.h"
#include "Moa.h"
#include "..\Minecraft.Client\Textures.h"
#include "SoundTypes.h"

static const float MOA_JUMP_STRENGTH = 1.5f;

Moa::Moa(Level *level) : TamableAnimal( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());

	flapM = 0;
	flapSpeedM = 0;
	flappingM = 1;
	oFlapSpeedM = oFlapM = 0.0f;

	setSize(1.2f, 2.0f);

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(1, new FloatGoal(this));
	goalSelector.addGoal(2, sitGoal, false);
	goalSelector.addGoal(3, new MeleeAttackGoal(this, 1.0, true));
	goalSelector.addGoal(4, new FollowOwnerGoal(this, 1.0, 10, 2));
	goalSelector.addGoal(5, new BreedGoal(this, 1.0));
	goalSelector.addGoal(6, new RandomStrollGoal(this, 1.0));
	goalSelector.addGoal(7, new LookAtPlayerGoal(this, typeid(Player), 8));
	goalSelector.addGoal(8, new RandomLookAroundGoal(this));

	targetSelector.addGoal(1, new OwnerHurtByTargetGoal(this));
	targetSelector.addGoal(2, new OwnerHurtTargetGoal(this));
	targetSelector.addGoal(3, new HurtByTargetGoal(this, true));
	targetSelector.addGoal(4, new NonTameRandomTargetGoal(this, typeid(Swet), 120, false));

	setTame(false);
}

int Moa::getMoaType()
{
	return (int) entityData->getByte(DATA_TYPE_ID);
}

void Moa::setMoaType(int type)
{
	entityData->set(DATA_TYPE_ID, (byte) type);
}

void Moa::registerAttributes()
{
	TamableAnimal::registerAttributes();

	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.3f);

	if (isTame())
	{
		getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(TAME_HEALTH);
	}
	else
	{
		getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(START_HEALTH);
	}
}

bool Moa::useNewAi()
{
	return true;
}

void Moa::causeFallDamage(float distance) 
{
}

void Moa::setTarget(shared_ptr<LivingEntity> target)
{
	TamableAnimal::setTarget(target);
	if ( target == nullptr )
	{
		setAngry(false);
	}
	else if(!isTame())
	{
		setAngry(true);
	}
}

bool Moa::canSpawn()
{
	int xt = Mth::floor(x);
	int yt = Mth::floor(bb->y0);
	int zt = Mth::floor(z);
	return ( level->getTile(xt, yt - 1, zt) == Tile::grass_Id || level->getTile(xt, yt - 1, zt) == Tile::aetherGrass_Id ) && level->getDaytimeRawBrightness(xt, yt, zt) > 8 && PathfinderMob::canSpawn();
}

void Moa::serverAiMobStep()
{
	entityData->set(DATA_HEALTH_ID, getHealth());
}

void Moa::defineSynchedData() 
{
	TamableAnimal::defineSynchedData();
	entityData->define(DATA_HEALTH_ID, getHealth());
	entityData->define(DATA_TYPE_ID, static_cast<byte>(0));
}

void Moa::addAdditonalSaveData(CompoundTag *tag) 
{
	TamableAnimal::addAdditonalSaveData(tag);

	tag->putBoolean(L"Angry", isAngry());
	tag->putByte(L"MoaType", (byte) getMoaType());
}

void Moa::readAdditionalSaveData(CompoundTag *tag) 
{
	TamableAnimal::readAdditionalSaveData(tag);

	setAngry(tag->getBoolean(L"Angry"));
	if (tag->contains(L"MoaType"))
	{
		int value = tag->getByte(L"MoaType");
		setMoaType(value);
	}
}

int Moa::getDeathLoot() 
{
	return Item::feather->id;
}

void Moa::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	int count = 1 + Mth::nextInt(level->random, 1, 2) + random->nextInt(1 + playerBonusLevel);
	for (int i = 0; i < count; i++)
	{
		spawnAtLocation(Item::feather_Id, 1);
	}

	if (random->nextInt(5) == 0) spawnAtLocation(Item::airFossil_Id, 1);
}

void Moa::aiStep() 
{
	TamableAnimal::aiStep();

	oFlapM = flapM;
	oFlapSpeedM = flapSpeedM;

	flapSpeedM += (onGround ? -1 : 4) * 0.3f;
	if (flapSpeedM < 0) flapSpeedM = 0;
	if (flapSpeedM > 0.75) flapSpeedM = 0.75;

	if (!onGround && flappingM < 1) flappingM = 1;
	flappingM *= 0.9;

	if (!onGround && yd < 0) 
	{
		yd *= 0.6;
	}

	flapM += flappingM * 2;
}

void Moa::tick() 
{
	TamableAnimal::tick();
}

void Moa::travel(float xa, float ya)
{
	if (rider.lock() == nullptr)
	{
		footSize = .5f;
		flyingSpeed = .02f;
		TamableAnimal::travel(xa, ya);
		return;
	}

	yRotO = yRot = rider.lock()->yRot;
	xRot = rider.lock()->xRot * 0.5f;
	setRot(yRot, xRot);
	yHeadRot = yBodyRot = yRot;

	shared_ptr<LivingEntity> livingRider = dynamic_pointer_cast<LivingEntity>(rider.lock());
	xa = livingRider->xxa * .5f;
	ya = livingRider->yya;

	// move much slower backwards
	if (ya <= 0)
	{
		ya *= .25f;
	}

	if (playerJumpPendingScale > 0 && !isEntityJumping && onGround)
	{
		yd = MOA_JUMP_STRENGTH * playerJumpPendingScale;
		isEntityJumping = true;
		hasImpulse = true;

		if (ya > 0)
		{
			float sin = Mth::sin(yRot * Mth::DEGRAD);
			float cos = Mth::cos(yRot * Mth::DEGRAD);

			xd += -0.4f * sin * playerJumpPendingScale;
			zd += 0.4f * cos * playerJumpPendingScale;
		}
		playerJumpPendingScale = 0;
	}

	footSize = 1;
	flyingSpeed = getSpeed() * .1f;
	if (!level->isClientSide)
	{
		setSpeed(static_cast<float>(getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->getValue()));
		TamableAnimal::travel(xa, ya);
	}

	if (onGround)
	{
		playerJumpPendingScale = 0;
		isEntityJumping = false;
	}
	walkAnimSpeedO = walkAnimSpeed;
	double dx = x - xo;
	double dz = z - zo;
	float wst = Mth::sqrt(dx * dx + dz * dz) * 4.0f;
	if (wst > 1.0f)
	{
		wst = 1.0f;
	}

	walkAnimSpeed += (wst - walkAnimSpeed) * 0.4f;
	walkAnimPos += walkAnimSpeed;
}

void Moa::onPlayerJump(int jumpAmount)
{
	playerJumpPendingScale = 1.0f;
}

bool Moa::hurt(DamageSource *source, float dmg) 
{
	if (isTame())
	{		
		shared_ptr<Entity> entity = source->getDirectEntity();
		if (entity != nullptr && entity->instanceof(eTYPE_PLAYER))
		{
			shared_ptr<Player> attacker = dynamic_pointer_cast<Player>(entity);
			attacker->canHarmPlayer(getOwnerUUID());
		}
	}

	if (isInvulnerable()) return false;
	shared_ptr<Entity> sourceEntity = source->getEntity();
	sitGoal->wantToSit(false);
	if (sourceEntity != nullptr && !(sourceEntity->instanceof(eTYPE_PLAYER) || sourceEntity->instanceof(eTYPE_ARROW))) 
	{
		dmg = (dmg + 1) / 2;
	}
	return TamableAnimal::hurt(source, dmg);
}

bool Moa::doHurtTarget(shared_ptr<Entity> target)
{
	int damage = isTame() ? 6 : 4;
	return target->hurt(DamageSource::mobAttack(dynamic_pointer_cast<Mob>(shared_from_this())), damage);
}

void Moa::setTame(bool value)
{
	TamableAnimal::setTame(value);

	if (value)
	{
		getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(TAME_HEALTH);
	}
	else
	{
		getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(START_HEALTH);
	}
}

void Moa::tame(const wstring &wsOwnerUUID, bool bDisplayTamingParticles, bool bSetSitting) 
{
	setTame(true);
	setPath(nullptr);
	setTarget(nullptr);
	sitGoal->wantToSit(bSetSitting);
	setHealth(TAME_HEALTH);

	setOwnerUUID(wsOwnerUUID);

	spawnTamingParticles(bDisplayTamingParticles);
}

bool Moa::mobInteract(shared_ptr<Player> player) 
{
	shared_ptr<ItemInstance> item = player->inventory->getSelected();

	if (isTame()) 
	{
		if (item != nullptr)
		{
			if (item->id == Item::blueBerry->id && entityData->getFloat(DATA_HEALTH_ID) < MAX_HEALTH)
			{
				heal(5);
				if (player->abilities.instabuild==false)
				{
					item->count--;
					if (item->count <= 0) 
					{
						player->inventory->setItem(player->inventory->selected, nullptr);
					}
				}
				return true;
			}			
		}
		if (equalsIgnoreCase(player->getUUID(), getOwnerUUID()))
		{
			if (!level->isClientSide)
			{
				if (player->isSneaking())
				{
					if (rider.lock() == player) player->ride(nullptr);
					sitGoal->wantToSit(!isSitting());
					jumping = false;
					setPath(nullptr);
					setAttackTarget(nullptr);
					setTarget(nullptr);
				}
				else
				{
					shared_ptr<Entity> currentRider = rider.lock();
					if (currentRider == nullptr || currentRider == player)
					{
						sitGoal->wantToSit(false);
						player->ride(currentRider == player ? nullptr : shared_from_this());
					}
				}
			}
		}
	}
	else
	{
		if (item != nullptr && item->id == Item::blueBerry->id && !isAngry()) 
		{
			if (player->abilities.instabuild==false)
			{
				item->count--;
				if (item->count <= 0)
				{
					player->inventory->setItem(player->inventory->selected, nullptr);
				}
			}

			if (!level->isClientSide) 
			{
				if (random->nextInt(3) == 0) 
				{
					tame(player->getUUID(),true,true);

					level->broadcastEntityEvent(shared_from_this(), EntityEvent::TAMING_SUCCEEDED);
				} 
				else 
				{
					spawnTamingParticles(false);
					level->broadcastEntityEvent(shared_from_this(), EntityEvent::TAMING_FAILED);
				}
			}

			return true;
		}

		if((item != nullptr) && isFood(item))
		{
			return false;
		}
	}
	player->swing();
	return TamableAnimal::mobInteract(player);
}

void Moa::handleEntityEvent(byte id) 
{
		TamableAnimal::handleEntityEvent(id);
}

bool Moa::isFood(shared_ptr<ItemInstance> item)
{
	if (item == nullptr) return false;
	if (dynamic_cast<FoodItem *>(Item::items[item->id]) == nullptr) return false;
	return static_cast<FoodItem *>(Item::items[item->id])->isMeat();
}

bool Moa::isAngry() 
{
	return (entityData->getByte(DATA_FLAGS_ID) & 0x02) != 0;
}

void Moa::setAngry(bool value) 
{
	byte current = entityData->getByte(DATA_FLAGS_ID);
	if (value) 
	{
		entityData->set(DATA_FLAGS_ID, static_cast<byte>(current | 0x02));
	} 
	else
	{
		entityData->set(DATA_FLAGS_ID, static_cast<byte>(current & ~0x02));
	}
}

int Moa::GetSynchedHealth()	
{
	return (int) getEntityData()->getFloat(DATA_HEALTH_ID);
}	

shared_ptr<AgableMob> Moa::getBreedOffspring(shared_ptr<AgableMob> target)
{
	if( level->canCreateMore( GetType(), Level::eSpawnType_Breed) )
	{
		shared_ptr<Moa> pBabyMoa = std::make_shared<Moa>(level);

		if(!getOwnerUUID().empty())
		{
			pBabyMoa->tame(getOwnerUUID(),false,false);
		}
		return pBabyMoa;
	}
	else
	{
		return nullptr;
	}
}

bool Moa::canMate(shared_ptr<Animal> animal)
{
	if (animal == shared_from_this()) return false;
	if (!isTame()) return false;

	if (!animal->instanceof(eTYPE_MOA)) return false;
	shared_ptr<Moa> partner = dynamic_pointer_cast<Moa>(animal);

	if (partner == nullptr) return false;
	if (!partner->isTame()) return false;
	if (partner->isSitting()) return false;

	return isInLove() && partner->isInLove();
}

bool Moa::removeWhenFarAway()
{
	return !isTame() && tickCount > SharedConstants::TICKS_PER_SECOND * 60 * 2;
}

bool Moa::wantsToAttack(shared_ptr<LivingEntity> target, shared_ptr<LivingEntity> owner)
{
	if (target->GetType() == eTYPE_CREEPER || target->GetType() == eTYPE_GHAST)
	{
		return false;
	}
	if (target->GetType() == eTYPE_MOA)
	{
		shared_ptr<Moa> moaTarget = dynamic_pointer_cast<Moa>(target);
		if (moaTarget->isTame() && moaTarget->getOwner() == owner)
		{
			return false;
		}
	}
	if ( target->instanceof(eTYPE_PLAYER) && owner->instanceof(eTYPE_PLAYER) && !dynamic_pointer_cast<Player>(owner)->canHarmPlayer(dynamic_pointer_cast<Player>(target) ))
	{
		return false;
	}
	if ((target->GetType() == eTYPE_HORSE) && dynamic_pointer_cast<EntityHorse>(target)->isTamed())
	{
		return false;
	}
	return true;
}

MobGroupData *Moa::finalizeMobSpawn(MobGroupData *groupData, int extraData)
{
	groupData = TamableAnimal::finalizeMobSpawn(groupData);

	if (getRandom()->nextInt(10) == 0)
	{
		setMoaType(TYPE_BLACK);
	}
	else if (getRandom()->nextInt(10) <= 5 && getRandom()->nextInt(10) > 0)
	{
		setMoaType(TYPE_WHITE);
	}
	else
	{
		setMoaType(TYPE_DEFAULT);
	}

	return groupData;
}
