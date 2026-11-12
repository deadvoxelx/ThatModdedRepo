#pragma once

#include <climits>
#include <memory>
#include <string>
#include <vector>

class Level;
class Entity;

struct PortalDefinition
{
	std::string modId;
	std::string portalId;
	std::string block;
	std::string frame;
	std::string from;
	std::string to;

	bool bidirectional = false;

	int cooldown = 100;
};

namespace PortalRegistry
{
	bool registerPortal(const PortalDefinition &definition);

	void finalize();

	bool enter(Level *level, std::shared_ptr<Entity> entity, int tileId, int x, int y, int z);
	bool tryLight(Level *level, int x, int y, int z);

	int placeArrivalPortal(Level *level, int fromSlot, int toSlot, int x, int y, int z);
}
