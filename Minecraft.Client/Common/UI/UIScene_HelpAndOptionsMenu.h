#pragma once

#include "UIScene.h"

#define BUTTON_HAO_WIKI					0
#define BUTTON_HAO_SETTINGS				1
#define BUTTON_HAO_AUDIO				2
#define BUTTON_HAO_GRAPHICS				3
#define BUTTON_HAO_UI					4
#define BUTTON_HAO_SENSITIVITY			5
#define BUTTON_HAO_CONTROLS				6
#define BUTTON_HAO_CREDITS				7
#define BUTTONS_HAO_MAX			BUTTON_HAO_CREDITS + 1

class UIScene_HelpAndOptionsMenu : public UIScene
{
private:
	UIControl_Button m_buttons[BUTTONS_HAO_MAX];
	UI_BEGIN_MAP_ELEMENTS_AND_NAMES(UIScene)
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_WIKI], "Button1")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_SETTINGS], "Button2")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_AUDIO], "Button3")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_GRAPHICS], "Button4")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_UI], "Button5")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_SENSITIVITY], "Button6")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_CONTROLS], "Button7")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_CREDITS], "Button8")
	UI_END_MAP_ELEMENTS_AND_NAMES()

	bool m_bNotInGame;

	Screen *lastScreen;

public:
	UIScene_HelpAndOptionsMenu(int iPad, void *initData, UILayer *parentLayer);
	virtual ~UIScene_HelpAndOptionsMenu();

	virtual EUIScene getSceneType() { return eUIScene_HelpAndOptionsMenu;}
	
	virtual void updateTooltips();
	virtual void updateComponents();

protected:
	// TODO: This should be pure virtual in this class
	virtual wstring getMoviePath();

	Minecraft *minecraft;

public:
	virtual void handleReload();

	// INPUT
	virtual void handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled);

protected:
	void handlePress(F64 controlId, F64 childId);
};
