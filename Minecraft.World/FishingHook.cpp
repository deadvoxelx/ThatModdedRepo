#include "stdafx.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.dimension.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.network.packet.h"
#include "com.mojang.nbt.h"
#include "..\Minecraft.Client\ServerPlayer.h"
#include "..\Minecraft.Client\PlayerConnection.h"
#include "EnchantmentHelper.h"
#include "WeighedTreasure.h"
#include "FishingHook.h"
#include "SoundTypes.h"



// 4J - added common ctor code.
void FishingHook::_init()
{
	// 4J Stu - This function call had to be moved here from the Entity ctor to ensure that
	// the derived version of the function is called
	this->defineSynchedData();

	xTile = -1;
	yTile = -1;
	zTile = -1;
	lastTile = 0;
	inGround = false;
	shakeTime = 0;
	flightTime = 0;
	nibble = 0;
	hookedIn = nullptr;

	lSteps = 0;
	lx = 0.0;
	ly = 0.0;
	lz = 0.0;
	lyr = 0.0;
	lxr = 0.0;
	lxd = 0.0;
	lyd = 0.0; 
	lzd = 0.0;
	owner = nullptr;
	life = 0;

	setSize(0.25f, 0.25f);
	noCulling = true;
}

FishingHook::FishingHook(Level *level) : Entity( level )
{
	_init();
}

FishingHook::FishingHook(Level *level, double x, double y, double z, shared_ptr<Player> owner) : Entity( level )
{
	_init();

	this->owner = owner;
	// 4J Stu - Moved this outside the ctor
	//owner->fishing = dynamic_pointer_cast<FishingHook>( shared_from_this() );

	setPos(x, y, z);
}

FishingHook::FishingHook(Level *level, shared_ptr<Player> mob) : Entity( level )
{
	_init();

	owner = mob;
	// 4J Stu - Moved this outside the ctor
	//owner->fishing = dynamic_pointer_cast<FishingHook>( shared_from_this() );

	moveTo(mob->x, mob->y + 1.62 - mob->heightOffset, mob->z, mob->yRot, mob->xRot);


	x -= Mth::cos(yRot / 180 * PI) * 0.16f;
	y -= 0.1f;
	z -= Mth::sin(yRot / 180 * PI) * 0.16f;
	setPos(x, y, z);
	heightOffset = 0;


	float speed = 0.4f;
	xd = (-Mth::sin(yRot / 180 * PI) * Mth::cos(xRot / 180 * PI)) * speed;
	zd = (Mth::cos(yRot / 180 * PI) * Mth::cos(xRot / 180 * PI)) * speed;
	yd = (-Mth::sin(xRot / 180 * PI)) * speed;

	shoot(xd, yd, zd, 1.5f, 1);
}

void FishingHook::defineSynchedData()
{
}

bool FishingHook::shouldRenderAtSqrDistance(double distance)
{
	double size = bb->getSize() * 4;
	size *= 64.0f;
	return distance < size * size;
}

void FishingHook::shoot(double xd, double yd, double zd, float pow, float uncertainty)
{
	float dist = static_cast<float>(sqrt(xd * xd + yd * yd + zd * zd));

	xd /= dist;
	yd /= dist;
	zd /= dist;

	xd += (random->nextGaussian()) * 0.0075f * uncertainty;
	yd += (random->nextGaussian()) * 0.0075f * uncertainty;
	zd += (random->nextGaussian()) * 0.0075f * uncertainty;

	xd *= pow;
	yd *= pow;
	zd *= pow;

	this->xd = xd;
	this->yd = yd;
	this->zd = zd;

	double sd = sqrt(xd * xd + zd * zd);

	yRotO = yRot = static_cast<float>(atan2(xd, zd) * 180 / PI);
	xRotO = xRot = static_cast<float>(atan2(yd, sd) * 180 / PI);
	life = 0;
}

void FishingHook::lerpTo(double x, double y, double z, float yRot, float xRot, int steps)
{
	lx = x;
	ly = y;
	lz = z;
	lyr = yRot;
	lxr = xRot;

	lSteps = steps;

	xd = lxd;
	yd = lyd;
	zd = lzd;
}

void FishingHook::lerpMotion(double xd, double yd, double zd)
{
	lxd = this->xd = xd;
	lyd = this->yd = yd;
	lzd = this->zd = zd;
}

void FishingHook::tick()
{
	Entity::tick();

	if (lSteps > 0)
	{
		double xt = x + (lx - x) / lSteps;
		double yt = y + (ly - y) / lSteps;
		double zt = z + (lz - z) / lSteps;

		double yrd = Mth::wrapDegrees(lyr - yRot);

		yRot += static_cast<float>((yrd) / lSteps);
		xRot += static_cast<float>((lxr - xRot) / lSteps);

		lSteps--;
		setPos(xt, yt, zt);
		setRot(yRot, xRot);
		return;
	}

	if (!level->isClientSide)
	{
		shared_ptr<ItemInstance> selectedItem = owner->getSelectedItem();
		if (owner->removed || !owner->isAlive() || selectedItem == nullptr || selectedItem->getItem() != Item::fishingRod || distanceToSqr(owner) > 32 * 32)
		{
			remove();
			owner->fishing = nullptr;
			return;
		}

		if (hookedIn != nullptr)
		{
			if (hookedIn->removed) hookedIn = nullptr;
			else
			{
				x = hookedIn->x;
				y = hookedIn->bb->y0 + hookedIn->bbHeight * 0.8;
				z = hookedIn->z;
				return;
			}
		}
	}

	if (shakeTime > 0) shakeTime--;

	if (inGround) 
	{
		int tile = level->getTile(xTile, yTile, zTile);
		if (tile != lastTile)
		{
			life++;
			if (life == 20 * 60) remove();
			return;
		}
		else
		{
			inGround = false;

			xd *= random->nextFloat() * 0.2f;
			yd *= random->nextFloat() * 0.2f;
			zd *= random->nextFloat() * 0.2f;
			life = 0;
			flightTime = 0;
		}
	}
	else
	{
		flightTime++;
	}

	Vec3 *from = Vec3::newTemp(x, y, z);
	Vec3 *to = Vec3::newTemp(x + xd, y + yd, z + zd);
	HitResult *res = level->clip(from, to);

	from = Vec3::newTemp(x, y, z);
	to = Vec3::newTemp(x + xd, y + yd, z + zd);
	if (res != nullptr) 
	{
		to = Vec3::newTemp(res->pos->x, res->pos->y, res->pos->z);
	}
	shared_ptr<Entity> hitEntity = nullptr;
	vector<shared_ptr<Entity> > *objects = level->getEntities(shared_from_this(), bb->expand(xd, yd, zd)->grow(1, 1, 1));
	double nearest = 0;
	for (auto it = objects->begin(); it != objects->end(); it++)
	{
		shared_ptr<Entity> e = *it; // objects->at(i);
		if (!e->isPickable() || (e == owner && flightTime < 5)) continue;

		float rr = 0.3f;
		AABB *bb = e->bb->grow(rr, rr, rr);
		HitResult *p = bb->clip(from, to);
		if (p != nullptr)
		{
			double dd = from->distanceTo(p->pos);
			if (dd < nearest || nearest == 0)
			{
				hitEntity = e;
				nearest = dd;
			}
			delete p;
		}
	}

	if (hitEntity != nullptr)
	{
		delete res;
		res = new HitResult(hitEntity);
	}

	if (res != nullptr)
	{
		if (res->entity != nullptr) 
		{
			// 4J Stu Move fix for : fix for #48587 - CRASH: Code: Gameplay: Hitting another player with the fishing bobber crashes the game. [Fishing pole, line]
			// Incorrect dynamic_pointer_cast used around the shared_from_this()
			DamageSource *damageSource = DamageSource::thrown(shared_from_this(), owner);
			if (res->entity->hurt(damageSource, 0))
			{
				hookedIn = res->entity;
			}
			delete damageSource;
		}
		else
		{
			inGround = true;
		}
	}
	delete res;

	if (inGround) return;

	move(xd, yd, zd);

	double sd = sqrt(xd * xd + zd * zd);
	yRot = static_cast<float>(atan2(xd, zd) * 180 / PI);
	xRot = static_cast<float>(atan2(yd, sd) * 180 / PI);

	while (xRot - xRotO < -180)
		xRotO -= 360;
	while (xRot - xRotO >= 180)
		xRotO += 360;

	while (yRot - yRotO < -180)
		yRotO -= 360;
	while (yRot - yRotO >= 180)
		yRotO += 360;

	xRot = xRotO + (xRot - xRotO) * 0.2f;
	yRot = yRotO + (yRot - yRotO) * 0.2f;


	float inertia = 0.92f;

	if (onGround || horizontalCollision)
	{
		inertia = 0.5f;
	}

	int steps = 5;
	double waterPercentage = 0;
	for (int i = 0; i < steps; i++)
	{
		double y0 = bb->y0 + (bb->y1 - bb->y0) * (i + 0) / steps - 2 / 16.0f + 2 / 16.0f;
		double y1 = bb->y0 + (bb->y1 - bb->y0) * (i + 1) / steps - 2 / 16.0f + 2 / 16.0f;
		AABB *bb2 = AABB::newTemp(bb->x0, y0, bb->z0, bb->x1, y1, bb->z1);
		if (level->containsLiquid(bb2, Material::water))
		{
			waterPercentage += 1.0 / steps;
		}
	}

	if (waterPercentage > 0)
	{
		if (nibble > 0)
		{
			nibble--;
		} 
		else
		{
			int nibbleOdds = 100;
			if (level->isRainingAt( Mth::floor(x), Mth::floor(y) + 1, Mth::floor(z))) nibbleOdds = 60;

			if (random->nextInt(nibbleOdds) == 0)
			{
				nibble = random->nextInt(30) + 10;
				yd -= 0.2f;
				playSound(eSoundType_RANDOM_SPLASH, 0.25f, 1 + (random->nextFloat() - random->nextFloat()) * 0.4f);
				if (level->isClientSide)
				{
					float yt = static_cast<float>(Mth::floor(bb->y0));
					for (int i = 0; i < 8; i++)
					{
						float xo = (level->random->nextFloat() * 2 - 1) * bbWidth;
						float zo = (level->random->nextFloat() * 2 - 1) * bbWidth;
						level->addParticle(eParticleType_splash, x + xo, y, z + zo, 0, 0.25, 0);
						level->addParticle(eParticleType_bubble, x + xo, y, z + zo, 0, 0.25, 0);
					}
				}

				if (!level->isClientSide && owner != nullptr && owner->GetType() == eTYPE_SERVERPLAYER)
				{
					shared_ptr<ServerPlayer> serverPlayer = dynamic_pointer_cast<ServerPlayer>(owner);
					if (serverPlayer != nullptr && serverPlayer->connection != nullptr)
					{
						wstring bubbleName = std::to_wstring(static_cast<int>(eParticleType_bubble));
						wstring splashName = std::to_wstring(static_cast<int>(eParticleType_splash));
						for (int i = 0; i < 8; i++)
						{
							float xo = (level->random->nextFloat() * 2 - 1) * bbWidth;
							float zo = (level->random->nextFloat() * 2 - 1) * bbWidth;
							serverPlayer->connection->send(std::make_shared<LevelParticlesPacket>(bubbleName, static_cast<float>(x + xo), static_cast<float>(y), static_cast<float>(z + zo), 0.0f, 0.0f, 0.0f, 0.0f, 1));
							serverPlayer->connection->send(std::make_shared<LevelParticlesPacket>(splashName, static_cast<float>(x + xo), static_cast<float>(y), static_cast<float>(z + zo), 0.0f, 0.0f, 0.0f, 0.0f, 1));
						}
					}
				}
			}
		}

	}

	if (nibble > 0) 
	{
		yd -= random->nextFloat() * random->nextFloat() * random->nextFloat() * 0.2;
	}

	double bob = waterPercentage * 2 - 1;
	yd += 0.04f * bob;
	if (waterPercentage > 0)
	{
		inertia *= 0.9;
		yd *= 0.8;
	}

	xd *= inertia;
	yd *= inertia;
	zd *= inertia;

	setPos(x, y, z);
}

void FishingHook::addAdditonalSaveData(CompoundTag *tag)
{
	tag->putShort(L"xTile", static_cast<short>(xTile));
	tag->putShort(L"yTile", static_cast<short>(yTile));
	tag->putShort(L"zTile", static_cast<short>(zTile));
	tag->putByte(L"inTile", static_cast<byte>(lastTile));
	tag->putByte(L"shake", static_cast<byte>(shakeTime));
	tag->putByte(L"inGround", static_cast<byte>(inGround ? 1 : 0));
}

void FishingHook::readAdditionalSaveData(CompoundTag *tag)
{
	xTile = tag->getShort(L"xTile");
	yTile = tag->getShort(L"yTile");
	zTile = tag->getShort(L"zTile");
	lastTile = tag->getByte(L"inTile") & 0x1ff;
	shakeTime = tag->getByte(L"shake") & 0xff;
	inGround = tag->getByte(L"inGround") == 1;
}

float FishingHook::getShadowHeightOffs()
{
	return 0;
}

int FishingHook::retrieve()
{
	if (level->isClientSide) return 0;

	int dmg = 0;
	if (hookedIn != nullptr)
	{
		double xa = owner->x - x;
		double ya = owner->y - y;
		double za = owner->z - z;

		double dist = sqrt(xa * xa + ya * ya + za * za);
		double speed = 0.1;
		hookedIn->xd += xa * speed;
		hookedIn->yd += ya * speed + sqrt(dist) * 0.08;
		hookedIn->zd += za * speed;
		dmg = 3;
	}
	else if (nibble > 0)
	{
		WeighedTreasureArray treasureArray(fishingLoot, TREASURE_ITEMS_COUNT);
		WeighedTreasureArray aetherTreasureArray(aetherFishingLoot, AETHER_TREASURE_ITEMS_COUNT);
		WeighedTreasure *treasure = static_cast<WeighedTreasure *>(WeighedRandom::getRandomItem(random, *((WeighedRandomItemArray *)&treasureArray)));
		WeighedTreasure *aetherTreasure = static_cast<WeighedTreasure *>(WeighedRandom::getRandomItem(random, *((WeighedRandomItemArray *)&aetherTreasureArray)));

		shared_ptr<ItemInstance> catchItem = catchItem;

		if (level->dimension->id == 3)
		{
			int count = aetherTreasure->getMinCount() + random->nextInt(aetherTreasure->getMaxCount() - aetherTreasure->getMinCount() + 1);
			catchItem = aetherTreasure->getItem()->copy();
			catchItem->count = count;

			if (catchItem->id == Item::enchantedBook_Id)
			{
				EnchantmentHelper::enchantItem(random, catchItem, 30);
			}
		}
		else
		{
			int count = treasure->getMinCount() + random->nextInt(treasure->getMaxCount() - treasure->getMinCount() + 1);
			catchItem = treasure->getItem()->copy();
			catchItem->count = count;

			if (catchItem->id == Item::bow_Id || catchItem->id == Item::sword_iron_Id || catchItem->id == Item::chestplate_iron_Id || catchItem->id == Item::enchantedBook_Id)
			{
				EnchantmentHelper::enchantItem(random, catchItem, 30);
			}
		}

		shared_ptr<ItemEntity> ie = std::make_shared<ItemEntity>(this->Entity::level, x, y, z, catchItem);
		
		double xa = owner->x - x;
		double ya = owner->y - y;
		double za = owner->z - z;

		double dist = sqrt(xa * xa + ya * ya + za * za);
		double speed = 0.1;
		ie->Entity::xd = xa * speed;
		ie->Entity::yd = ya * speed + sqrt(dist) * 0.08;
		ie->Entity::zd = za * speed;
		level->addEntity(ie);
		owner->level->addEntity(std::make_shared<ExperienceOrb>(owner->level, owner->x, owner->y + 0.5f, owner->z + 0.5f, random->nextInt(6) + 1)); // 4J Stu brought forward from 1.4
		dmg = 1;
	}
	if (inGround) dmg = 2;

	remove();
	owner->fishing = nullptr;
	return dmg;
}

void FishingHook::remove()
{
	Entity::remove();
	if (owner != nullptr) owner->fishing = nullptr;
}

WeighedTreasure *FishingHook::fishingLoot[FishingHook::TREASURE_ITEMS_COUNT] = 
{
	new WeighedTreasure(Item::fish_raw_Id, 0, 1, 1, 30),
	new WeighedTreasure(Item::string_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::rotten_flesh_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::bone_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::leather_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::slimeBall_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::shellFossil_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::boots_leather_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::nameTag_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::bucket_water_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::ironIngot_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::goldIngot_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::sword_iron_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::chestplate_iron_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::bow_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::enchantedBook_Id, 0, 1, 1, 1),
};

WeighedTreasure *FishingHook::aetherFishingLoot[FishingHook::AETHER_TREASURE_ITEMS_COUNT] = 
{
	new WeighedTreasure(Item::sunFish_Id, 0, 1, 1, 10),
	new WeighedTreasure(Item::moonFish_Id, 0, 1, 1, 10),
	new WeighedTreasure(Item::cloudFish_Id, 0, 1, 1, 10),
	new WeighedTreasure(Item::string_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::goldenAmber_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::swetBall_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::airFossil_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::boots_leather_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::nameTag_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::skyrootBucket_water_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::zaniteGemstone_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::goldIngot_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::dartShooterGold_Id, 0, 1, 1, 1),
	new WeighedTreasure(Item::enchantedBook_Id, 0, 1, 1, 1),
};
