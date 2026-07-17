#include "stdafx.h"
#include "UI.h"
#include "UIScene_SettingsMenu.h"
#include "..\..\Minecraft.h"

// Voxel - this ui scene is basically dead; may repurpose it later

UIScene_SettingsMenu::UIScene_SettingsMenu(int iPad, void *initData, UILayer *parentLayer) : UIScene(iPad, parentLayer)
{
	// Setup all the Iggy references we need for this scene
	initialiseMovie();
	
	bool bNotInGame=(Minecraft::GetInstance()->level==nullptr);

	m_buttons[BUTTON_ALL_OPTIONS].init(IDS_OPTIONS,BUTTON_ALL_OPTIONS);
	m_buttons[BUTTON_ALL_AUDIO].init(IDS_AUDIO,BUTTON_ALL_AUDIO);
	m_buttons[BUTTON_ALL_CONTROL].init(IDS_SENSITIVITY,BUTTON_ALL_CONTROL);
	m_buttons[BUTTON_ALL_GRAPHICS].init(IDS_GRAPHICS,BUTTON_ALL_GRAPHICS);
	m_buttons[BUTTON_ALL_UI].init(IDS_USER_INTERFACE,BUTTON_ALL_UI);
	m_buttons[BUTTON_ALL_RESETTODEFAULTS].init(IDS_RESET_TO_DEFAULTS,BUTTON_ALL_RESETTODEFAULTS);

	if(ProfileManager.GetPrimaryPad()!=m_iPad)
	{
		removeControl( &m_buttons[BUTTON_ALL_AUDIO], bNotInGame);
		removeControl( &m_buttons[BUTTON_ALL_GRAPHICS], bNotInGame);
	}

	doHorizontalResizeCheck();
}

UIScene_SettingsMenu::~UIScene_SettingsMenu()
{
}

wstring UIScene_SettingsMenu::getMoviePath()
{
	if(app.GetLocalPlayerCount() > 1)
	{
		return L"SettingsMenuSplit";
	}
	else
	{
		return L"SettingsMenu";
	}
}

void UIScene_SettingsMenu::handleReload()
{
	doHorizontalResizeCheck();
}

void UIScene_SettingsMenu::updateTooltips()
{
	ui.SetTooltips( m_iPad, IDS_TOOLTIPS_SELECT,IDS_TOOLTIPS_BACK);
}

void UIScene_SettingsMenu::updateComponents()
{
}

void UIScene_SettingsMenu::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
{
	ui.AnimateKeyPress(m_iPad, key, repeat, pressed, released);

	switch(key)
	{
	case ACTION_MENU_CANCEL:
		if(pressed)
		{
			app.CheckGameSettingsChanged(true,iPad);          
			navigateBack();
		}
		break;
	case ACTION_MENU_OK:
		sendInputToMovie(key, repeat, pressed, released);
		break;
	case ACTION_MENU_UP:
	case ACTION_MENU_DOWN:
		sendInputToMovie(key, repeat, pressed, released);
		break;
	}
}

void UIScene_SettingsMenu::handlePress(F64 controlId, F64 childId)
{
	ui.PlayUISFX(eSFX_Press);

	switch(static_cast<int>(controlId))
	{
	case BUTTON_ALL_OPTIONS:
		ui.NavigateToScene(m_iPad, eUIScene_SettingsOptionsMenu);
		break;
	case BUTTON_ALL_AUDIO:
		ui.NavigateToScene(m_iPad, eUIScene_SettingsAudioMenu);
		break;
	case BUTTON_ALL_CONTROL:
		ui.NavigateToScene(m_iPad, eUIScene_SettingsControlMenu);
		break;
	case BUTTON_ALL_GRAPHICS:
		ui.NavigateToScene(m_iPad, eUIScene_SettingsGraphicsMenu);
		break;
	case BUTTON_ALL_UI:
		ui.NavigateToScene(m_iPad, eUIScene_SettingsUIMenu);
		break;
	case BUTTON_ALL_RESETTODEFAULTS:
		{
			UINT uiIDA[2];
			uiIDA[0]=IDS_CONFIRM_CANCEL;
			uiIDA[1]=IDS_CONFIRM_OK;

			ui.RequestAlertMessage(IDS_DEFAULTS_TITLE, IDS_DEFAULTS_TEXT, uiIDA, 2, m_iPad,&UIScene_SettingsMenu::ResetDefaultsDialogReturned,this);
		}	
		break;
	}
}

int UIScene_SettingsMenu::ResetDefaultsDialogReturned(void *pParam,int iPad,C4JStorage::EMessageResult result)
{
	UIScene_SettingsMenu* pClass = static_cast<UIScene_SettingsMenu *>(pParam);

	if(result==C4JStorage::EMessage_ResultDecline) 
	{
#if (defined __PS3__ || defined __ORBIS__ || defined _DURANGO || defined __PSVITA__)
		app.SetDefaultOptions(StorageManager.GetDashboardProfileSettings(pClass->m_iPad),pClass->m_iPad);
#else
		app.SetDefaultOptions(ProfileManager.GetDashboardProfileSettings(pClass->m_iPad),pClass->m_iPad);
#endif
		app.CheckGameSettingsChanged(true,iPad);
	}
	return 0;
}
