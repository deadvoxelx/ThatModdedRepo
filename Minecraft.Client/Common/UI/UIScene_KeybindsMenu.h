#pragma once

#include "UIScene.h"

#define KEYBINDS_BUTTON_COUNT 18

class UIScene_KeybindsMenu : public UIScene
{
private:
	UIControl_Button m_keyButtons[KEYBINDS_BUTTON_COUNT];

	int selectedKey;
	bool m_waitForMouseRelease;

	UI_BEGIN_MAP_ELEMENTS_AND_NAMES(UIScene)
		UI_MAP_ELEMENT( m_keyButtons[0], "Button1")
		UI_MAP_ELEMENT( m_keyButtons[1], "Button2")
		UI_MAP_ELEMENT( m_keyButtons[2], "Button3")
		UI_MAP_ELEMENT( m_keyButtons[3], "Button4")
		UI_MAP_ELEMENT( m_keyButtons[4], "Button5")
		UI_MAP_ELEMENT( m_keyButtons[5], "Button6")
		UI_MAP_ELEMENT( m_keyButtons[6], "Button7")
		UI_MAP_ELEMENT( m_keyButtons[7], "Button8")
		UI_MAP_ELEMENT( m_keyButtons[8], "Button9")
		UI_MAP_ELEMENT( m_keyButtons[9], "Button10")
		UI_MAP_ELEMENT( m_keyButtons[10], "Button11")
		UI_MAP_ELEMENT( m_keyButtons[11], "Button12")
		UI_MAP_ELEMENT( m_keyButtons[12], "Button13")
		UI_MAP_ELEMENT( m_keyButtons[13], "Button14")
		UI_MAP_ELEMENT( m_keyButtons[14], "Button15")
		UI_MAP_ELEMENT( m_keyButtons[15], "Button16")
		UI_MAP_ELEMENT( m_keyButtons[16], "Button17")
		UI_MAP_ELEMENT( m_keyButtons[17], "Button18")
	UI_END_MAP_ELEMENTS_AND_NAMES()

public:
	UIScene_KeybindsMenu(int iPad, void *initData, UILayer *parentLayer);
	virtual ~UIScene_KeybindsMenu();
	virtual EUIScene getSceneType() { return eUIScene_KeybindsMenu;}

	virtual void updateTooltips();
	virtual void updateComponents();

protected:
	virtual wstring getMoviePath();

public:
	virtual void handleReload();
	virtual void handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled);
	virtual void tick();

protected:
	void handlePress(F64 controlId, F64 childId);
};