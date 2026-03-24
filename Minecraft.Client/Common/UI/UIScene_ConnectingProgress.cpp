#include "stdafx.h"
#include "UI.h"
#include "UIScene_ConnectingProgress.h"
#include "../../Minecraft.h"
#ifdef _WINDOWS64
#include "../../Windows64/Network/WinsockNetLayer.h"
#include "..\..\..\Minecraft.World\DisconnectPacket.h"

static int ConnectingProgress_OnRejectedDialogOK(LPVOID, int iPad, const C4JStorage::EMessageResult)
{
	ui.NavigateBack(iPad);
	return 0;
}
#endif

UIScene_ConnectingProgress::UIScene_ConnectingProgress(int iPad, void *_initData, UILayer *parentLayer) : UIScene(iPad, parentLayer)
{
	// Setup all the Iggy references we need for this scene
	initialiseMovie();

	parentLayer->addComponent(iPad,eUIComponent_Panorama);
	parentLayer->addComponent(iPad,eUIComponent_Logo);

	m_progressBar.showBar(false);
	m_progressBar.setVisible( false );
	m_labelTip.setVisible( false );

	ConnectionProgressParams *param = static_cast<ConnectionProgressParams *>(_initData);

	if( param->stringId >= 0 )
	{
		m_labelTitle.init( app.GetString( param->stringId ) );
	}
	else
	{		
		m_labelTitle.init( L"" );
	}
	m_progressBar.init(L"",0,0,100,0);
	m_buttonConfirm.init( app.GetString( IDS_CONFIRM_OK ), eControl_Confirm );
	m_buttonConfirm.setVisible(false);

#if 0
	if(app.GetLocalPlayerCount()>1)
	{
		app.AdjustSplitscreenScene(m_hObj,&m_OriginalPosition,m_iPad,false);
	}
#endif

	m_showTooltips = param->showTooltips;
	m_runFailTimer = param->setFailTimer;
	m_timerTime = param->timerTime;
	m_cancelFunc = param->cancelFunc;
	m_cancelFuncParam = param->cancelFuncParam;
	m_removeLocalPlayer = false;
	m_showingButton = false;

#ifdef _WINDOWS64
	WinsockNetLayer::eJoinState initState = WinsockNetLayer::GetJoinState();
	m_asyncJoinActive = (initState != WinsockNetLayer::eJoinState_Idle && initState != WinsockNetLayer::eJoinState_Cancelled);
	m_asyncJoinFailed = false;
#endif
}

UIScene_ConnectingProgress::~UIScene_ConnectingProgress()
{
	m_parentLayer->removeComponent(eUIComponent_Panorama);
	m_parentLayer->removeComponent(eUIComponent_Logo);
}

void UIScene_ConnectingProgress::updateTooltips()
{
#ifdef _WINDOWS64
	if (m_asyncJoinActive)
	{
		ui.SetTooltips( m_iPad, -1, IDS_TOOLTIPS_BACK);
		return;
	}
	if (m_asyncJoinFailed)
	{
		ui.SetTooltips( m_iPad, IDS_TOOLTIPS_SELECT, -1);
		return;
	}
#endif
	// 4J-PB - removing the option of cancel join, since it didn't work anyway
	//ui.SetTooltips( m_iPad, -1, m_showTooltips?IDS_TOOLTIPS_CANCEL_JOIN:-1);
	ui.SetTooltips( m_iPad, -1, -1);
}

void UIScene_ConnectingProgress::tick()
{
	UIScene::tick();

#ifdef _WINDOWS64
	if (m_asyncJoinActive)
	{
		WinsockNetLayer::eJoinState state = WinsockNetLayer::GetJoinState();
		if (state == WinsockNetLayer::eJoinState_Connecting)
		{
			// connecting.............
			int attempt = WinsockNetLayer::GetJoinAttempt();
			int maxAttempts = WinsockNetLayer::GetJoinMaxAttempts();
			char buf[128];
			if (attempt <= 1)
				sprintf_s(buf, "Connecting...");
			else
				sprintf_s(buf, "Connecting failed, trying again (%d/%d)", attempt, maxAttempts);
			wchar_t wbuf[128];
			mbstowcs(wbuf, buf, 128);
			m_labelTitle.setLabel(wstring(wbuf));
		}
		else if (state == WinsockNetLayer::eJoinState_Success)
		{
			m_asyncJoinActive = false;
			// go go go
		}
		else if (state == WinsockNetLayer::eJoinState_Cancelled)
		{
			// cancel
			m_asyncJoinActive = false;
			navigateBack();
		}
		else if (state == WinsockNetLayer::eJoinState_Rejected)
		{
			// server full and banned are passed differently compared to other disconnects it seems
			m_asyncJoinActive = false;
			DisconnectPacket::eDisconnectReason reason = WinsockNetLayer::GetJoinRejectReason();
			int exitReasonStringId;
			switch (reason)
			{
			case DisconnectPacket::eDisconnect_ServerFull:
				exitReasonStringId = IDS_DISCONNECTED_SERVER_FULL;
				break;
			case DisconnectPacket::eDisconnect_Banned:
				exitReasonStringId = IDS_DISCONNECTED_KICKED;
				break;
			default:
				exitReasonStringId = IDS_CONNECTION_LOST_SERVER;
				break;
			}
			UINT uiIDA[1];
			uiIDA[0] = IDS_CONFIRM_OK;
			ui.RequestErrorMessage(IDS_CONNECTION_FAILED, exitReasonStringId, uiIDA, 1, ProfileManager.GetPrimaryPad(), ConnectingProgress_OnRejectedDialogOK, nullptr, nullptr);
		}
		else if (state == WinsockNetLayer::eJoinState_Failed)
		{
			// FAIL
			m_asyncJoinActive = false;
			m_asyncJoinFailed = true;

			int maxAttempts = WinsockNetLayer::GetJoinMaxAttempts();
			char buf[256];
			sprintf_s(buf, "Failed to connect after %d attempts. The server may be unavailable.", maxAttempts);
			wchar_t wbuf[256];
			mbstowcs(wbuf, buf, 256);

			// TIL that these exist
			// not going to use a actual popup due to it requiring messing with strings which can really mess things up
			// i dont trust myself with that
			// these need to be touched up later as teh button is a bit offset
			m_labelTitle.setLabel(L"Unable to connect to server");
			m_progressBar.setLabel(wstring(wbuf));
			m_progressBar.showBar(false);
			m_progressBar.setVisible(true);
			m_buttonConfirm.setVisible(true);
			m_showingButton = true;
			m_controlTimer.setVisible(false);
		}
		return;
	}
#endif

	if( m_removeLocalPlayer )
	{
		m_removeLocalPlayer = false;
		
		Minecraft *pMinecraft = Minecraft::GetInstance();
		pMinecraft->removeLocalPlayerIdx(m_iPad);
#ifdef _XBOX_ONE
		ProfileManager.RemoveGamepadFromGame(m_iPad);
#endif
	}
}

wstring UIScene_ConnectingProgress::getMoviePath()
{
	if(app.GetLocalPlayerCount() > 1 && !m_parentLayer->IsFullscreenGroup())
	{
		return L"FullscreenProgressSplit";
	}
	else
	{
		return L"FullscreenProgress";
	}
}

void UIScene_ConnectingProgress::handleGainFocus(bool navBack)
{
	UIScene::handleGainFocus(navBack);
	if(!navBack && m_runFailTimer) addTimer(0,m_timerTime);
}

void UIScene_ConnectingProgress::handleLoseFocus()
{
	if (!m_runFailTimer) return;

	int millisecsLeft = getTimer(0)->targetTime - System::currentTimeMillis();
	int millisecsTaken = getTimer(0)->duration - millisecsLeft;
	app.DebugPrintf("\n");
	app.DebugPrintf("---------------------------------------------------------\n");
	app.DebugPrintf("---------------------------------------------------------\n");
	app.DebugPrintf("UIScene_ConnectingProgress time taken = %d millisecs\n", millisecsTaken);
	app.DebugPrintf("---------------------------------------------------------\n");
	app.DebugPrintf("---------------------------------------------------------\n");
	app.DebugPrintf("\n");


	killTimer(0);
}

void UIScene_ConnectingProgress::handleTimerComplete(int id)
{
	// Check if the connection failed
	Minecraft *pMinecraft = Minecraft::GetInstance();

	if( pMinecraft->m_connectionFailed[m_iPad] || !g_NetworkManager.IsInSession() )
	{

#if 0
		app.RemoveBackScene(m_iPad);
#endif

		int exitReasonStringId;
		switch(pMinecraft->m_connectionFailedReason[m_iPad])
		{
		case DisconnectPacket::eDisconnect_LoginTooLong:
			exitReasonStringId = IDS_DISCONNECTED_LOGIN_TOO_LONG;
			break;
		case DisconnectPacket::eDisconnect_ServerFull:
			exitReasonStringId = IDS_DISCONNECTED_SERVER_FULL;
			break;
		case DisconnectPacket::eDisconnect_Kicked:
			exitReasonStringId = IDS_DISCONNECTED_KICKED;
			break;
		case DisconnectPacket::eDisconnect_NoUGC_AllLocal:
			exitReasonStringId = IDS_NO_USER_CREATED_CONTENT_PRIVILEGE_ALL_LOCAL;
			break;
		case DisconnectPacket::eDisconnect_NoUGC_Single_Local:
			exitReasonStringId = IDS_NO_USER_CREATED_CONTENT_PRIVILEGE_SINGLE_LOCAL;
			break;
#if defined(__PS3__) || defined(__ORBIS__)
		case DisconnectPacket::eDisconnect_ContentRestricted_AllLocal:
			exitReasonStringId = IDS_CONTENT_RESTRICTION_MULTIPLAYER;
			break;
		case DisconnectPacket::eDisconnect_ContentRestricted_Single_Local:
			exitReasonStringId = IDS_CONTENT_RESTRICTION;
			break;
#endif
#ifdef _XBOX
		case DisconnectPacket::eDisconnect_NoUGC_Remote:
			exitReasonStringId = IDS_NO_USER_CREATED_CONTENT_PRIVILEGE_REMOTE;
			break;
#endif
		case DisconnectPacket::eDisconnect_NoFlying:
			exitReasonStringId = IDS_DISCONNECTED_FLYING;
			break;
		case DisconnectPacket::eDisconnect_Quitting:
			exitReasonStringId = IDS_DISCONNECTED_SERVER_QUIT;
			break;
		case DisconnectPacket::eDisconnect_OutdatedServer:
			exitReasonStringId = IDS_DISCONNECTED_SERVER_OLD;
			break;
		case DisconnectPacket::eDisconnect_OutdatedClient:
			exitReasonStringId = IDS_DISCONNECTED_CLIENT_OLD;
			break;
#if defined __ORBIS__ || defined __PS3__ || defined __PSVITA__
		case DisconnectPacket::eDisconnect_NATMismatch:
			exitReasonStringId = IDS_DISCONNECTED_NAT_TYPE_MISMATCH;
			break;
#endif
		default:
			exitReasonStringId = IDS_CONNECTION_LOST_SERVER;
			break;
		}

		if( m_iPad != ProfileManager.GetPrimaryPad() && g_NetworkManager.IsInSession() )
		{
			m_buttonConfirm.setVisible(true);
			m_showingButton = true;

			// Set text
			m_labelTitle.setLabel( app.GetString( IDS_CONNECTION_FAILED ) );
			m_progressBar.setLabel( app.GetString( exitReasonStringId ) );
			m_progressBar.setVisible( true );
			m_controlTimer.setVisible( false );
		}
		else
		{
			UINT uiIDA[1];
			uiIDA[0]=IDS_CONFIRM_OK;
			ui.RequestErrorMessage( IDS_CONNECTION_FAILED, exitReasonStringId, uiIDA,1,ProfileManager.GetPrimaryPad());
			exitReasonStringId = -1;

			//app.NavigateToHomeMenu();
			app.SetAction(ProfileManager.GetPrimaryPad(),eAppAction_ExitWorld,(void *)TRUE);
		}
	}
}

void UIScene_ConnectingProgress::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
{
	//app.DebugPrintf("UIScene_DebugOverlay handling input for pad %d, key %d, down- %s, pressed- %s, released- %s\n", iPad, key, down?"TRUE":"FALSE", pressed?"TRUE":"FALSE", released?"TRUE":"FALSE");
	
	if( m_showTooltips )
	{
		ui.AnimateKeyPress(m_iPad, key, repeat, pressed, released);

		switch(key)
		{
// 4J-PB - Removed the option to cancel join - it didn't work anyway
#ifdef _WINDOWS64
		case ACTION_MENU_CANCEL:
			if (pressed && m_asyncJoinActive)
			{
				m_asyncJoinActive = false;
				WinsockNetLayer::CancelJoinGame();
				navigateBack();
				handled = true;
			}
			break;
#endif
// 		case ACTION_MENU_CANCEL:
// 			{
// 				if(m_cancelFunc != nullptr)
// 				{
// 					m_cancelFunc(m_cancelFuncParam);
// 				}
// 				else
// 				{
// 					// Cancel the join
// 					Minecraft *pMinecraft = Minecraft::GetInstance();
// 					pMinecraft->removeLocalPlayerIdx(m_iPad);
// 				}
// 				handled = true;
// 			}
// 			break;
		case ACTION_MENU_OK:
#ifdef __ORBIS__
		case ACTION_MENU_TOUCHPAD_PRESS:
#endif
			if(pressed)
			{
				sendInputToMovie(key, repeat, pressed, released);
			}
			break;
		case ACTION_MENU_UP:
		case ACTION_MENU_DOWN:
			if(pressed)
			{
				sendInputToMovie(key, repeat, pressed, released);
			}
			break;
		}
	}
}

void UIScene_ConnectingProgress::handlePress(F64 controlId, F64 childId)
{
	switch(static_cast<int>(controlId))
	{
	case eControl_Confirm:
		if(m_showingButton)
		{
#ifdef _WINDOWS64
			if (m_asyncJoinFailed)
			{
				navigateBack();
			}
			else
#endif
			if( m_iPad != ProfileManager.GetPrimaryPad() && g_NetworkManager.IsInSession() )
			{
				// The connection failed if we see the button, so the temp player should be removed and the viewports updated again
				// This is actually done in the tick as we can't pull down the scene we are currently in from here
				m_removeLocalPlayer = true;
			}
			else
			{
				ui.NavigateToHomeMenu();
				//app.NavigateBack( ProfileManager.GetPrimaryPad() );
			}
		}
		break;
	}
}
