#pragma once
#include "Screen.h"

class DirtBackgroundScreen : public Screen
{
public:
	DirtBackgroundScreen();
	virtual void render(int xm, int ym, float a);

protected:
	virtual void keyPressed(wchar_t eventCharacter, int eventKey);
};
