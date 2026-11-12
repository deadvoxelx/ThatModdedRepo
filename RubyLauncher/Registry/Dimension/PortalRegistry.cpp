#include "PortalRegistry.h"
#include "Client/ModTile.h"
#include "Loader.h"
#include "Host/RubyLauncherHost.h"
#include "Registry/Dimension/DimensionRegistry.h"
#include "Registry/IDs.h"
#include "Level.h"
#include "MinecraftServer.h"
#include "Tile.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.level.dimension.h"
#include <map>
#include <vector>

namespace
{
	struct ResolvedPortal
	{
		std::string name;
		bool hasFrom = false;
		int fromSlot = 0;
		int toSlot = 0;
		int cooldown = 100;
	};

	std::vector<PortalDefinition> g_pending;
	std::map<int, std::vector<ResolvedPortal> > g_portals;

	std::string describe(const PortalDefinition &definition)
	{
		return definition.modId + ":" + definition.portalId;
	}

	bool fillFrame(Level *level, int portalId, int frameId, int x, int y, int z, int xd, int zd)
	{
		for (int xx = -1; xx <= 2; xx++)
		{
			for (int yy = -1; yy <= 3; yy++)
			{
				const bool edge = (xx == -1) || (xx == 2) || (yy == -1) || (yy == 3);
				if ((xx == -1 || xx == 2) && (yy == -1 || yy == 3)) continue;

				const int tile = level->getTile(x + xd * xx, y + yy, z + zd * xx);

				if (edge)
				{
					if (tile != frameId) return false;
				}
				else if (tile != 0 && tile != Tile::fire_Id)
				{
					return false;
				}
			}
		}

		for (int xx = 0; xx < 2; xx++)
		{
			for (int yy = 0; yy < 3; yy++)
			{
				level->setTileAndData(x + xd * xx, y + yy, z + zd * xx, portalId, 0, Tile::UPDATE_CLIENTS);
			}
		}
		return true;
	}

	bool createPortal(Level *level, ModTile *portal, int x, int y, int z)
	{
		const int frameId = portal->getPortalFrame();
		const int portalId = portal->id;

		int xd = 0;
		int zd = 0;
		if (level->getTile(x - 1, y, z) == frameId || level->getTile(x + 1, y, z) == frameId) xd = 1;
		if (level->getTile(x, y, z - 1) == frameId || level->getTile(x, y, z + 1) == frameId) zd = 1;

		bool twoPossible = false;
		if (xd == zd)
		{
			if (xd == 1) twoPossible = true;
			else return false;
		}

		bool changedX = false;
		if (level->getTile(x - xd, y, z) == 0)
		{
			changedX = true;
			x--;
		}
		else if (level->getTile(x, y, z - zd) == 0 && !twoPossible)
		{
			z--;
		}

		if (!twoPossible) return fillFrame(level, portalId, frameId, x, y, z, xd, zd);

		if (fillFrame(level, portalId, frameId, x, y, z, xd, 0)) return true;

		if (changedX) x++;
		if (level->getTile(x, y, z - zd) == 0) z--;

		return fillFrame(level, portalId, frameId, x, y, z, 0, zd);
	}

	bool canOpen(Level *level, int x, int y, int z)
	{
		const int tile = level->getTile(x, y, z);
		if (tile == 0) return true;

		Tile *entry = Tile::tiles[tile];
		return entry != nullptr && !entry->material->blocksMotion();
	}

	bool buildArrivalFrame(Level *level, int portalId, int frameId, int x, int y, int z, int xd, int zd)
	{
		for (int xx = 0; xx < 2; xx++)
		{
			for (int yy = 0; yy < 3; yy++)
			{
				if (!canOpen(level, x + xd * xx, y + yy, z + zd * xx)) return false;
			}
		}

		for (int xx = -1; xx <= 2; xx++)
		{
			for (int yy = -1; yy <= 3; yy++)
			{
				const bool edge = (xx == -1) || (xx == 2) || (yy == -1) || (yy == 3);
				if (!edge) continue;

				level->setTileAndData(x + xd * xx, y + yy, z + zd * xx, frameId, 0, Tile::UPDATE_CLIENTS);
			}
		}

		for (int xx = 0; xx < 2; xx++)
		{
			for (int yy = 0; yy < 3; yy++)
			{
				level->setTileAndData(x + xd * xx, y + yy, z + zd * xx, portalId, 0, Tile::UPDATE_CLIENTS);
			}
		}
		return true;
	}

	bool duplicates(const PortalDefinition &definition)
	{
		for (const PortalDefinition &pending : g_pending)
		{
			if (pending.modId == definition.modId && pending.portalId == definition.portalId) return true;
		}
		return false;
	}
}

bool PortalRegistry::registerPortal(const PortalDefinition &definition)
{
	const std::string context = describe(definition);

	if (definition.modId.empty() || definition.portalId.empty())
	{
		Loader::_debugPrint("portal " + context + ": modId and portalId are required");
		return false;
	}
	if (definition.block.empty() || definition.to.empty())
	{
		Loader::_debugPrint("portal " + context + ": block and to are required");
		return false;
	}
	if (definition.bidirectional && definition.from.empty())
	{
		Loader::_debugPrint("portal " + context + ": a bidirectional portal needs a from dimension, or it would lead nowhere");
		return false;
	}
	if (definition.cooldown < 0)
	{
		Loader::_debugPrint("portal " + context + ": cooldown cannot be negative");
		return false;
	}
	if (duplicates(definition))
	{
		Loader::_debugPrint("portal " + context + ": already registered");
		return false;
	}

	g_pending.push_back(definition);
	return true;
}

void PortalRegistry::finalize()
{
	if (g_pending.empty()) return;

	g_portals.clear();

	int added = 0;

	for (const PortalDefinition &pending : g_pending)
	{
		const std::string context = describe(pending);

		IDMapping::MappedItem mapped = IDMapping::get()->getID(pending.block);

		ModTile *tile = nullptr;
		if (mapped.id > 0 && Tile::tiles[mapped.id] != nullptr)
		{
			tile = dynamic_cast<ModTile *>(Tile::tiles[mapped.id]);
		}
		if (tile == nullptr)
		{
			Loader::_debugPrint("portal " + context + ": no mod block '" + pending.block + "' (register the block first)");
			continue;
		}

		if (!pending.frame.empty())
		{
			IDMapping::MappedItem frame = IDMapping::get()->getID(pending.frame);
			if (frame.id <= 0 || Tile::tiles[frame.id] == nullptr)
			{
				Loader::_debugPrint("portal " + context + ": no block '" + pending.frame + "' for the frame");
				continue;
			}

			tile->setPortalFrame(frame.id);
		}

		const int toSlot = DimensionRegistry::slotForName(pending.to);
		if (toSlot == DimensionRegistry::NO_SLOT)
		{
			Loader::_debugPrint("portal " + context + ": no dimension '" + pending.to + "'");
			continue;
		}

		const bool hasFrom = !pending.from.empty();
		int fromSlot = 0;
		if (hasFrom)
		{
			fromSlot = DimensionRegistry::slotForName(pending.from);
			if (fromSlot == DimensionRegistry::NO_SLOT)
			{
				Loader::_debugPrint("portal " + context + ": no dimension '" + pending.from + "'");
				continue;
			}
			if (fromSlot == toSlot)
			{
				Loader::_debugPrint("portal " + context + ": leads to its own dimension");
				continue;
			}
		}

		ResolvedPortal portal;
		portal.name = context;
		portal.hasFrom = hasFrom;
		portal.fromSlot = fromSlot;
		portal.toSlot = toSlot;
		portal.cooldown = pending.cooldown;

		g_portals[mapped.id].push_back(portal);

		if (pending.bidirectional)
		{
			ResolvedPortal back = portal;
			back.hasFrom = true;
			back.fromSlot = toSlot;
			back.toSlot = fromSlot;
			g_portals[mapped.id].push_back(back);
		}

		tile->setPortalBlock(true);
		RubyCreative::removeEntry(mapped.id);

		Loader::_debugPrint("portal " + context + ": '" + pending.block + "' leads to slot " + std::to_string(toSlot) + (pending.bidirectional ? " and back" : ""));
		++added;
	}

	g_pending.clear();

	if (added == 0) Loader::_debugPrint("PortalRegistry: no portals could be registered");
}

bool PortalRegistry::tryLight(Level *level, int x, int y, int z)
{
	if (level == nullptr || level->isClientSide) return false;

	const int slot = level->dimension != nullptr ? level->dimension->id : 0;

	for (std::map<int, std::vector<ResolvedPortal> >::const_iterator it = g_portals.begin(); it != g_portals.end(); ++it)
	{
		ModTile *portal = dynamic_cast<ModTile *>(Tile::tiles[it->first]);
		if (portal == nullptr || portal->getPortalFrame() <= 0) continue;

		bool usable = false;
		for (const ResolvedPortal &candidate : it->second)
		{
			if (!candidate.hasFrom || candidate.fromSlot == slot)
			{
				usable = true;
				break;
			}
		}
		if (!usable) continue;

		if (createPortal(level, portal, x, y, z)) return true;
	}
	return false;
}

int PortalRegistry::placeArrivalPortal(Level *level, int fromSlot, int toSlot, int x, int y, int z)
{
	if (level == nullptr || level->isClientSide) return -1;

	for (std::map<int, std::vector<ResolvedPortal> >::const_iterator it = g_portals.begin(); it != g_portals.end(); ++it)
	{
		ModTile *portal = dynamic_cast<ModTile *>(Tile::tiles[it->first]);
		if (portal == nullptr || portal->getPortalFrame() <= 0) continue;

		bool usable = false;
		for (const ResolvedPortal &candidate : it->second)
		{
			if (candidate.toSlot == toSlot && (!candidate.hasFrom || candidate.fromSlot == fromSlot))
			{
				usable = true;
				break;
			}
		}
		if (!usable) continue;

		const int portalId = portal->id;
		const int frameId = portal->getPortalFrame();

		int bestY = -1;
		double bestDistance = -1;
		for (int dx = -16; dx <= 16; dx++)
		{
			for (int dz = -16; dz <= 16; dz++)
			{
				for (int yy = y + 8; yy >= y - 8; yy--)
				{
					if (level->getTile(x + dx, yy, z + dz) != portalId) continue;

					const double distance = static_cast<double>(dx * dx + dz * dz) + (yy - y) * (yy - y);
					if (bestDistance < 0 || distance < bestDistance)
					{
						bestDistance = distance;
						bestY = yy;
					}
				}
			}
		}
		if (bestY >= 0) return bestY;

		for (int axis = 0; axis < 2; axis++)
		{
			const int xd = axis == 0 ? 1 : 0;
			const int zd = axis == 0 ? 0 : 1;

			for (int offset = 0; offset <= 3; offset++)
			{
				const int bottom = y + offset;
				if (buildArrivalFrame(level, portalId, frameId, x, bottom, z, xd, zd)) return bottom;
			}
		}
	}
	return -1;
}

bool PortalRegistry::enter(Level *level, std::shared_ptr<Entity> entity, int tileId, int x, int y, int z)
{
	if (level == nullptr || entity == nullptr || level->isClientSide) return false;

	std::map<int, std::vector<ResolvedPortal> >::const_iterator portals = g_portals.find(tileId);
	if (portals == g_portals.end()) return false;

	if (entity->GetType() == eTYPE_EXPERIENCEORB) return false;
	if (entity->riding != nullptr || entity->rider.lock() != nullptr) return false;
	if (entity->changingDimensionDelay > 0) return false;

	const int fromSlot = level->dimension != nullptr ? level->dimension->id : 0;

	const ResolvedPortal *portal = nullptr;
	for (const ResolvedPortal &candidate : portals->second)
	{
		if (!candidate.hasFrom || candidate.fromSlot == fromSlot)
		{
			portal = &candidate;
			break;
		}
	}

	if (portal == nullptr || portal->toSlot == fromSlot) return false;

	MinecraftServer *server = MinecraftServer::getInstance();
	if (server == nullptr || server->getLevel(portal->toSlot) == nullptr) return false;

	Loader::_debugPrint("portal " + portal->name + ": sent an entity from slot " + std::to_string(fromSlot) + " to " + std::to_string(portal->toSlot) + " at " + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z));

	entity->changingDimensionDelay = portal->cooldown;
	entity->changeDimension(portal->toSlot);

	return true;
}
