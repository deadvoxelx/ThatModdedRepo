#pragma once

#include "ButtonTile.h"

class EndStoneButtonTile : public ButtonTile
{
public:
	EndStoneButtonTile(int id);

	virtual Icon *getTexture(int face, int data);
};