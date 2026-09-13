#pragma once

#include "UIScene.h"

#define BUTTON_HAO_WIKI					0
#define BUTTON_HAO_SETTINGS				1
#define BUTTON_HAO_GRAPHICS				2
#define BUTTON_HAO_UI					3
#define BUTTON_HAO_SENSITIVITY			4
#define BUTTON_HAO_CONTROLS				5
#define BUTTON_HAO_CREDITS				6
#define BUTTON_HAO_MODS				7
#define BUTTONS_HAO_MAX			BUTTON_HAO_MODS + 1

class UIScene_HelpAndOptionsMenu : public UIScene
{
private:
	enum EControls
	{
		eControl_Music = 100,
		eControl_Sound
	};

	UIControl_Button m_buttons[BUTTONS_HAO_MAX];
	UIControl_Slider m_sliderMusic, m_sliderSound;
	UI_BEGIN_MAP_ELEMENTS_AND_NAMES(UIScene)
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_WIKI], "Button1")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_SETTINGS], "Button2")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_GRAPHICS], "Button3")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_UI], "Button4")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_SENSITIVITY], "Button5")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_CONTROLS], "Button6")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_CREDITS], "Button7")
		UI_MAP_ELEMENT( m_buttons[BUTTON_HAO_MODS], "Button8")
		UI_MAP_ELEMENT( m_sliderMusic, "Music")
		UI_MAP_ELEMENT( m_sliderSound, "Sound")
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

	virtual void handleSliderMove(F64 sliderId, F64 currentValue);

protected:
	void handlePress(F64 controlId, F64 childId);
};
