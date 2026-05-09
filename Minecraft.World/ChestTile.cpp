#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.redstone.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.phys.h"
#include "ChestTile.h"
#include "ChestPair.h"
#include "Facing.h"

namespace
{
	// Fireblade - bunch of functions once again cause mc only got this ever in 1.13 w/ a new block nbt / blockstate system i think is what its called
	// dont quote me on that
	enum class ChestAxis
	{
		X,
		Z,
	};

	struct PendingChestPlacementIntent
	{
		bool hasValue;
		int placeX;
		int placeY;
		int placeZ;
		int targetX;
		int targetY;
		int targetZ;
	};

	PendingChestPlacementIntent pendingChestPlacementIntent = { false, 0, 0, 0, 0, 0, 0 };

	void setPendingChestPlacementIntent(int placeX, int placeY, int placeZ, int targetX, int targetY, int targetZ)
	{
		pendingChestPlacementIntent.hasValue = true;
		pendingChestPlacementIntent.placeX = placeX;
		pendingChestPlacementIntent.placeY = placeY;
		pendingChestPlacementIntent.placeZ = placeZ;
		pendingChestPlacementIntent.targetX = targetX;
		pendingChestPlacementIntent.targetY = targetY;
		pendingChestPlacementIntent.targetZ = targetZ;
	}

	inline bool isChestBlock(LevelSource *level, int chestId, int x, int y, int z)
	{
		return level->getTile(x, y, z) == chestId;
	}

	ChestAxis getChestAxis(LevelSource *level, int chestId, int x, int y, int z)
	{
		int data = level->getData(x, y, z);
		if (data == Facing::NORTH || data == Facing::SOUTH) return ChestAxis::X;
		if (data == Facing::WEST || data == Facing::EAST) return ChestAxis::Z;

		bool hasX = isChestBlock(level, chestId, x - 1, y, z) || isChestBlock(level, chestId, x + 1, y, z);
		bool hasZ = isChestBlock(level, chestId, x, y, z - 1) || isChestBlock(level, chestId, x, y, z + 1);
		if (hasZ && !hasX) return ChestAxis::Z;

		return ChestAxis::X;
	}

	bool hasOtherChestOnAxis(LevelSource *level, int chestId, int x, int y, int z, ChestAxis axis, int exceptX, int exceptZ)
	{
		if (axis == ChestAxis::X)
		{
			if (isChestBlock(level, chestId, x - 1, y, z) && !(x - 1 == exceptX && z == exceptZ)) return true;
			if (isChestBlock(level, chestId, x + 1, y, z) && !(x + 1 == exceptX && z == exceptZ)) return true;
			return false;
		}

		if (isChestBlock(level, chestId, x, y, z - 1) && !(x == exceptX && z - 1 == exceptZ)) return true;
		if (isChestBlock(level, chestId, x, y, z + 1) && !(x == exceptX && z + 1 == exceptZ)) return true;
		return false;
	}

	bool isRightCandidateFirst(LevelSource *level, int x, int y, int z, ChestAxis axis, int firstX, int firstZ)
	{
		int data = level->getData(x, y, z);

		if (axis == ChestAxis::X)
		{
			int rightX = x + 1;
			if (data == Facing::SOUTH) rightX = x - 1;
			else if (data == Facing::NORTH) rightX = x + 1;
			return firstX == rightX;
		}

		int rightZ = z + 1;
		if (data == Facing::WEST) rightZ = z - 1;
		else if (data == Facing::EAST) rightZ = z + 1;
		return firstZ == rightZ;
	}

	static bool useCandidateForAxis(LevelSource *level, int chestId, int x, int y, int z, ChestAxis axis, shared_ptr<ChestTileEntity> neighbor, int &outX, int &outZ)
	{
		if (neighbor == nullptr) return false;

		int nx = neighbor->x;
		int nz = neighbor->z;
		bool isAdjacent = (nx == x - 1 && nz == z) || (nx == x + 1 && nz == z) || (nx == x && nz == z - 1) || (nx == x && nz == z + 1);
		if (!isAdjacent) return false;

		if (axis == ChestAxis::X && nz != z) return false;
		if (axis == ChestAxis::Z && nx != x) return false;
		if (!isChestBlock(level, chestId, nx, y, nz)) return false;

		outX = nx;
		outZ = nz;
		return true;
	}

	bool getCachedChestNeighborOnAxis(LevelSource *level, int chestId, int x, int y, int z, ChestAxis axis, int &outX, int &outZ)
	{
		shared_ptr<ChestTileEntity> chestEntity = dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x, y, z));
		if (chestEntity == nullptr) return false;
		chestEntity->checkNeighbors();

		if (useCandidateForAxis(level, chestId, x, y, z, axis, chestEntity->n.lock(), outX, outZ)) return true;
		if (useCandidateForAxis(level, chestId, x, y, z, axis, chestEntity->e.lock(), outX, outZ)) return true;
		if (useCandidateForAxis(level, chestId, x, y, z, axis, chestEntity->w.lock(), outX, outZ)) return true;
		if (useCandidateForAxis(level, chestId, x, y, z, axis, chestEntity->s.lock(), outX, outZ)) return true;

		return false;
	}

	bool candidateCommittedElsewhere(LevelSource *level, int chestId, int x, int y, int z, int candidateX, int candidateZ)
	{
		int partnerX = candidateX;
		int partnerZ = candidateZ;
		ChestAxis candidateAxis = getChestAxis(level, chestId, candidateX, y, candidateZ);
		if (!getCachedChestNeighborOnAxis(level, chestId, candidateX, y, candidateZ, candidateAxis, partnerX, partnerZ)) return false;
		return !(partnerX == x && partnerZ == z);
	}

	bool candidateMatchesFacingData(LevelSource *level, int x, int y, int z, int candidateX, int candidateZ)
	{
		int selfFacingData = level->getData(x, y, z);
		if (selfFacingData == 0) return false;
		return level->getData(candidateX, y, candidateZ) == selfFacingData;
	}

	ChestPair::CandidateDecision buildCandidateDecision(LevelSource *level, int chestId, int x, int y, int z, ChestAxis axis, int candidateX, int candidateZ)
	{
		ChestPair::CandidateDecision decision = { false, false, false, false };

		int partnerX = candidateX;
		int partnerZ = candidateZ;
		ChestAxis candidateAxis = getChestAxis(level, chestId, candidateX, y, candidateZ);
		bool hasPartner = getCachedChestNeighborOnAxis(level, chestId, candidateX, y, candidateZ, candidateAxis, partnerX, partnerZ);
		decision.prefersThisChest = hasPartner && partnerX == x && partnerZ == z;
		decision.committedElsewhere = hasPartner && !decision.prefersThisChest;
		decision.matchesFacingData = candidateMatchesFacingData(level, x, y, z, candidateX, candidateZ);
		decision.available = !hasOtherChestOnAxis(level, chestId, candidateX, y, candidateZ, axis, x, z);

		return decision;
	}

	struct IsCommittedElsewhereSelector
	{
		LevelSource *level;
		int chestId;
		int x;
		int y;
		int z;

		bool operator()(int candidateX, int candidateZ) const
		{
			return candidateCommittedElsewhere(level, chestId, x, y, z, candidateX, candidateZ);
		}
	};

	struct BuildCandidateDecisionSelector
	{
		LevelSource *level;
		int chestId;
		int x;
		int y;
		int z;
		ChestAxis axis;

		ChestPair::CandidateDecision operator()(int candidateX, int candidateZ) const
		{
			return buildCandidateDecision(level, chestId, x, y, z, axis, candidateX, candidateZ);
		}
	};

	bool getPreferredChestNeighbor(LevelSource *level, int chestId, int x, int y, int z, int &outX, int &outZ)
	{
		ChestAxis axis = getChestAxis(level, chestId, x, y, z);

		int firstX = x;
		int firstZ = z;
		int secondX = x;
		int secondZ = z;

		if (axis == ChestAxis::X)
		{
			firstX = x - 1;
			secondX = x + 1;
		}
		else
		{
			firstZ = z - 1;
			secondZ = z + 1;
		}

		bool hasFirst = isChestBlock(level, chestId, firstX, y, firstZ);
		bool hasSecond = isChestBlock(level, chestId, secondX, y, secondZ);

		bool hasPlacementIntent = false;
		int intendedPartnerX = x;
		int intendedPartnerY = y;
		int intendedPartnerZ = z;
		bool strictPlacementIntent = false;
		shared_ptr<ChestTileEntity> preferenceEntity = dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x, y, z));
		if (preferenceEntity != nullptr)
		{
			hasPlacementIntent = preferenceEntity->getPlacementPartnerIntent(intendedPartnerX, intendedPartnerY, intendedPartnerZ, strictPlacementIntent);
			if (hasPlacementIntent && !isChestBlock(level, chestId, intendedPartnerX, intendedPartnerY, intendedPartnerZ))
			{
				if (strictPlacementIntent) return false;
				preferenceEntity->clearPlacementPartnerIntent();
				hasPlacementIntent = false;
			}
		}

		int cachedX = x;
		int cachedZ = z;
		bool hasCachedPartner = getCachedChestNeighborOnAxis(level, chestId, x, y, z, axis, cachedX, cachedZ);
		bool rightIsFirst = isRightCandidateFirst(level, x, y, z, axis, firstX, firstZ);

		ChestPair::PreferredNeighborInput input =
		{
			firstX,
			firstZ,
			secondX,
			secondZ,
			hasFirst,
			hasSecond,
			hasPlacementIntent,
			intendedPartnerX,
			intendedPartnerY,
			intendedPartnerZ,
			y,
			strictPlacementIntent,
			hasCachedPartner,
			cachedX,
			cachedZ,
			rightIsFirst,
		};

		IsCommittedElsewhereSelector isCommittedElsewhere = { level, chestId, x, y, z };
		BuildCandidateDecisionSelector candidateDecisionSelector = { level, chestId, x, y, z, axis };

		return ChestPair::choosePreferredNeighbor(
			input,
			isCommittedElsewhere,
			candidateDecisionSelector,
			outX,
			outZ);
	}

	bool getMutualChestNeighbor(LevelSource *level, int chestId, int x, int y, int z, int &outX, int &outZ)
	{
		int neighborX = x;
		int neighborZ = z;
		if (!getPreferredChestNeighbor(level, chestId, x, y, z, neighborX, neighborZ)) return false;

		int backX = neighborX;
		int backZ = neighborZ;
		if (!getPreferredChestNeighbor(level, chestId, neighborX, y, neighborZ, backX, backZ)) return false;

		if (backX != x || backZ != z) return false;

		outX = neighborX;
		outZ = neighborZ;
		return true;
	}
}

ChestTile::ChestTile(int id, int type) : BaseEntityTile(id, Material::wood, isSolidRender() )
{
	random = new Random();
	this->type = type;

	setShape(1 / 16.0f, 0, 1 / 16.0f, 15 / 16.0f, 14 / 16.0f, 15 / 16.0f);
}

ChestTile::~ChestTile()
{
	delete random;
}

bool ChestTile::isSolidRender(bool isServerLevel)
{
	return false;
}

bool ChestTile::isCubeShaped()
{
	return false;
}

int ChestTile::getRenderShape()
{
	return Tile::SHAPE_ENTITYTILE_ANIMATED;
}

// fix chest hitbox [https://discord.com/channels/1482443865754701868/1494370281073016953]

AABB *ChestTile::getTileAABB(Level *level, int x, int y, int z)
{
	updateShape(level, x, y, z, -1, shared_ptr<TileEntity>());
	return Tile::getTileAABB(level, x, y, z);
}

AABB *ChestTile::getAABB(Level *level, int x, int y, int z)
{
	updateShape(level, x, y, z, -1, shared_ptr<TileEntity>());
	return Tile::getAABB(level, x, y, z);
}

int ChestTile::getPlacedOnFaceDataValue(Level *level, int x, int y, int z, int face, float clickX, float clickY, float clickZ, int itemValue)
{
	int clickedX = x;
	int clickedY = y;
	int clickedZ = z;

	if (face == Facing::DOWN) clickedY = y + 1;
	else if (face == Facing::UP) clickedY = y - 1;
	else if (face == Facing::NORTH) clickedZ = z + 1;
	else if (face == Facing::SOUTH) clickedZ = z - 1;
	else if (face == Facing::WEST) clickedX = x + 1;
	else if (face == Facing::EAST) clickedX = x - 1;

	setPendingChestPlacementIntent(x, y, z, clickedX, clickedY, clickedZ);

	return itemValue;
}

// Fireblade - recalculate facing direction + relation to neighboring chests
void ChestTile::updateShape(LevelSource *level, int x, int y, int z, int forceData, shared_ptr<TileEntity> forceEntity)
{
	int partnerX = x;
	int partnerZ = z;
	if (getMutualChestNeighbor(level, id, x, y, z, partnerX, partnerZ))
	{
		if (partnerZ < z)
		{
			setShape(1 / 16.0f, 0, 0, 15 / 16.0f, 14 / 16.0f, 15 / 16.0f);
			return;
		}
		if (partnerZ > z)
		{
			setShape(1 / 16.0f, 0, 1 / 16.0f, 15 / 16.0f, 14 / 16.0f, 1);
			return;
		}
		if (partnerX < x)
		{
			setShape(0, 0, 1 / 16.0f, 15 / 16.0f, 14 / 16.0f, 15 / 16.0f);
			return;
		}
		if (partnerX > x)
		{
			setShape(1 / 16.0f, 0, 1 / 16.0f, 1, 14 / 16.0f, 15 / 16.0f);
			return;
		}
	}

	setShape(1 / 16.0f, 0, 1 / 16.0f, 15 / 16.0f, 14 / 16.0f, 15 / 16.0f);
}

void ChestTile::onPlace(Level *level, int x, int y, int z)
{
	BaseEntityTile::onPlace(level, x, y, z);
	recalcLockDir(level, x, y, z);

	int partnerX = x;
	int partnerZ = z;
	// Fireblade - recalculate neighbor on place
	if (getMutualChestNeighbor(level, id, x, y, z, partnerX, partnerZ))
	{
		recalcLockDir(level, partnerX, y, partnerZ);
	}
}

void ChestTile::setPlacedBy(Level *level, int x, int y, int z, shared_ptr<LivingEntity> by, shared_ptr<ItemInstance> itemInstance)
{
	auto neighborCanRetargetToPlacedChest = [&](int neighborX, int neighborZ) -> bool
	{
		if (level->getTile(neighborX, y, neighborZ) != id) return false;

		int partnerX = neighborX;
		int partnerZ = neighborZ;
		if (!getMutualChestNeighbor(level, id, neighborX, y, neighborZ, partnerX, partnerZ)) return true;

		return partnerX == x && partnerZ == z;
	};

	int explicitPartnerX = 0;
	int explicitPartnerY = 0;
	int explicitPartnerZ = 0;
	bool hasExplicitPartnerTarget =
		pendingChestPlacementIntent.hasValue &&
		pendingChestPlacementIntent.placeX == x &&
		pendingChestPlacementIntent.placeY == y &&
		pendingChestPlacementIntent.placeZ == z;

	if (hasExplicitPartnerTarget)
	{
		explicitPartnerX = pendingChestPlacementIntent.targetX;
		explicitPartnerY = pendingChestPlacementIntent.targetY;
		explicitPartnerZ = pendingChestPlacementIntent.targetZ;
	}

	pendingChestPlacementIntent.hasValue = false;

	bool strictTargetSelection = hasExplicitPartnerTarget && by != nullptr && by->isSneaking();
	bool explicitTargetCanPairWithPlaced = false;
	if (strictTargetSelection && explicitPartnerY == y)
	{
		bool adjacentX = (explicitPartnerX == x - 1 || explicitPartnerX == x + 1) && explicitPartnerZ == z;
		bool adjacentZ = (explicitPartnerZ == z - 1 || explicitPartnerZ == z + 1) && explicitPartnerX == x;
		explicitTargetCanPairWithPlaced = adjacentX || adjacentZ;
	}

	// Fireblade - clear + set relation intents
	if (explicitTargetCanPairWithPlaced && level->getTile(explicitPartnerX, explicitPartnerY, explicitPartnerZ) == id)
	{
		shared_ptr<ChestTileEntity> explicitTargetEntity = dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(explicitPartnerX, explicitPartnerY, explicitPartnerZ));
		if (explicitTargetEntity != nullptr)
		{
			explicitTargetEntity->clearPlacementPartnerIntent();
			explicitTargetEntity->clearCache();
		}
	}

	shared_ptr<ChestTileEntity> placedChestEntity = dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x, y, z));
	if (placedChestEntity != nullptr)
	{
		if (strictTargetSelection)
		{
			placedChestEntity->setPlacementPartnerIntent(explicitPartnerX, explicitPartnerY, explicitPartnerZ, true);
		}
		else
		{
			placedChestEntity->clearPlacementPartnerIntent();
		}
	}

	int facing = 0;
	int dir = (Mth::floor(by->yRot * 4 / (360) + 0.5)) & 3;

	if (dir == 0) facing = Facing::NORTH;
	if (dir == 1) facing = Facing::EAST;
	if (dir == 2) facing = Facing::SOUTH;
	if (dir == 3) facing = Facing::WEST;

	// Fireblade - make chest face player [idk why but it didnt work half the time before]
	level->setData(x, y, z, facing, Tile::UPDATE_ALL);

	int partnerX = x;
	int partnerZ = z;
	bool hasMutualPartner = getMutualChestNeighbor(level, id, x, y, z, partnerX, partnerZ);
	if (hasMutualPartner && neighborCanRetargetToPlacedChest(partnerX, partnerZ))
	{
		level->setData(partnerX, y, partnerZ, facing, Tile::UPDATE_ALL);
	}

	if (itemInstance->hasCustomHoverName() && placedChestEntity != nullptr)
	{
		placedChestEntity->setCustomName(itemInstance->getHoverName());
	}

	if (placedChestEntity != nullptr)
	{
		// Fireblade - shift right click selection, failure to attach to the selected block forces the placed chest to be single
		if (!strictTargetSelection || hasMutualPartner)
		{
			placedChestEntity->clearPlacementPartnerIntent();
		}
	}

}

void ChestTile::recalcLockDir(Level *level, int x, int y, int z)
{
	if (level->isClientSide)
	{
		return;
	}

	int n = level->getTile(x, y, z - 1); // face = 2
	int s = level->getTile(x, y, z + 1); // face = 3
	int w = level->getTile(x - 1, y, z); // face = 4
	int e = level->getTile(x + 1, y, z); // face = 5

	int partnerX = x;
	int partnerZ = z;
	bool hasPartner = getMutualChestNeighbor(level, id, x, y, z, partnerX, partnerZ);

	// Long!
	int lockDir = 4;
	if (hasPartner && partnerZ != z)
	{
		int w2 = level->getTile(x - 1, y, partnerZ);
		int e2 = level->getTile(x + 1, y, partnerZ);

		lockDir = 5;

		int otherDir = level->getData(x, y, partnerZ);
		if (otherDir == 4) lockDir = 4;

		if ((Tile::solid[w] || Tile::solid[w2]) && !Tile::solid[e] && !Tile::solid[e2]) lockDir = 5;
		if ((Tile::solid[e] || Tile::solid[e2]) && !Tile::solid[w] && !Tile::solid[w2]) lockDir = 4;
	}
	else if (hasPartner && partnerX != x)
	{
		int n2 = level->getTile(partnerX, y, z - 1);
		int s2 = level->getTile(partnerX, y, z + 1);

		lockDir = 3;
		int otherDir = level->getData(partnerX, y, z);
		if (otherDir == 2) lockDir = 2;

		if ((Tile::solid[n] || Tile::solid[n2]) && !Tile::solid[s] && !Tile::solid[s2]) lockDir = 3;
		if ((Tile::solid[s] || Tile::solid[s2]) && !Tile::solid[n] && !Tile::solid[n2]) lockDir = 2;
	}
	else
	{
		lockDir = 3;
		if (Tile::solid[n] && !Tile::solid[s]) lockDir = 3;
		if (Tile::solid[s] && !Tile::solid[n]) lockDir = 2;
		if (Tile::solid[w] && !Tile::solid[e]) lockDir = 5;
		if (Tile::solid[e] && !Tile::solid[w]) lockDir = 4;
	}

	level->setData(x, y, z, lockDir, Tile::UPDATE_ALL);
}

bool ChestTile::mayPlace(Level *level, int x, int y, int z)
{
	return true;
}

void ChestTile::neighborChanged(Level *level, int x, int y, int z, int type)
{
	BaseEntityTile::neighborChanged(level, x, y, z, type);
	shared_ptr<ChestTileEntity> cte = dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(x, y, z));
	if (cte != nullptr) cte->clearCache();
}

void ChestTile::onRemove(Level *level, int x, int y, int z, int id, int data)
{
	shared_ptr<Container> container = dynamic_pointer_cast<ChestTileEntity>( level->getTileEntity(x, y, z) );
	if (container != nullptr )
	{
		for (unsigned int i = 0; i < container->getContainerSize(); i++)
		{
			shared_ptr<ItemInstance> item = container->getItem(i);
			if (item != nullptr)
			{
				float xo = random->nextFloat() * 0.8f + 0.1f;
				float yo = random->nextFloat() * 0.8f + 0.1f;
				float zo = random->nextFloat() * 0.8f + 0.1f;

				while (item->count > 0)
				{
					int count = random->nextInt(21) + 10;
					if (count > item->count) count = item->count;
					item->count -= count;

					shared_ptr<ItemInstance> newItem = std::make_shared<ItemInstance>(item->id, count, item->getAuxValue());
					newItem->set4JData( item->get4JData() );
					shared_ptr<ItemEntity> itemEntity = std::make_shared<ItemEntity>(level, x + xo, y + yo, z + zo, newItem);
					float pow = 0.05f;
					itemEntity->xd = static_cast<float>(random->nextGaussian()) * pow;
					itemEntity->yd = static_cast<float>(random->nextGaussian()) * pow + 0.2f;
					itemEntity->zd = static_cast<float>(random->nextGaussian()) * pow;
					if (item->hasTag())
					{
						itemEntity->getItem()->setTag(static_cast<CompoundTag *>(item->getTag()->copy()));
					}

					level->addEntity(itemEntity);
				}

				// 4J Stu - Fix for duplication glitch
				container->setItem(i,nullptr);
			}
		}
		level->updateNeighbourForOutputSignal(x, y, z, id);
	}
	BaseEntityTile::onRemove(level, x, y, z, id, data);
}

// 4J-PB - Adding a TestUse for tooltip display
bool ChestTile::TestUse()
{
	return true;
}

// 4J-PB - changing to 1.5 equivalent
bool ChestTile::use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly/*=false*/) // 4J added soundOnly param
{
	if( soundOnly ) return true;

	if (level->isClientSide)
	{
		return true;
	}
	shared_ptr<Container> container = getContainer(level, x, y, z);

	if (container != nullptr)
	{
		player->openContainer(container);
	}

	return true;
}

shared_ptr<Container> ChestTile::getContainer(Level *level, int x, int y, int z)
{
	shared_ptr<Container> container = dynamic_pointer_cast<ChestTileEntity>( level->getTileEntity(x, y, z) );
	if (container == nullptr) return nullptr;

	if (level->isSolidBlockingTile(x, y + 1, z)) return nullptr;
	if (isCatSittingOnChest(level,x, y, z)) return nullptr;	


	// Fireblade - check for + combine with neighbor if applicable
	int partnerX = x;
	int partnerZ = z;
	if (!getMutualChestNeighbor(level, id, x, y, z, partnerX, partnerZ)) return container;

	if (level->isSolidBlockingTile(partnerX, y + 1, partnerZ)) return nullptr;
	if (isCatSittingOnChest(level, partnerX, y, partnerZ)) return nullptr;

	shared_ptr<Container> partnerContainer = dynamic_pointer_cast<ChestTileEntity>(level->getTileEntity(partnerX, y, partnerZ));
	if (partnerContainer == nullptr) return container;

	if (partnerX < x || partnerZ < z)
	{
		container = std::make_shared<CompoundContainer>(IDS_CHEST_LARGE, partnerContainer, container);
	}
	else
	{
		container = std::make_shared<CompoundContainer>(IDS_CHEST_LARGE, container, partnerContainer);
	}

	return container;
}

shared_ptr<TileEntity> ChestTile::newTileEntity(Level *level)
{
	MemSect(50);
	shared_ptr<TileEntity> retval = std::make_shared<ChestTileEntity>();
	MemSect(0);
	return retval;
}

bool ChestTile::isSignalSource()
{
	return type == TYPE_TRAP;
}

int ChestTile::getSignal(LevelSource *level, int x, int y, int z, int dir)
{
	if (!isSignalSource()) return Redstone::SIGNAL_NONE;

	int openCount = dynamic_pointer_cast<ChestTileEntity>( level->getTileEntity(x, y, z))->openCount;
	return Mth::clamp(openCount, Redstone::SIGNAL_NONE, Redstone::SIGNAL_MAX);
}

int ChestTile::getDirectSignal(LevelSource *level, int x, int y, int z, int dir)
{
	if (dir == Facing::UP)
	{
		return getSignal(level, x, y, z, dir);
	}
	else
	{
		return Redstone::SIGNAL_NONE;
	}
}

bool ChestTile::isCatSittingOnChest(Level *level, int x, int y, int z) 
{
	vector<shared_ptr<Entity> > *entities = level->getEntitiesOfClass(typeid(Ocelot), AABB::newTemp(x, y + 1, z, x + 1, y + 2, z + 1));
	if ( entities )
	{
		for (auto& it : *entities)
		{
			shared_ptr<Ocelot> ocelot = dynamic_pointer_cast<Ocelot>(it);
			if ( ocelot && ocelot->isSitting())
			{
				delete entities;
				return true;
			}
		}
		delete entities;
	}
	return false;
}

bool ChestTile::hasAnalogOutputSignal()
{
	return true;
}

int ChestTile::getAnalogOutputSignal(Level *level, int x, int y, int z, int dir)
{
	return AbstractContainerMenu::getRedstoneSignalFromContainer(getContainer(level, x, y, z));
}

void ChestTile::registerIcons(IconRegister *iconRegister) 
{
	// Register wood as the chest's icon, because it's used by the particles
	// when destroying the chest
	icon = iconRegister->registerIcon(L"planks_oak");
}