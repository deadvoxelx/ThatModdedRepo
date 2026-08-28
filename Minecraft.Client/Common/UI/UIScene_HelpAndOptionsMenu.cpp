#include "stdafx.h"
#include "UI.h"
#include "Screen.h"
#include "OptionsScreen.h"
#include "VideoSettingsScreen.h"
#include "ControlsScreen.h"
#include "DirtBackgroundScreen.h"
#include "UIScene_HelpAndOptionsMenu.h"
#include "..\..\Minecraft.h"
#include <fstream>
#include <windows.h>
#include <shellapi.h>

UIScene_HelpAndOptionsMenu::UIScene_HelpAndOptionsMenu(int iPad, void *initData, UILayer *parentLayer) : UIScene(iPad, parentLayer)
{
	// Setup all the Iggy references we need for this scene
	initialiseMovie();

	m_bNotInGame=(Minecraft::GetInstance()->level==nullptr);

	m_buttons[BUTTON_HAO_WIKI].init(IDS_WIKI,BUTTON_HAO_WIKI);
	m_buttons[BUTTON_HAO_SETTINGS].init(IDS_SETTINGS,BUTTON_HAO_SETTINGS);
	m_buttons[BUTTON_HAO_AUDIO].init(IDS_AUDIO,BUTTON_HAO_AUDIO);
	m_buttons[BUTTON_HAO_GRAPHICS].init(IDS_GRAPHICS,BUTTON_HAO_GRAPHICS);
	m_buttons[BUTTON_HAO_UI].init(IDS_USER_INTERFACE,BUTTON_HAO_UI);
	m_buttons[BUTTON_HAO_SENSITIVITY].init(IDS_SENSITIVITY,BUTTON_HAO_SENSITIVITY);
	m_buttons[BUTTON_HAO_CONTROLS].init(IDS_CONTROLS,BUTTON_HAO_CONTROLS);
	if (Minecraft::GetInstance()->level != nullptr)
	{
		m_buttons[BUTTON_HAO_CREDITS].init(IDS_CHANGE_SKIN,BUTTON_HAO_CREDITS);
	}
	else
	{
		m_buttons[BUTTON_HAO_CREDITS].init(IDS_CREDITS,BUTTON_HAO_CREDITS);
	}
	
	if(app.GetLocalPlayerCount()>1)
	{
		// no credits in splitscreen
		removeControl( &m_buttons[BUTTON_HAO_CREDITS], false);
	}

	// 4J-TomK Moved horizontal resize check to the end to prevent horizontal scaling for buttons that might get removed anyways (debug options for example)
	doHorizontalResizeCheck();
}

UIScene_HelpAndOptionsMenu::~UIScene_HelpAndOptionsMenu()
{
	// Note: We intentionally do NOT hide the shared Panorama/Logo/MenuBackground components
	// here. Scenes are destroyed lazily (deferred to the next UILayer::tick), whereas the
	// underlying scene regains focus and re-shows its components synchronously during the
	// navigation-back call. Hiding them in this destructor would override that re-show and
	// leave the components hidden (e.g. panorama/logo disappearing from the main menu).
	// The closeAllScenes() path hides them when a world loads, so they can't linger over the game.
}

wstring UIScene_HelpAndOptionsMenu::getMoviePath()
{
	if(app.GetLocalPlayerCount() > 1)
	{
		return L"HelpAndOptionsMenuSplit";
	}
	else
	{
		return L"HelpAndOptionsMenu";
	}
}

void UIScene_HelpAndOptionsMenu::updateTooltips()
{
	ui.SetTooltips( m_iPad, IDS_TOOLTIPS_SELECT,IDS_TOOLTIPS_BACK);
}

void UIScene_HelpAndOptionsMenu::updateComponents()
{
	bool bNotInGame=(Minecraft::GetInstance()->level==nullptr);
	if(bNotInGame)
	{
		m_parentLayer->showComponent(m_iPad,eUIComponent_Panorama,true);
		m_parentLayer->showComponent(m_iPad,eUIComponent_Logo,true);
		m_parentLayer->showComponent(m_iPad,eUIComponent_MenuBackground,true);
	}
	else
	{
		m_parentLayer->showComponent(m_iPad,eUIComponent_Panorama,false);
		m_parentLayer->showComponent(m_iPad,eUIComponent_MenuBackground,true);

		if( app.GetLocalPlayerCount() == 1 ) m_parentLayer->showComponent(m_iPad,eUIComponent_Logo,true);
		else m_parentLayer->showComponent(m_iPad,eUIComponent_Logo,false);
	}
}

void UIScene_HelpAndOptionsMenu::handleReload()
{
	if(app.GetLocalPlayerCount()>1)
	{
		// no credits in splitscreen
		removeControl( &m_buttons[BUTTON_HAO_CREDITS], false);
	}

	doHorizontalResizeCheck();
}

void UIScene_HelpAndOptionsMenu::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
{
	ui.AnimateKeyPress(m_iPad, key, repeat, pressed, released);

	switch(key)
	{
	case ACTION_MENU_CANCEL:
		if(pressed && !repeat)
		{
			navigateBack();
		}
		break;
	case ACTION_MENU_OK:
#ifdef __ORBIS__
	case ACTION_MENU_TOUCHPAD_PRESS:
#endif
	//CD - Added for audio
	if(pressed)
	{
		ui.PlayUISFX(eSFX_Press);
	}

	case ACTION_MENU_UP:
	case ACTION_MENU_DOWN:
		sendInputToMovie(key, repeat, pressed, released);
		break;
	}
}

void UIScene_HelpAndOptionsMenu::handlePress(F64 controlId, F64 childId)
{
	switch(static_cast<int>(controlId))
	{
	case BUTTON_HAO_WIKI:
		//ui.NavigateToScene(m_iPad, eUIScene_HowToPlayMenu);
		ShellExecute(0, 0, "https://hellishends.miraheze.org/wiki/Main_Page", 0, 0 , SW_SHOW );
		break;
	case BUTTON_HAO_SETTINGS:
		ui.NavigateToScene(m_iPad, eUIScene_SettingsOptionsMenu);
		break;
	case BUTTON_HAO_AUDIO:
		ui.NavigateToScene(m_iPad, eUIScene_SettingsAudioMenu);
		break;
	case BUTTON_HAO_GRAPHICS:
		ui.NavigateToScene(m_iPad, eUIScene_SettingsGraphicsMenu);
		break;
	case BUTTON_HAO_UI:
		ui.NavigateToScene(m_iPad, eUIScene_SettingsUIMenu);
		break;
	case BUTTON_HAO_SENSITIVITY:
		ui.NavigateToScene(m_iPad, eUIScene_SettingsControlMenu);
		break;
	case BUTTON_HAO_CONTROLS:
#ifdef _WINDOWS64
		ui.CloseAllPlayersScenes();
		m_parentLayer->removeComponent(eUIComponent_Panorama);
		m_parentLayer->removeComponent(eUIComponent_Logo);
		m_parentLayer->removeComponent(eUIComponent_MenuBackground);
		Minecraft::GetInstance()->setScreen(new ControlsScreen(nullptr, Minecraft::GetInstance()->options));
#else
		ui.NavigateToScene(m_iPad, eUIScene_ControlsMenu);
#endif
		break;
	case BUTTON_HAO_CREDITS:
		if(Minecraft::GetInstance()->level != nullptr)
		{
			ui.NavigateToScene(m_iPad, eUIScene_SkinSelectMenu);
		}
		else
		{
			ui.NavigateToScene(m_iPad, eUIScene_Credits);
		}
		break;
	}
}
