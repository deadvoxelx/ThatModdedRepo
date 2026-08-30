#pragma once
#include "Screen.h"
class Options;
using namespace std;

class OptionsScreen : public Screen
{
private:
	static const int CONTROLS_BUTTON_ID = 100;
    static const int VIDEO_BUTTON_ID = 101;
    static const int SETTINGS_BUTTON_ID = 102;
    static const int SENSITIVITY_BUTTON_ID = 103;
    static const int UI_BUTTON_ID = 104;
    static const int CREDITS_BUTTON_ID = 105;

    Screen *lastScreen;

protected:
	wstring title;

private:
	Options *options;

public:
	OptionsScreen(Screen *lastScreen, Options *options);
    virtual void init();
	static void checkPendingRestore();

protected:
	virtual void buttonClicked(Button *button);

public:
	virtual void render(int xm, int ym, float a);
};
