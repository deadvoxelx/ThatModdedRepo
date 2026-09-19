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
	m_buttons[BUTTON_HAO_MODS].init(IDS_MODS_MENU,BUTTON_HAO_MODS);
	
	if(app.GetLocalPlayerCount()>1)
	{
		// no credits in splitscreen
		removeControl( &m_buttons[BUTTON_HAO_CREDITS], false);
	}

	WCHAR TempString[256];	
	swprintf( (WCHAR *)TempString, 256, L"%ls: %d%%", app.GetString( IDS_SLIDER_MUSIC ),app.GetGameSettings(m_iPad,eGameSetting_MusicVolume));	
	m_sliderMusic.init(TempString,eControl_Music,0,100,app.GetGameSettings(m_iPad,eGameSetting_MusicVolume));
	
	swprintf( (WCHAR *)TempString, 256, L"%ls: %d%%", app.GetString( IDS_SLIDER_SOUND ),app.GetGameSettings(m_iPad,eGameSetting_SoundFXVolume));	
	m_sliderSound.init(TempString,eControl_Sound,0,100,app.GetGameSettings(m_iPad,eGameSetting_SoundFXVolume));

	// 4J-TomK Moved horizontal resize check to the end to prevent horizontal scaling for buttons that might get removed anyways (debug options for example)
	doHorizontalResizeCheck();
}

UIScene_HelpAndOptionsMenu::~UIScene_HelpAndOptionsMenu()
{
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
	case ACTION_MENU_LEFT:
	case ACTION_MENU_RIGHT:
		sendInputToMovie(key, repeat, pressed, released);
		break;
	}
}

void UIScene_HelpAndOptionsMenu::handleSliderMove(F64 sliderId, F64 currentValue)
{
	WCHAR TempString[256];
	int value = static_cast<int>(currentValue);
	switch(static_cast<int>(sliderId))
	{
	case eControl_Music:
		m_sliderMusic.handleSliderMove(value);
		
		app.SetGameSettings(m_iPad,eGameSetting_MusicVolume,value);	
		swprintf( (WCHAR *)TempString, 256, L"%ls: %d%%", app.GetString( IDS_SLIDER_MUSIC ),value);	
		m_sliderMusic.setLabel(TempString);

		break;
	case eControl_Sound:
		m_sliderSound.handleSliderMove(value);
		
		app.SetGameSettings(m_iPad,eGameSetting_SoundFXVolume,value);
		swprintf( (WCHAR *)TempString, 256, L"%ls: %d%%", app.GetString( IDS_SLIDER_SOUND ),value);
		m_sliderSound.setLabel(TempString);

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
		ui.NavigateToScene(m_iPad, eUIScene_KeybindsMenu);
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
	case BUTTON_HAO_MODS:
#ifdef _WINDOWS64
		ui.NavigateToScene(m_iPad, eUIScene_ModsMenu);
#endif
		break;
	}
}
