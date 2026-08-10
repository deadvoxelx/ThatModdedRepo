#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.dimension.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.item.enchantment.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.network.packet.h"
#include "..\Minecraft.Client\ServerPlayer.h"
#include "..\Minecraft.Client\PlayerConnection.h"
#include "SoundTypes.h"
#include "DartEnchanted.h"

// base damage, multiplied with velocity
const double DartEnchanted::DART_BASE_DAMAGE = 4.0f;

void DartEnchanted::_init()
{
	this->defineSynchedData();

	xTile = -1;
	yTile = -1;
	zTile = -1;
	lastTile = 0;
	lastData = 0;
	inGround = false;
	pickup = PICKUP_DISALLOWED;
	shakeTime = 0;
	flightTime = 0;

	owner = nullptr;
	life = 0;

	baseDamage = DART_BASE_DAMAGE;
	knockback = 0;
}

DartEnchanted::DartEnchanted(Level *level) : Entity( level )
{
	_init();
	
	viewScale = 10;
	setSize(0.5f, 0.5f);
}

DartEnchanted::DartEnchanted(Level *level, shared_ptr<LivingEntity> mob, shared_ptr<LivingEntity> target, float power, float uncertainty) : Entity( level )
{
	_init();
	
	viewScale = 10;
	owner = mob;
	if ( mob->instanceof(eTYPE_PLAYER) ) pickup = PICKUP_ALLOWED;

	y = mob->y + mob->getHeadHeight() - 0.1f;

	double xd = target->x - mob->x;
	double yd = (target->y + target->getHeadHeight() - 0.7f) - y;
	double zd = target->z - mob->z;
	double sd = sqrt(xd * xd + zd * zd);
	if (sd < 0.0000001) return;

	float yRot = static_cast<float>(atan2(zd, xd) * 180 / PI) - 90;
	float xRot = static_cast<float>(-(atan2(yd, sd) * 180 / PI));

	double xdn = xd / sd;
	double zdn = zd / sd;
	moveTo(mob->x + xdn, y, mob->z + zdn, yRot, xRot);
	heightOffset = 0;

	float yo = static_cast<float>(sd) * 0.2f;
	shoot(xd, yd + yo, zd, power, uncertainty);
}

DartEnchanted::DartEnchanted(Level *level, double x, double y, double z) : Entity( level )
{
	_init();
	
	viewScale = 10;
	setSize(0.5f, 0.5f);

	setPos(x, y, z);
	heightOffset = 0;
}
 
DartEnchanted::DartEnchanted(Level *level, shared_ptr<LivingEntity> mob, float power) : Entity( level )
{
	_init();

	viewScale = 10;
	owner = mob;
	if ( mob->instanceof(eTYPE_PLAYER) ) pickup = PICKUP_ALLOWED;

	setSize(0.5f, 0.5f);

	moveTo(mob->x, mob->y + mob->getHeadHeight(), mob->z, mob->yRot, mob->xRot);

	x -= Mth::cos(yRot / 180 * PI) * 0.16f;
	y -= 0.1f;
	z -= Mth::sin(yRot / 180 * PI) * 0.16f;
	setPos(x, y, z);
	heightOffset = 0;

	xd = -Mth::sin(yRot / 180 * PI) * Mth::cos(xRot / 180 * PI);
	zd = Mth::cos(yRot / 180 * PI) * Mth::cos(xRot / 180 * PI);
	yd = -Mth::sin(xRot / 180 * PI);

	shoot(xd, yd, zd, power * 1.5f, 1);
}

void DartEnchanted::defineSynchedData()
{
	entityData->define(ID_FLAGS, static_cast<byte>(0));
}

void DartEnchanted::shoot(double xd, double yd, double zd, float pow, float uncertainty)
{
	float dist = static_cast<float>(sqrt(xd * xd + yd * yd + zd * zd));

	xd /= dist;
	yd /= dist;
	zd /= dist;

	xd += (random->nextGaussian() * (random->nextBoolean() ? -1 : 1)) * 0.0075f * uncertainty;
	yd += (random->nextGaussian() * (random->nextBoolean() ? -1 : 1)) * 0.0075f * uncertainty;
	zd += (random->nextGaussian() * (random->nextBoolean() ? -1 : 1)) * 0.0075f * uncertainty;

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

void DartEnchanted::lerpTo(double x, double y, double z, float yRot, float xRot, int steps)
{
	setPos(x, y, z);
	setRot(yRot, xRot);
}

void DartEnchanted::lerpMotion(double xd, double yd, double zd)
{
	this->xd = xd;
	this->yd = yd;
	this->zd = zd;
	if (xRotO == 0 && yRotO == 0)
	{
		double sd = sqrt(xd * xd + zd * zd);
		yRotO = yRot = static_cast<float>(atan2(xd, zd) * 180 / PI);
		xRotO = xRot = static_cast<float>(atan2(yd, sd) * 180 / PI);
		xRotO = xRot;
		yRotO = yRot;
		app.DebugPrintf("%f %f : 0x%x\n",xRot,yRot,&yRot);
		moveTo(x, y, z, yRot, xRot);
		life = 0;
	}
}

void DartEnchanted::tick() 
{
	Entity::tick();

	if (!level->isClientSide)
	{
		if (!level->hasChunkAt(Mth::floor(x), Mth::floor(y), Mth::floor(z)))
		{
			remove();
			return;
		}

		int minXZ = -(level->dimension->getXZSize() * 16) / 2;
		int maxXZ = (level->dimension->getXZSize() * 16) / 2 - 1;
		if ((x <= minXZ) || (x >= maxXZ) || (z <= minXZ) || (z >= maxXZ))
		{
			remove();
			return;
		}
	}


	if (xRotO == 0 && yRotO == 0) 
	{
		double sd = sqrt(xd * xd + zd * zd);
		yRotO = yRot = static_cast<float>(atan2(xd, zd) * 180 / PI);
		xRotO = xRot = static_cast<float>(atan2(yd, sd) * 180 / PI);
	}


	{
		int t = level->getTile(xTile, yTile, zTile);
		if (t > 0 && Tile::tiles[t] != nullptr)
		{
			Tile::tiles[t]->updateShape(level, xTile, yTile, zTile);
			AABB *aabb = Tile::tiles[t]->getAABB(level, xTile, yTile, zTile);
			if (aabb != nullptr && aabb->contains(Vec3::newTemp(x, y, z)))
			{
				inGround = true;
			}
		}

	}

	if (shakeTime > 0) shakeTime--;

	if (inGround)
	{
		int tile = level->getTile(xTile, yTile, zTile);
		int data = level->getData(xTile, yTile, zTile);
		if (tile != lastTile || data != lastData)
		{
			inGround = false;

			xd *= random->nextFloat() * 0.2f;
			yd *= random->nextFloat() * 0.2f;
			zd *= random->nextFloat() * 0.2f;
			life = 0;
			flightTime = 0;
			return;
		} 

		else 
		{
			life++;
			if (life == 20 * 60) remove();
			return;
		}
	} 
	
	else 
	{
		flightTime++;
	}

	Vec3 *from = Vec3::newTemp(x, y, z);
	Vec3 *to = Vec3::newTemp(x + xd, y + yd, z + zd);
	HitResult *res = level->clip(from, to, false, true);

	from = Vec3::newTemp(x, y, z);
	to = Vec3::newTemp(x + xd, y + yd, z + zd);
	if (res != nullptr)
	{
		to = Vec3::newTemp(res->pos->x, res->pos->y, res->pos->z);
	}
	shared_ptr<Entity> hitEntity = nullptr;
	vector<shared_ptr<Entity> > objects = *level->getEntities(shared_from_this(), this->bb->expand(xd, yd, zd)->grow(1, 1, 1));
	double nearest = 0;
	for ( auto& e : objects )
	{
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

	if ( (res != nullptr) && (res->entity != nullptr) && res->entity->instanceof(eTYPE_PLAYER))
	{
		shared_ptr<Player> player = dynamic_pointer_cast<Player>(res->entity);
		if ( player->abilities.invulnerable || ((owner != nullptr) && (owner->instanceof(eTYPE_PLAYER) && !dynamic_pointer_cast<Player>(owner)->canHarmPlayer(player))))
		{
			res = nullptr;
		}
	}

	if (res != nullptr)
	{
		if (res->entity != nullptr)
		{
			float pow = Mth::sqrt(xd * xd + yd * yd + zd * zd);
			int dmg = (int) Mth::ceil(static_cast<float>(pow * baseDamage));

			if(isCritDart()) dmg += random->nextInt(dmg / 2 + 2);

			DamageSource *damageSource = nullptr;
			if (owner == nullptr)
			{
				damageSource = DamageSource::dartEnchanted(dynamic_pointer_cast<DartEnchanted>(shared_from_this()), shared_from_this());
			}
			else
			{
				damageSource = DamageSource::dartEnchanted(dynamic_pointer_cast<DartEnchanted>(shared_from_this()), owner);
			}

			if(!res->entity->isInvulnerable())
			{
				if (isOnFire() && res->entity->GetType() != eTYPE_ENDERMAN)
				{
					res->entity->setOnFire(5);
				}
			}

			if(res->entity->hurt(damageSource, dmg))
			{
				if (res->entity->instanceof(eTYPE_LIVINGENTITY))
				{
					shared_ptr<LivingEntity> mob = dynamic_pointer_cast<LivingEntity>(res->entity);

					if (!level->isClientSide)
					{
						mob->setArrowCount(mob->getArrowCount() + 1);
					}
					if (knockback > 0)
					{
						float pushLen = sqrt(xd * xd + zd * zd);
						if (pushLen > 0)
						{
							res->entity->push(xd * knockback * .6f / pushLen, 0.1, zd * knockback * .6f / pushLen);
						}
					}

					if (owner != nullptr)
					{
						ThornsEnchantment::doThornsAfterAttack(owner, mob, random);
					}

					if (owner != nullptr && res->entity != owner && owner->GetType() == eTYPE_SERVERPLAYER)
					{
						dynamic_pointer_cast<ServerPlayer>(owner)->connection->send(std::make_shared<GameEventPacket>(GameEventPacket::SUCCESSFUL_BOW_HIT, 0));
					}
				}
				playSound( eSoundType_RANDOM_BOW_HIT, 1.0f, 1.2f / (random->nextFloat() * 0.2f + 0.9f));
				if (res->entity->GetType() != eTYPE_ENDERDRAGON) remove();
			}			
			else
			{
				xd *= -0.1f;
				yd *= -0.1f;
				zd *= -0.1f;
				yRot += 180;
				yRotO += 180;
				flightTime = 0;
			}
			
			delete damageSource;
		}
		else
		{
			xTile = res->x;
			yTile = res->y;
			zTile = res->z;
			lastTile = level->getTile(xTile, yTile, zTile);
			lastData = level->getData(xTile, yTile, zTile);
			xd = static_cast<float>(res->pos->x - x);
			yd = static_cast<float>(res->pos->y - y);
			zd = static_cast<float>(res->pos->z - z);
			float dd = static_cast<float>(sqrt(xd * xd + yd * yd + zd * zd));
			if( dd > 0.0001f )
			{
				x -= (xd / dd) * 0.05f;
				y -= (yd / dd) * 0.05f;
				z -= (zd / dd) * 0.05f;
			}

			playSound(eSoundType_RANDOM_BOW_HIT, 1.0f, 1.2f / (random->nextFloat() * 0.2f + 0.9f));
			inGround = true;
			shakeTime = 7;
			setCritDart(false);

			if (lastTile != 0 && Tile::tiles[lastTile] != nullptr)
			{
				Tile::tiles[lastTile]->entityInside(level, xTile, yTile, zTile, shared_from_this() );
			}
		}
	}
	delete res;

	if(isCritDart())
	{
		for (int i = 0; i < 4; i++)
		{
			level->addParticle(eParticleType_crit, x + xd * i / 4.0f, y + yd * i / 4.0f, z + zd * i / 4.0f, -xd, -yd + 0.2, -zd);
		}
	}

	x += xd;
	y += yd;
	z += zd;

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

	float inertia = 0.99f;

	if (isInWater())
	{
		for (int i = 0; i < 4; i++)
		{
			float s = 1 / 4.0f;
			level->addParticle(eParticleType_bubble, x - xd * s, y - yd * s, z - zd * s, xd, yd, zd);
		}
		inertia = 0.80f;
	}

	xd *= inertia;
	yd *= inertia;
	zd *= inertia;

	setPos(x, y, z);

	checkInsideTiles();
}

void DartEnchanted::addAdditonalSaveData(CompoundTag *tag)
{
	tag->putShort(L"xTile", static_cast<short>(xTile));
	tag->putShort(L"yTile", static_cast<short>(yTile));
	tag->putShort(L"zTile", static_cast<short>(zTile));
	tag->putByte(L"inTile", static_cast<byte>(lastTile));
	tag->putByte(L"inData", static_cast<byte>(lastData));
	tag->putByte(L"shake", static_cast<byte>(shakeTime));
	tag->putByte(L"inGround", static_cast<byte>(inGround ? 1 : 0));
	tag->putByte(L"pickup", static_cast<byte>(pickup));
	tag->putDouble(L"damage", baseDamage);
}

void DartEnchanted::readAdditionalSaveData(CompoundTag *tag)
{
	xTile = tag->getShort(L"xTile");
	yTile = tag->getShort(L"yTile");
	zTile = tag->getShort(L"zTile");
	lastTile = tag->getByte(L"inTile") & 0x1ff;
	lastData = tag->getByte(L"inData") & 0xff;
	shakeTime = tag->getByte(L"shake") & 0xff;
	inGround = tag->getByte(L"inGround") == 1;
	if (tag->contains(L"damage"))
	{
		baseDamage = tag->getDouble(L"damage");
	}

	if (tag->contains(L"pickup"))
	{
		pickup = tag->getByte(L"pickup");
	}
	else if (tag->contains(L"player"))
	{
		pickup = tag->getBoolean(L"player") ? PICKUP_ALLOWED : PICKUP_DISALLOWED;
	}
}

void DartEnchanted::playerTouch(shared_ptr<Player> player)
{
	if (level->isClientSide || !inGround || shakeTime > 0) return;

	bool bRemove = pickup == PICKUP_ALLOWED || (pickup == PICKUP_CREATIVE_ONLY && player->abilities.instabuild);

	if (pickup == PICKUP_ALLOWED)
	{
		if (!player->inventory->add(std::make_shared<ItemInstance>(Item::dartEnchanted, 1)))
		{
			bRemove = false;
		}
	}

	if (bRemove)
	{
		playSound(eSoundType_RANDOM_POP, 0.2f, ((random->nextFloat() - random->nextFloat()) * 0.7f + 1.0f) * 2.0f);
		player->take(shared_from_this(), 1);
		remove();
	}
}

bool DartEnchanted::makeStepSound()
{
	return false;
}

float DartEnchanted::getShadowHeightOffs()
{
	return 0;
}

void DartEnchanted::setBaseDamage(double baseDamage)
{
	this->baseDamage = baseDamage;
}

double DartEnchanted::getBaseDamage()
{
	return baseDamage;
}

void DartEnchanted::setKnockback(int knockback)
{
	this->knockback = knockback;
}

bool DartEnchanted::isAttackable()
{
	return false;
}

void DartEnchanted::setCritDart(bool critDart)
{
	byte flags = entityData->getByte(ID_FLAGS);
	if (critDart)
	{
		entityData->set(ID_FLAGS, static_cast<byte>(flags | FLAG_CRIT));
	}
	else
	{
		entityData->set(ID_FLAGS, static_cast<byte>(flags & ~FLAG_CRIT));
	}
}

bool DartEnchanted::isCritDart()
{
	byte flags = entityData->getByte(ID_FLAGS);
	return (flags & FLAG_CRIT) != 0;
}
