#pragma once

#include "Cow.h"

class Moobloom : public Cow
{
public:
	eINSTANCEOF GetType() { return eTYPE_MOOBLOOM; }
	static Entity *create(Level *level) { return new Moobloom(level); }

public:
	Moobloom(Level *level);

	virtual bool mobInteract(shared_ptr<Player> player);
	virtual bool canSpawn();
	virtual shared_ptr<AgableMob> getBreedOffspring(shared_ptr<AgableMob> target);
};