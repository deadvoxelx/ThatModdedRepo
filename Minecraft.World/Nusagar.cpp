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
#include "Nusagar.h"
#include "Nuskull.h"
#include "NuclearNuskull.h"
#include "NusaDemon.h"
#include "NusaSpike.h"
#include "..\Minecraft.Client\Textures.h"
#include "MobCategory.h"

// AAAAAAUUUUUUUUGGGGGGHHHHHHHHH!!!!!!!!!!!

int Nusagar::s_aliveCount = 0;

Nusagar::Nusagar(Level *level) : FlyingMonster( level )
{
	s_aliveCount++;

	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());

	setSize(0.7f, 2.95f);

	fireImmune = true;
	xpReward = Enemy::XP_REWARD_NUSAGAR;	// Drops 12000 xp since its the true final boss

	originPointX = 0;
	originPointY = 0;
	originPointZ = 0;
	bOriginSet = false;
	spawnLevel1 = false;
	spawnLevel2 = false;
	spawnLevel3 = false;
	targetPosition = nullptr;
	checkTicks = 20;
	checkDist = 0.0;
	spikeTimer = 100;
	spinTicks = 0;

	setBossName(app.GetString(IDS_NUSAGAR));

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(0, new FloatGoal(this));
	goalSelector.addGoal(7, new LookAtPlayerGoal(this, typeid(Player), 8));
	goalSelector.addGoal(8, new RandomLookAroundGoal(this));
}

Nusagar::~Nusagar()
{
	s_aliveCount--;
}

void Nusagar::defineSynchedData()
{
	FlyingMonster::defineSynchedData();

	entityData->define(DATA_BOSS_NAME, L"");
	entityData->define(DATA_SPIN_TICKS, (byte)0);
}

bool Nusagar::useNewAi()
{
	return true;
}

void Nusagar::registerAttributes()
{
	FlyingMonster::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(800);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.35f);
	getAttribute(SharedMonsterAttributes::KNOCKBACK_RESISTANCE)->setBaseValue(1.0f);
}

void Nusagar::setBossName(const wstring &name)
{
	entityData->set(DATA_BOSS_NAME, name);
}

wstring Nusagar::getBossName()
{
	return entityData->getString(DATA_BOSS_NAME);
}

void Nusagar::setOriginPosition(int x, int y, int z)
{
	originPointX = x;
	originPointY = y;
	originPointZ = z;
	bOriginSet = true;
}

inline int signum(double x) { return (x > 0) - (x < 0); }

void Nusagar::newServerAiStep()
{
	FlyingMonster::newServerAiStep();

	if (--spikeTimer <= 0)
	{
		spikeTimer = 80;
		spikeLine();
	}

	if (random->nextInt(256) == 0)
	{
		spikeLines();
	}

	if (spinTicks > 0 && --spinTicks == 0)
	{
		entityData->set(DATA_SPIN_TICKS, (byte)0);
	}

	{
		double originDist = (x - originPointX) * (x - originPointX) + (y - originPointY) * (y - originPointY) + (z - originPointZ) * (z - originPointZ);

		if (targetPosition != nullptr && !level->isEmptyTile(targetPosition->x, targetPosition->y, targetPosition->z))
		{	// Trash current target pos and create a new one in case the selected target pos becomes invalid
			pickTarget();
		}
		
		if (originDist > 16.0 * 16.0)
		{	// Head back to valid range
			pickTarget();
		}
		else if (targetPosition == nullptr || random->nextInt(512) == 0 || targetPosition->distSqr(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)) <= 4)
		{	// Same Evupul shit
			pickTarget();
		}
		else if (--checkTicks <= 0)
		{	// This stops it from getting stuck at walls
			checkTicks = 20;

			double curDist = sqrt((targetPosition->x + .3 - x) * (targetPosition->x + .3 - x) + (targetPosition->y + .1 - y) * (targetPosition->y + .1 - y) + (targetPosition->z + .3 - z) * (targetPosition->z + .3 - z));
			if (curDist > 3.0 && curDist > checkDist - 1.5)
			{	// If blocked, reset path
				pickTarget();
			}
			else
			{	// Otherwise keepn rolling
				checkDist = curDist;
			}
		}
		
		double dx = (targetPosition->x + .3) - x;
		double dy = (targetPosition->y + .1) - y;
		double dz = (targetPosition->z + .3) - z;

		xd = xd + (signum(dx) * .2f - xd) * .1f;
		yd = yd + (signum(dy) * .22f - yd) * .1f;
		zd = zd + (signum(dz) * .2f - zd) * .1f;

		float yRotD = static_cast<float>(atan2(zd, xd) * 180 / PI) - 90;
		float rotDiff = Mth::wrapDegrees(yRotD - yRot);
		yya = .5f;
		yRot += rotDiff;
		yHeadRot = yBodyRot = yRot;
		setRot(yRot, xRot);
	}
}

bool Nusagar::pickTarget()
{
	if (targetPosition != nullptr)
	{
		delete targetPosition;
		targetPosition = nullptr;
	}

	int sx = Mth::floor(x);
	int sy = Mth::floor(y);
	int sz = Mth::floor(z);

	shared_ptr<Player> nearestPlayer = level->getNearestPlayer(shared_from_this(), 48.0);
	bool biasToPlayer = (nearestPlayer != nullptr && random->nextInt(2) == 0);

	// Find a valid path
	for (int attempt = 0; attempt < 12; attempt++)
	{
		int tx;
		int ty;
		int tz;

		if (attempt == 0 && biasToPlayer)
		{	// Hover a few blocks out from the player
			double angle = random->nextDouble() * 2.0 * PI;
			double dist = 2.0 + random->nextDouble() * 2.0;
			tx = Mth::floor(nearestPlayer->x + cos(angle) * dist);
			ty = Mth::floor(nearestPlayer->y + random->nextDouble() * 2.0);
			tz = Mth::floor(nearestPlayer->z + sin(angle) * dist);
		}
		else
		{
			tx = sx + random->nextInt(24) - random->nextInt(24);
			ty = sy + random->nextInt(8) - random->nextInt(8);
			tz = sz + random->nextInt(24) - random->nextInt(24);
		}

		clampToOrigin(tx, ty, tz);

		if (pathClear(tx, ty, tz))
		{
			targetPosition = new Pos(tx, ty, tz);
			break;
		}
	}

	// Create and set that new path
	if (targetPosition == nullptr)
	{
		const int probe[6][3] =
		{
			{ 8, 0, 0 }, { -8, 0, 0 }, { 0, 0, 8 }, { 0, 0, -8 }, { 0, 6, 0 }, { 0, -6, 0 }
		};

		for (int i = 0; i < 6; i++)
		{
			int tx = sx + probe[i][0];
			int ty = sy + probe[i][1];
			int tz = sz + probe[i][2];

			clampToOrigin(tx, ty, tz);

			if (pathClear(tx, ty, tz))
			{
				targetPosition = new Pos(tx, ty, tz);
				break;
			}
		}
	}

	// Hover in place if for some reason theres no targetPosition set
	if (targetPosition == nullptr) targetPosition = new Pos(sx, sy, sz);

	checkTicks = 20;
	checkDist = sqrt((targetPosition->x + .3 - x) * (targetPosition->x + .3 - x) + (targetPosition->y + .1 - y) * (targetPosition->y + .1 - y) + (targetPosition->z + .3 - z) * (targetPosition->z + .3 - z));
	return true;
}

void Nusagar::clampToOrigin(int &tx, int &ty, int &tz)
{	// Dont traverse too far from the spawnpoint
	double dx = tx - originPointX;
	double dy = ty - originPointY;
	double dz = tz - originPointZ;
	double dist = sqrt(dx * dx + dy * dy + dz * dz);
	if (dist > 16)
	{
		double scale = 16.0 / dist;
		tx = originPointX + static_cast<int>(dx * scale);
		ty = originPointY + static_cast<int>(dy * scale);
		tz = originPointZ + static_cast<int>(dz * scale);
	}
}

bool Nusagar::pathClear(int tx, int ty, int tz)
{	// Check for a valid path
	double dx = (tx + 0.5) - x;
	double dy = (ty + 0.5) - y;
	double dz = (tz + 0.5) - z;
	double dist = sqrt(dx * dx + dy * dy + dz * dz);
	if (dist < 1.0) return true;

	int steps = Mth::floor(dist) + 1;
	for (int i = 1; i <= steps; i++)
	{
		double t = (double)i / (double)steps;
		int px = Mth::floor(x + dx * t);
		int py = Mth::floor(y + dy * t);
		int pz = Mth::floor(z + dz * t);
		if (!level->isEmptyTile(px, py, pz) || !level->isEmptyTile(px, py + 1, pz) || !level->isEmptyTile(px, py + 2, pz))
		{
			return false;
		}
	}
	return true;
}

void Nusagar::aiStep() 
{
	if (!bOriginSet)
	{
		originPointX = Mth::floor(x);
		originPointY = Mth::floor(y);
		originPointZ = Mth::floor(z);
		bOriginSet = true;
	}

	FlyingMonster::aiStep();
}

int Nusagar::getDeathLoot() 
{
	return Item::nusaBlade_Id;
}

void Nusagar::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	spawnAtLocation(Item::nusaBlade_Id, 1);
}

bool Nusagar::removeWhenFarAway()
{
	return false;
}

bool Nusagar::hurt(DamageSource *source, float dmg)
{
	if (source == DamageSource::cactus) return false;
	if (source == DamageSource::dragonbreath) return false;
	if (source == DamageSource::drown) return false;
	if (source == DamageSource::fall) return false;
	if (source == DamageSource::inWall) return false;
	if (source == DamageSource::magic) return false;
	if (source == DamageSource::inFire) return false;
	if (source == DamageSource::onFire) return false;
	if (source == DamageSource::lava) return false;
	if (source->isExplosion()) return false;

	shared_ptr<Entity> directEntity = source->getDirectEntity();
	if (directEntity != nullptr && directEntity->GetType() == eTYPE_PROJECTILE)
	{
		return false;
	}

	if (!level->isClientSide)
	{
		if (random->nextInt(7) == 0)
		{	// Random explosion on hurt, the Nusa Demon does this too
			float r = 5.0f;
			level->explode(shared_from_this(), x, y + 1.5f, z, r, true);
		}

		if (random->nextInt(5) == 0)
		{	// Random teleport on hurt
			double yaw = random->nextDouble() * 2.0 * PI;
			double pitch = (random->nextDouble() - 0.5) * PI;
			double dx = 8.0 * cos(pitch) * cos(yaw);
			double dz = 8.0 * cos(pitch) * sin(yaw);
			moveTo(x + dx, y, z + dz, yRot, xRot);
		}
	}

	bool flag = FlyingMonster::hurt(source, dmg * 0.5f);	// Damage recieved is divided by 1/2 to simulate a high defense; it has 800 hp and im not changing that
	if (flag && !level->isClientSide && getHealth() > 0.0f)
	{
		if (!spawnLevel1 && getHealth() < 600.0f)
		{
			spawnLevel1 = true;
			summonLevel1();
		}

		if (!spawnLevel2 && getHealth() < 400.0f)
		{
			spawnLevel2 = true;
			summonLevel2();
		}

		if (!spawnLevel3 && getHealth() < 200.0f)
		{
			spawnLevel3 = true;
			summonLevel3();
		}
	}
	return flag;
}

void Nusagar::summonLevel1()
{	// Spawns 4 Nuskulls in a circle around itself
	for (int i = 0; i < 4; i++)
	{
		double angle = i * (2.0 * PI / 4.0) + (random->nextDouble() - 0.5) * 0.5;
		double dist = 3.0 + random->nextDouble() * 3.0;

		shared_ptr<Nuskull> nuskull = make_shared<Nuskull>(level);
		nuskull->moveTo(x + cos(angle) * dist, y + 1.0, z + sin(angle) * dist, yRot, 0.0f);
		nuskull->setNuskullType(random->nextInt(2) == 0 ? Nuskull::TYPE_DEFAULT : Nuskull::TYPE_VARIANT);
		level->addEntity(nuskull);
	}
}

void Nusagar::summonLevel2()
{	// Spawns 1 Nuclear Nuskull on itself, and 1 Nusa Demon on the player that last hurt it
	shared_ptr<NuclearNuskull> nuclear = make_shared<NuclearNuskull>(level);
	nuclear->moveTo(x, y + 1.0, z, yRot, 0.0f);
	level->addEntity(nuclear);

	shared_ptr<Player> targetPlayer = nullptr;
	if (lastHurtByPlayer != nullptr && lastHurtByPlayer->isAlive() && !lastHurtByPlayer->removed && lastHurtByPlayer->level == level)
	{
		targetPlayer = lastHurtByPlayer;
	}
	else
	{
		targetPlayer = level->getNearestPlayer(shared_from_this(), 64.0);
	}

	shared_ptr<NusaDemon> demon = make_shared<NusaDemon>(level);
	demon->moveTo(targetPlayer->x, targetPlayer->y, targetPlayer->z, 0.0f, 0.0f);
	level->addEntity(demon);
}

void Nusagar::summonLevel3()
{	// Spawns 2 Nuclear Nuskulls around itself, and 2 Nusa Demons on the player that last hurt it
	for (int i = 0; i < 2; i++)
	{
		double angle = i * (2.0 * PI / 4.0) + (random->nextDouble() - 0.5) * 0.5;
		double dist = 3.0 + random->nextDouble() * 3.0;

		shared_ptr<NuclearNuskull> nuke = make_shared<NuclearNuskull>(level);
		nuke->moveTo(x + cos(angle) * dist, y + 1.0, z + sin(angle) * dist, yRot, 0.0f);
		level->addEntity(nuke);
	}

	shared_ptr<Player> targetPlayer = nullptr;
	if (lastHurtByPlayer != nullptr && lastHurtByPlayer->isAlive() && !lastHurtByPlayer->removed && lastHurtByPlayer->level == level)
	{
		targetPlayer = lastHurtByPlayer;
	}
	else
	{
		targetPlayer = level->getNearestPlayer(shared_from_this(), 64.0);
	}

	for (int i = 0; i < 2; i++)
	{
		shared_ptr<NusaDemon> demon = make_shared<NusaDemon>(level);
		demon->moveTo(targetPlayer->x, targetPlayer->y, targetPlayer->z, 0.0f, 0.0f);
		level->addEntity(demon);
	}
}

void Nusagar::spikeLine()
{	// Spawns a line of Nusa Spikes in the direction of the nearest player
	shared_ptr<Player> player = level->getNearestPlayer(shared_from_this(), 64.0);
	if (player == nullptr) return;

	double dx = player->x - x;
	double dz = player->z - z;
	double horizDist = sqrt(dx * dx + dz * dz);
	if (horizDist < 0.001) return;

	dx /= horizDist;
	dz /= horizDist;

	double lineReach = (horizDist + 2.0 < 30.0) ? horizDist + 2.0 : 30.0;
	spikeLineInDirection(player, dx, dz, lineReach, 1.0);

	spinTicks = SPIN_DURATION_TICKS;
	entityData->set(DATA_SPIN_TICKS, (byte)SPIN_DURATION_TICKS);
}

void Nusagar::spikeLines()
{	// 8 Spike lines in 8 directions (4 straight out, 4 diagonal)
	shared_ptr<Player> player = level->getNearestPlayer(shared_from_this(), 64.0);
	if (player == nullptr) return;

	const double sqrt2over2 = 0.7071067811865476;
	const double dirX[8] = { 1.0, -1.0, 0.0, 0.0,  sqrt2over2, -sqrt2over2,  sqrt2over2, -sqrt2over2 };
	const double dirZ[8] = { 0.0,  0.0, 1.0, -1.0, sqrt2over2, -sqrt2over2, -sqrt2over2,  sqrt2over2 };

	for (int d = 0; d < 8; d++)
	{
		spikeLineInDirection(player, dirX[d], dirZ[d], 30.0, 2.0);
	}

	spinTicks = SPIN_DURATION_TICKS;
	entityData->set(DATA_SPIN_TICKS, (byte)SPIN_DURATION_TICKS);
}

void Nusagar::spikeLineInDirection(shared_ptr<Player> player, double dirX, double dirZ, double lineReach, double spacing)
{	// Spawns a line of Nusa Spikes
	int count = Mth::floor(lineReach / spacing);
	if (count < 3) count = 3;
	if (count > 30) count = 30;

	float yaw = static_cast<float>(atan2(dirZ, dirX) * 180.0 / PI) - 90.0f;
	int playerPos = Mth::floor(player->y);

	for (int i = 0; i < count; i++)
	{
		double dist = 1.5 + i * spacing;
		int px = Mth::floor(x + dirX * dist);
		int pz = Mth::floor(z + dirZ * dist);

		int colTop = level->getTopSolidBlock(px, pz);

		int groundY;
		if (colTop >= playerPos)
		{
			groundY = -1;
			for (int yy = playerPos; yy > 0; yy--)
			{
				int t = level->getTile(px, yy, pz);
				if (t != 0 && Tile::tiles[t] != nullptr && Tile::tiles[t]->material->blocksMotion())
				{
					groundY = yy + 1;
					break;
				}
			}

			if (groundY < colTop - 6) groundY = colTop;
		}
		else
		{
			groundY = colTop;
		}

		if (groundY <= 1) continue;
		if (!level->isEmptyTile(px, groundY, pz)) continue;

		shared_ptr<NusaSpike> spike = make_shared<NusaSpike>(level);
		spike->moveTo(px + 0.5, groundY, pz + 0.5, yaw, 0.0f);
		level->addEntity(spike);
	}
}

bool Nusagar::isSpinning()
{
	return entityData->getByte(DATA_SPIN_TICKS) > 0;
}

void Nusagar::playerTouch(shared_ptr<Player> player)
{	// Damages on contact; the Java version is like this too
	if (isDealsDamage())
	{
		if (canSee(player) && distanceToSqr(player) < (2.0) * (2.0))
		{
			DamageSource *damageSource = DamageSource::mobAttack( dynamic_pointer_cast<Mob>( shared_from_this() ) );
			if (player->hurt(damageSource, getAttackDamage()))
			{
			}
			delete damageSource;
		}
	}
}

bool Nusagar::isDealsDamage()
{
	return true;
}

int Nusagar::getAttackDamage()
{
	return 50;
}

bool Nusagar::isPushable()
{
	return false;
}

void Nusagar::doPush(shared_ptr<Entity> e)
{
}

void Nusagar::pushEntities()
{
}

int Nusagar::decreaseAirSupply(int currentSupply)
{
	return currentSupply;
}

void Nusagar::causeFallDamage(float distance)
{
}

bool Nusagar::canBeAffected(MobEffectInstance *newEffect)
{
	return false;
}

void Nusagar::addAdditonalSaveData(CompoundTag *tag)
{
	FlyingMonster::addAdditonalSaveData(tag);

	tag->putInt(L"originPointX", originPointX);
	tag->putInt(L"originPointY", originPointY);
	tag->putInt(L"originPointZ", originPointZ);
	tag->putBoolean(L"spawnLevel1", spawnLevel1);
	tag->putBoolean(L"spawnLevel2", spawnLevel2);
	tag->putBoolean(L"spawnLevel3", spawnLevel3);
	tag->putString(L"bossName", getBossName());
}

void Nusagar::readAdditionalSaveData(CompoundTag *tag)
{
	FlyingMonster::readAdditionalSaveData(tag);

	bOriginSet = true;

	if (tag->contains(L"originPointX")) originPointX = tag->getInt(L"originPointX");
	if (tag->contains(L"originPointY")) originPointY = tag->getInt(L"originPointY");
	if (tag->contains(L"originPointZ")) originPointZ = tag->getInt(L"originPointZ");
	if (tag->contains(L"spawnLevel1")) spawnLevel1 = tag->getBoolean(L"spawnLevel1");
	if (tag->contains(L"spawnLevel2")) spawnLevel2 = tag->getBoolean(L"spawnLevel2");
	if (tag->contains(L"spawnLevel3")) spawnLevel3 = tag->getBoolean(L"spawnLevel3");
	if (tag->contains(L"bossName")) setBossName(tag->getString(L"bossName"));
}

int Nusagar::getHurtSound()
{
	return eSoundType_MOB_NUSAGAR_HURT;
}

int Nusagar::getDeathSound()
{
	return eSoundType_MOB_NUSAGAR_HURT;
}