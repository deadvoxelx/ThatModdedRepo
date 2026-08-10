#include "stdafx.h"
#include "UI.h"
#include "UIScene_LoadMenu.h"
#include "..\..\Minecraft.h"
#include "..\..\User.h"
#include "..\..\TexturePackRepository.h"
#include "..\..\Options.h"
#include "..\..\MinecraftServer.h"
#include "..\..\..\Minecraft.World\LevelSettings.h"
#include "..\..\..\Minecraft.World\StringHelpers.h"
#include "..\..\..\Minecraft.World\ConsoleSaveFileOriginal.h"
#if defined(__PS3__) || defined(__ORBIS__) || defined(__PSVITA__)
#include "Common\Network\Sony\SonyHttp.h"
#endif
#include "..\..\DLCTexturePack.h"
#if defined(__ORBIS__) || defined(__PSVITA__)
#include <ces.h>
#endif

#define GAME_CREATE_ONLINE_TIMER_ID 0
#define GAME_CREATE_ONLINE_TIMER_TIME 100
#ifdef _XBOX
#define CHECKFORAVAILABLETEXTUREPACKS_TIMER_ID 1
#define CHECKFORAVAILABLETEXTUREPACKS_TIMER_TIME 50
#endif

int UIScene_LoadMenu::m_iDifficultyTitleSettingA[4]=
{
	IDS_DIFFICULTY_TITLE_PEACEFUL,
	IDS_DIFFICULTY_TITLE_EASY,
	IDS_DIFFICULTY_TITLE_NORMAL,
	IDS_DIFFICULTY_TITLE_HARD
};

int UIScene_LoadMenu::LoadSaveDataThumbnailReturned(LPVOID lpParam,PBYTE pbThumbnail,DWORD dwThumbnailBytes)
{
	UIScene_LoadMenu *pClass= static_cast<UIScene_LoadMenu *>(ui.GetSceneFromCallbackId((size_t)lpParam));

	if(pClass)
	{
		app.DebugPrintf("Received data for a thumbnail\n");

		if(pbThumbnail && dwThumbnailBytes)
		{
			pClass->registerSubstitutionTexture(pClass->m_thumbnailName,pbThumbnail,dwThumbnailBytes);

			pClass->m_pbThumbnailData = pbThumbnail;
			pClass->m_uiThumbnailSize = dwThumbnailBytes;
			pClass->m_bSaveThumbnailReady = true;
		}
		else
		{
			app.DebugPrintf("Thumbnail data is nullptr, or has size 0\n");
			pClass->m_bThumbnailGetFailed = true;
		}
		pClass->m_bRetrievingSaveThumbnail = false;
	}

	return 0;
}

UIScene_LoadMenu::UIScene_LoadMenu(int iPad, void *initData, UILayer *parentLayer) : IUIScene_StartGame(iPad, parentLayer)
{
	initialiseMovie();

	LoadMenuInitData *params = static_cast<LoadMenuInitData *>(initData);

	m_labelGameName.init(app.GetString(IDS_WORLD_NAME));
	m_labelSeed.init(L"");
	m_labelCreatedMode.init(app.GetString(IDS_CREATED_IN_SURVIVAL));

	m_buttonGamemode.init(app.GetString(IDS_GAMEMODE_SURVIVAL),eControl_GameMode);
	m_buttonMoreOptions.init(app.GetString(IDS_MORE_OPTIONS),eControl_MoreOptions);
	m_buttonLoadWorld.init(app.GetString(IDS_LOAD),eControl_LoadWorld);
	m_texturePackList.init(app.GetString(IDS_DLC_MENU_TEXTUREPACKS), eControl_TexturePackList);

	m_labelTexturePackName.init(L"");
	m_labelTexturePackDescription.init(L"");

	m_CurrentDifficulty=app.GetGameSettings(m_iPad,eGameSetting_Difficulty);
	WCHAR TempString[256];
	swprintf( (WCHAR *)TempString, 256, L"%ls: %ls", app.GetString( IDS_SLIDER_DIFFICULTY ),app.GetString(m_iDifficultyTitleSettingA[app.GetGameSettings(m_iPad,eGameSetting_Difficulty)]));	
	m_sliderDifficulty.init(TempString,eControl_Difficulty,0,3,app.GetGameSettings(m_iPad,eGameSetting_Difficulty));

	m_MoreOptionsParams.bGenerateOptions=FALSE;
	m_MoreOptionsParams.bPVP = TRUE;
	m_MoreOptionsParams.bTrust = TRUE;
	m_MoreOptionsParams.bFireSpreads = TRUE;
	m_MoreOptionsParams.bHostPrivileges = FALSE;
	m_MoreOptionsParams.bTNT = TRUE;
	m_MoreOptionsParams.iPad = iPad;

	m_iSaveGameInfoIndex=params->iSaveGameInfoIndex;
	m_levelGen = params->levelGen;

	m_bGameModeCreative = false;
	m_iGameModeId = GameType::SURVIVAL->getId();
	m_bHasBeenInCreative = false;
	m_bIsSaveOwner = true;

	m_bSaveThumbnailReady = false;
	m_bRetrievingSaveThumbnail = true;
	m_bShowTimer = false;
	m_pDLCPack = nullptr;
	m_bAvailableTexturePacksChecked=false;
	m_bRequestQuadrantSignin = false;
	m_iTexturePacksNotInstalled=0;
	m_bRebuildTouchBoxes = false;
	m_bThumbnailGetFailed = false;
	m_seed = 0;
	m_bIsCorrupt = false;
	m_pbThumbnailData = nullptr;
	m_uiThumbnailSize = 0;

	m_bMultiplayerAllowed = ProfileManager.IsSignedInLive( m_iPad ) && ProfileManager.AllowedToPlayMultiplayer(m_iPad);
	bool bGameSetting_Online=(app.GetGameSettings(m_iPad,eGameSetting_Online)!=0);
	m_MoreOptionsParams.bOnlineSettingChangedBySystem=false;

	if( m_bMultiplayerAllowed)
	{
		m_MoreOptionsParams.bOnlineGame = bGameSetting_Online?TRUE:FALSE;
		if(bGameSetting_Online)
		{
			m_MoreOptionsParams.bInviteOnly = (app.GetGameSettings(m_iPad,eGameSetting_InviteOnly)!=0)?TRUE:FALSE;
			m_MoreOptionsParams.bAllowFriendsOfFriends = (app.GetGameSettings(m_iPad,eGameSetting_FriendsOfFriends)!=0)?TRUE:FALSE;
		}
		else
		{
			m_MoreOptionsParams.bInviteOnly = FALSE;
			m_MoreOptionsParams.bAllowFriendsOfFriends = FALSE;
		}
	}
	else
	{
		m_MoreOptionsParams.bOnlineGame = FALSE;
		m_MoreOptionsParams.bInviteOnly = FALSE;
		m_MoreOptionsParams.bAllowFriendsOfFriends = FALSE;
		if(bGameSetting_Online)
		{
			m_MoreOptionsParams.bOnlineSettingChangedBySystem=true;
		}	
	}

	bool bOnlineGame = m_MoreOptionsParams.bOnlineGame;
	m_checkboxOnline.SetEnable(true);

	if(ProfileManager.IsSignedInLive(m_iPad) == false)
	{
		m_checkboxOnline.SetEnable(false);
	}

	if(m_MoreOptionsParams.bOnlineSettingChangedBySystem)
	{
		m_checkboxOnline.SetEnable(false);
		bOnlineGame = false;
	}

	m_checkboxOnline.init(app.GetString(IDS_ONLINE_GAME), eControl_OnlineGame, bOnlineGame);

	if(m_levelGen)
	{
		m_labelGameName.init(m_levelGen->getDisplayName());
		if(m_levelGen->requiresTexturePack())
		{
			m_MoreOptionsParams.dwTexturePack = m_levelGen->getRequiredTexturePackId();

			m_texturePackList.setEnabled(false);


			TexturePack *tp = Minecraft::GetInstance()->skins->getTexturePackById(m_MoreOptionsParams.dwTexturePack);
			DWORD dwImageBytes;
			PBYTE pbImageData = tp->getPackIcon(dwImageBytes);

			if(dwImageBytes > 0 && pbImageData)
			{
				wchar_t textureName[64];
				swprintf(textureName,64,L"loadsave");				
				registerSubstitutionTexture(textureName,pbImageData,dwImageBytes);
				m_bitmapIcon.setTextureName( textureName );
			}
		}
		m_bHasBeenInCreative = m_levelGen->getLevelHasBeenInCreative();
		if(m_bHasBeenInCreative)
		{
			m_labelCreatedMode.setLabel( app.GetString(IDS_CREATED_IN_CREATIVE) );
		}
		else
		{
			m_labelCreatedMode.setLabel( app.GetString(IDS_CREATED_IN_SURVIVAL) );
		}
	}
	else
	{

#if defined(__PS3__) || defined(__ORBIS__)|| defined(_DURANGO) || defined (__PSVITA__)
		uint16_t u16Message[MAX_SAVEFILENAME_LENGTH];
		size_t srclen,dstlen;
		srclen=MAX_SAVEFILENAME_LENGTH;
		dstlen=MAX_SAVEFILENAME_LENGTH;
#ifdef __PS3__
		L10nResult lres= UTF8stoUTF16s((uint8_t *)params->saveDetails->UTF8SaveFilename,&srclen,u16Message,&dstlen);
#elif defined(_DURANGO) 
		memcpy(u16Message,params->saveDetails->UTF16SaveFilename, MAX_SAVEFILENAME_LENGTH);
#else
		{
			SceCesUcsContext Context;
			sceCesUcsContextInit( &Context );
			uint32_t utf8Len, utf16Len;
			sceCesUtf8StrToUtf16Str(&Context, (uint8_t *)params->saveDetails->UTF8SaveFilename, srclen, &utf8Len, u16Message, dstlen, &utf16Len);
		}
#endif
		m_thumbnailName = (wchar_t *)u16Message;
		if(params->saveDetails->pbThumbnailData)
		{
			m_pbThumbnailData = params->saveDetails->pbThumbnailData;
			m_uiThumbnailSize = params->saveDetails->dwThumbnailSize;
			m_bSaveThumbnailReady = true;
		}
		else
		{
			app.DebugPrintf("Requesting the save thumbnail\n");
			PSAVE_DETAILS pSaveDetails=StorageManager.ReturnSavesInfo();
#ifdef _DURANGO
			C4JStorage::ESaveGameState eLoadStatus=StorageManager.LoadSaveDataThumbnail(&pSaveDetails->SaveInfoA[(int)m_iSaveGameInfoIndex],&LoadSaveDataThumbnailReturned,(LPVOID)GetCallbackUniqueId(),true);
#else
			C4JStorage::ESaveGameState eLoadStatus=StorageManager.LoadSaveDataThumbnail(&pSaveDetails->SaveInfoA[(int)m_iSaveGameInfoIndex],&LoadSaveDataThumbnailReturned,(LPVOID)GetCallbackUniqueId());
#endif
			m_bShowTimer = true;
		}
#if defined(_DURANGO)
		m_labelGameName.init(params->saveDetails->UTF16SaveName);
#else
        wchar_t wSaveName[128];
        ZeroMemory(wSaveName, 128 * sizeof(wchar_t) );
        mbstowcs(wSaveName, params->saveDetails->UTF8SaveName, strlen(params->saveDetails->UTF8SaveName)+1); // plus null
		m_labelGameName.init(wSaveName);
#endif
#endif
#ifdef _WINDOWS64
		if (params->saveDetails != nullptr)
		{
			if (params->saveDetails->UTF8SaveName[0] != '\0')
			{
				wchar_t wSaveName[128];
				ZeroMemory(wSaveName, sizeof(wSaveName));
				mbstowcs(wSaveName, params->saveDetails->UTF8SaveName, 127);
				m_levelName = wstring(wSaveName);
				m_labelGameName.init(m_levelName);
			}

			wchar_t wFilename[MAX_SAVEFILENAME_LENGTH];
			ZeroMemory(wFilename, sizeof(wFilename));
			mbstowcs(wFilename, params->saveDetails->UTF8SaveFilename, MAX_SAVEFILENAME_LENGTH - 1);
			m_thumbnailName = wFilename;

			if (params->saveDetails->pbThumbnailData && params->saveDetails->dwThumbnailSize > 0)
			{
				registerSubstitutionTexture(wFilename, params->saveDetails->pbThumbnailData, params->saveDetails->dwThumbnailSize);
				m_bitmapIcon.setTextureName(wFilename);
				m_pbThumbnailData = params->saveDetails->pbThumbnailData;
				m_uiThumbnailSize = params->saveDetails->dwThumbnailSize;

				applyThumbnailMetadata();
			}
			m_bRetrievingSaveThumbnail = false;
		}
#endif
	}
	
	TelemetryManager->RecordMenuShown(m_iPad, eUIScene_LoadMenu, 0);
	m_iTexturePacksNotInstalled=0;

	if(app.StartInstallDLCProcess(m_iPad)==true)
	{
		m_bIgnoreInput=true;
	}
	else
	{
		m_bIgnoreInput = false;

		Minecraft *pMinecraft = Minecraft::GetInstance();
		unsigned int texturePacksCount = (unsigned int)pMinecraft->skins->getTexturePackCount();
		for(unsigned int i = 0; i < texturePacksCount; ++i)
		{
			TexturePack *tp = pMinecraft->skins->getTexturePackByIndex(i);

			DWORD dwImageBytes;
			PBYTE pbImageData = tp->getPackIcon(dwImageBytes);

			if(dwImageBytes > 0 && pbImageData)
			{
				wchar_t imageName[64];
				swprintf(imageName,64,L"tpack%08x",tp->getId());
				registerSubstitutionTexture(imageName, pbImageData, dwImageBytes);
				m_texturePackList.addPack(i,imageName);
			}
		}
		m_currentTexturePackIndex = pMinecraft->skins->getTexturePackIndex(m_MoreOptionsParams.dwTexturePack);
		UpdateTexturePackDescription(m_currentTexturePackIndex);
		m_texturePackList.selectSlot(m_currentTexturePackIndex);

#ifdef _XBOX

#if defined(__PS3__) || defined(__ORBIS__)
		if(!m_bAvailableTexturePacksChecked && app.GetCommerceProductListRetrieved()&& app.GetCommerceProductListInfoRetrieved())
#else
		if(!m_bAvailableTexturePacksChecked)
#endif
		{		
			DLC_INFO *pDLCInfo=nullptr;

			bool bTexturePackAlreadyListed;
			bool bNeedToGetTPD=false;

			for(unsigned int i = 0; i < app.GetDLCInfoTexturesOffersCount(); ++i)
			{
				bTexturePackAlreadyListed=false;
#if defined(__PS3__) || defined(__ORBIS__)
				char *pchName=app.GetDLCInfoTextures(i);
				pDLCInfo=app.GetDLCInfo(pchName);
#else
				ULONGLONG ull=app.GetDLCInfoTexturesFullOffer(i);
				pDLCInfo=app.GetDLCInfoForFullOfferID(ull);
#endif

				for(unsigned int i = 0; i < texturePacksCount; ++i)
				{
					TexturePack *tp = pMinecraft->skins->getTexturePackByIndex(i);
					if(pDLCInfo && pDLCInfo->iConfig==tp->getDLCParentPackId())
					{
						bTexturePackAlreadyListed=true;
					}
				}
				if(bTexturePackAlreadyListed==false)
				{
					bNeedToGetTPD=true;

					m_iTexturePacksNotInstalled++;
				}
			}

			if(bNeedToGetTPD==true)
			{
				app.DebugPrintf("+++ Adding TMSPP request for texture pack data\n");
				app.AddTMSPPFileTypeRequest(e_DLC_TexturePackData);
				m_iConfigA= new int [m_iTexturePacksNotInstalled];
				m_iTexturePacksNotInstalled=0;

				for(unsigned int i = 0; i < app.GetDLCInfoTexturesOffersCount(); ++i)
				{
					bTexturePackAlreadyListed=false;
#if defined(__PS3__) || defined(__ORBIS__)
					char *pchName=app.GetDLCInfoTextures(i);
					pDLCInfo=app.GetDLCInfo(pchName);
#else
					ULONGLONG ull=app.GetDLCInfoTexturesFullOffer(i);
					pDLCInfo=app.GetDLCInfoForFullOfferID(ull);
#endif

					if(pDLCInfo)
					{
						for(unsigned int i = 0; i < texturePacksCount; ++i)
						{
							TexturePack *tp = pMinecraft->skins->getTexturePackByIndex(i);
							if(pDLCInfo && pDLCInfo->iConfig==tp->getDLCParentPackId())
							{
								bTexturePackAlreadyListed=true;
							}
						}
						if(bTexturePackAlreadyListed==false)
						{
							m_iConfigA[m_iTexturePacksNotInstalled++]=pDLCInfo->iConfig;
						}
					}
				}
			}
		}
#endif
	}

#ifdef _XBOX
	addTimer(CHECKFORAVAILABLETEXTUREPACKS_TIMER_ID,CHECKFORAVAILABLETEXTUREPACKS_TIMER_TIME);
#endif

	if(params) delete params;
	addTimer(GAME_CREATE_ONLINE_TIMER_ID,GAME_CREATE_ONLINE_TIMER_TIME);
}

void UIScene_LoadMenu::applyThumbnailMetadata()
{
	bool bHostOptionsRead = false;
	unsigned int uiHostOptions = 0;

	char szSeed[50];
	ZeroMemory(szSeed,50);
	app.GetImageTextData(m_pbThumbnailData,m_uiThumbnailSize,(unsigned char *)&szSeed,uiHostOptions,bHostOptionsRead,m_MoreOptionsParams.dwTexturePack);

#if defined(_XBOX_ONE) || defined(__ORBIS__) || defined(_WINDOWS64)
	sscanf_s(szSeed, "%I64d", &m_seed);
#endif

	if(szSeed[0]!=0)
	{
		WCHAR TempString[256];
		swprintf( (WCHAR *)TempString, 256, L"%ls: %hs", app.GetString( IDS_SEED ),szSeed);	
		m_labelSeed.setLabel(TempString);
	}
	else
	{
		m_labelSeed.setLabel(L"");
	}

	if(bHostOptionsRead)
	{
		m_MoreOptionsParams.bPVP = app.GetGameHostOption(uiHostOptions,eGameHostOption_PvP)>0?TRUE:FALSE;
		m_MoreOptionsParams.bTrust = app.GetGameHostOption(uiHostOptions,eGameHostOption_TrustPlayers)>0?TRUE:FALSE;
		m_MoreOptionsParams.bFireSpreads = app.GetGameHostOption(uiHostOptions,eGameHostOption_FireSpreads)>0?TRUE:FALSE;
		m_MoreOptionsParams.bTNT = app.GetGameHostOption(uiHostOptions,eGameHostOption_TNT)>0?TRUE:FALSE;
		m_MoreOptionsParams.bHostPrivileges = app.GetGameHostOption(uiHostOptions,eGameHostOption_CheatsEnabled)>0?TRUE:FALSE;
		m_MoreOptionsParams.bDisableSaving = app.GetGameHostOption(uiHostOptions,eGameHostOption_DisableSaving)>0?TRUE:FALSE;
		m_MoreOptionsParams.currentWorldSize = static_cast<EGameHostOptionWorldSize>(app.GetGameHostOption(uiHostOptions, eGameHostOption_WorldSize));
		m_MoreOptionsParams.newWorldSize = m_MoreOptionsParams.currentWorldSize;

		m_MoreOptionsParams.bMobGriefing = app.GetGameHostOption(uiHostOptions, eGameHostOption_MobGriefing);
		m_MoreOptionsParams.bKeepInventory = app.GetGameHostOption(uiHostOptions, eGameHostOption_KeepInventory);
		m_MoreOptionsParams.bDoMobSpawning = app.GetGameHostOption(uiHostOptions, eGameHostOption_DoMobSpawning);
		m_MoreOptionsParams.bDoMobLoot = app.GetGameHostOption(uiHostOptions, eGameHostOption_DoMobLoot);
		m_MoreOptionsParams.bDoTileDrops = app.GetGameHostOption(uiHostOptions, eGameHostOption_DoTileDrops);
		m_MoreOptionsParams.bNaturalRegeneration = app.GetGameHostOption(uiHostOptions, eGameHostOption_NaturalRegeneration);
		m_MoreOptionsParams.bDoDaylightCycle = app.GetGameHostOption(uiHostOptions, eGameHostOption_DoDaylightCycle);

		bool cheatsOn = m_MoreOptionsParams.bHostPrivileges;
		if (!cheatsOn)
		{
			m_MoreOptionsParams.bMobGriefing = true;
			m_MoreOptionsParams.bKeepInventory = false;
			m_MoreOptionsParams.bDoMobSpawning = true;
			m_MoreOptionsParams.bDoDaylightCycle = true;
		}

		if(app.GetGameHostOption(uiHostOptions,eGameHostOption_WasntSaveOwner)>0)
		{
			m_bIsSaveOwner = false;
		}

		m_bHasBeenInCreative = app.GetGameHostOption(uiHostOptions,eGameHostOption_HasBeenInCreative)>0;
		if(app.GetGameHostOption(uiHostOptions,eGameHostOption_HasBeenInCreative)>0)
		{
			m_labelCreatedMode.setLabel( app.GetString(IDS_CREATED_IN_CREATIVE) );
		}
		else
		{
			m_labelCreatedMode.setLabel( app.GetString(IDS_CREATED_IN_SURVIVAL) );
		}

		switch(app.GetGameHostOption(uiHostOptions,eGameHostOption_GameType))
		{
		case 1: // Creative
			m_buttonGamemode.setLabel(app.GetString(IDS_GAMEMODE_CREATIVE));
			m_bGameModeCreative=true;
			m_iGameModeId = GameType::CREATIVE->getId();
			break;
#ifdef _ADVENTURE_MODE_ENABLED
		case 2: // Adventure
			m_buttonGamemode.setLabel(app.GetString(IDS_GAMEMODE_ADVENTURE));
			m_bGameModeCreative=false;
			m_iGameModeId = GameType::ADVENTURE->getId();
			break;
#endif
		case 0: // Survival
		default:
			m_buttonGamemode.setLabel(app.GetString(IDS_GAMEMODE_SURVIVAL));
			m_bGameModeCreative=false;
			m_iGameModeId = GameType::SURVIVAL->getId();
			break;
		};

		bool bGameSetting_Online=(app.GetGameSettings(m_iPad,eGameSetting_Online)!=0);
		if(app.GetGameHostOption(uiHostOptions,eGameHostOption_FriendsOfFriends) && !(m_bMultiplayerAllowed && bGameSetting_Online))
		{
			m_MoreOptionsParams.bAllowFriendsOfFriends = TRUE;
		}
	}
}

void UIScene_LoadMenu::updateTooltips()
{
	ui.SetTooltips( DEFAULT_XUI_MENU_USER, IDS_TOOLTIPS_SELECT,IDS_TOOLTIPS_BACK, -1, -1);
}

void UIScene_LoadMenu::updateComponents()
{
	m_parentLayer->showComponent(m_iPad,eUIComponent_Panorama,true);
	m_parentLayer->showComponent(m_iPad,eUIComponent_Logo,false);
}

wstring UIScene_LoadMenu::getMoviePath()
{
	return L"LoadMenu";
}

UIControl* UIScene_LoadMenu::GetMainPanel()
{
	return &m_controlMainPanel;
}

void UIScene_LoadMenu::tick()
{
	if(m_bShowTimer)
	{
		m_bShowTimer = false;
		ui.NavigateToScene(m_iPad, eUIScene_Timer);
	}

	if( m_bThumbnailGetFailed )
	{
		ui.NavigateBack(m_iPad, false, eUIScene_LoadOrJoinMenu);
		return;
	}

	if( m_bSaveThumbnailReady )
	{
		m_bSaveThumbnailReady = false;

		m_bitmapIcon.setTextureName( m_thumbnailName.c_str() );

		applyThumbnailMetadata();

		Minecraft *pMinecraft = Minecraft::GetInstance();
		m_currentTexturePackIndex = pMinecraft->skins->getTexturePackIndex(m_MoreOptionsParams.dwTexturePack);

		UpdateTexturePackDescription(m_currentTexturePackIndex);

		m_texturePackList.selectSlot(m_currentTexturePackIndex);

		ui.NavigateBack(m_iPad, false, getSceneType() );
	}

	if(m_iSetTexturePackDescription >= 0 )
	{
		UpdateTexturePackDescription( m_iSetTexturePackDescription );
		m_iSetTexturePackDescription = -1;
	}
	if(m_bShowTexturePackDescription)
	{
		slideLeft();
		m_texturePackDescDisplayed = true;

		m_bShowTexturePackDescription = false;
	}

	if(m_bRequestQuadrantSignin)
	{
		m_bRequestQuadrantSignin = false;
		SignInInfo info;
		info.Func = &UIScene_LoadMenu::StartGame_SignInReturned;
		info.lpParam = this;
		info.requireOnline = m_MoreOptionsParams.bOnlineGame;
		ui.NavigateToScene(ProfileManager.GetPrimaryPad(),eUIScene_QuadrantSignin,&info);
	}

#ifdef __ORBIS__
	switch (sceNpCommerceDialogUpdateStatus())
	{
	case SCE_COMMON_DIALOG_STATUS_FINISHED:
		{
			SceNpCommerceDialogResult Result;
			sceNpCommerceDialogGetResult(&Result);
			sceNpCommerceDialogTerminate();

			if(Result.authorized)
			{
				ProfileManager.PsPlusUpdate(ProfileManager.GetPrimaryPad(), &Result);
				LoadDataComplete(this);
			}
			else
			{
				UINT uiIDA[1];
				uiIDA[0]=IDS_PRO_NOTONLINE_DECLINE;

				ui.RequestAlertMessage(IDS_PLAY_OFFLINE,IDS_NO_PLAYSTATIONPLUS, uiIDA, 1, ProfileManager.GetPrimaryPad(),&UIScene_LoadMenu::ContinueOffline,this);
			}
		}
		break;
	default:
		break;
	}
#endif

	UIScene::tick();
}

#ifdef __ORBIS__
int UIScene_LoadMenu::ContinueOffline(void *pParam,int iPad,C4JStorage::EMessageResult result)
{
	UIScene_LoadMenu* pClass = (UIScene_LoadMenu*)pParam;

	if(result==C4JStorage::EMessage_ResultAccept) 
	{
		pClass->m_MoreOptionsParams.bOnlineGame=false;
		pClass->LoadDataComplete(pClass);
	}
	return 0;
}

#endif

void UIScene_LoadMenu::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
{
	if(m_bIgnoreInput) return;

	ui.AnimateKeyPress(m_iPad, key, repeat, pressed, released);

	switch(key)
	{
	case ACTION_MENU_CANCEL:
		if(pressed)
		{
			app.SetCorruptSaveDeleted(false);
			navigateBack();
			handled = true;
		}
		break;
	case ACTION_MENU_OK:
#ifdef __ORBIS__
	case ACTION_MENU_TOUCHPAD_PRESS:
#endif
		
#if defined _XBOX_ONE
		if ( pressed && controlHasFocus(m_checkboxOnline.getId()) && !m_checkboxOnline.IsEnabled() )
		{
			UINT uiIDA[1] = { IDS_CONFIRM_OK };
			ui.RequestErrorMessage(IDS_PRO_NOTONLINE_TITLE, IDS_PRO_XBOXLIVE_NOTIFICATION, uiIDA, 1, iPad); 
		}
#endif

	case ACTION_MENU_UP:
	case ACTION_MENU_DOWN:
	case ACTION_MENU_LEFT:
	case ACTION_MENU_RIGHT:
	case ACTION_MENU_OTHER_STICK_UP:
	case ACTION_MENU_OTHER_STICK_DOWN:
		sendInputToMovie(key, repeat, pressed, released);

		bool bOnlineGame = m_checkboxOnline.IsChecked();
		if (m_MoreOptionsParams.bOnlineGame != bOnlineGame)
		{
			m_MoreOptionsParams.bOnlineGame = bOnlineGame;

			if (!m_MoreOptionsParams.bOnlineGame)
			{
				m_MoreOptionsParams.bInviteOnly = false;
				m_MoreOptionsParams.bAllowFriendsOfFriends = false;
			}
		}

		handled = true;
		break;
	}
}

void UIScene_LoadMenu::handlePress(F64 controlId, F64 childId)
{
	if(m_bIgnoreInput) return;

	switch(static_cast<int>(controlId))
	{
	case eControl_GameMode:
		switch(m_iGameModeId)
		{
		case 0: // Survival
			m_buttonGamemode.setLabel(app.GetString(IDS_GAMEMODE_CREATIVE));
			m_iGameModeId = GameType::CREATIVE->getId();
			m_bGameModeCreative = true;
			break;
		case 1: // Creative
#ifdef _ADVENTURE_MODE_ENABLED
			m_buttonGamemode.setLabel(app.GetString(IDS_GAMEMODE_ADVENTURE));
			m_iGameModeId = GameType::ADVENTURE->getId();
			m_bGameModeCreative = false;
			break;
		case 2: // Adventure
#endif
			m_buttonGamemode.setLabel(app.GetString(IDS_GAMEMODE_SURVIVAL));
			m_iGameModeId = GameType::SURVIVAL->getId();
			m_bGameModeCreative = false;
			break;
		};
		break;
	case eControl_MoreOptions:
		ui.NavigateToScene(m_iPad, eUIScene_LaunchMoreOptionsMenu, &m_MoreOptionsParams);
		break;
	case eControl_TexturePackList:
		{
			ui.PlayUISFX(eSFX_Press);
			UpdateCurrentTexturePack(static_cast<int>(childId));
		}
		break;
	case eControl_LoadWorld:
		{
			ui.PlayUISFX(eSFX_Press);
#ifdef _DURANGO
			if(m_MoreOptionsParams.bOnlineGame)
			{
				m_bIgnoreInput = true;
				ProfileManager.CheckMultiplayerPrivileges(m_iPad, true, &checkPrivilegeCallback, this);
			}
			else
#endif
			{
				StartSharedLaunchFlow();
			}
		}
		break;
	};
}

#ifdef _DURANGO
void UIScene_LoadMenu::checkPrivilegeCallback(LPVOID lpParam, bool hasPrivilege, int iPad)
{
	UIScene_LoadMenu* pClass = (UIScene_LoadMenu*)lpParam;

	if(hasPrivilege)
	{
		pClass->StartSharedLaunchFlow();
	}
	else
	{
		pClass->m_bIgnoreInput = false;
	}
}
#endif

void UIScene_LoadMenu::StartSharedLaunchFlow()
{
	Minecraft *pMinecraft=Minecraft::GetInstance();
	if(m_MoreOptionsParams.dwTexturePack!=0)
	{
		TexturePack *pTexturePack = pMinecraft->skins->getTexturePackById(m_MoreOptionsParams.dwTexturePack);

		if(pTexturePack==nullptr)
		{
			UINT uiIDA[2];

			uiIDA[0]=IDS_TEXTUREPACK_FULLVERSION;
			uiIDA[1]=IDS_CONFIRM_CANCEL;

			ui.RequestAlertMessage(IDS_DLC_TEXTUREPACK_NOT_PRESENT_TITLE, IDS_DLC_TEXTUREPACK_NOT_PRESENT, uiIDA, 2, ProfileManager.GetPrimaryPad(),&TexturePackDialogReturned,this);
			return;
		}
	}
	m_bIgnoreInput = true;

	if(!m_MoreOptionsParams.bOnlineSettingChangedBySystem)
	{
		app.SetGameSettings(m_iPad,eGameSetting_Online,m_MoreOptionsParams.bOnlineGame?1:0);
	}
	app.SetGameSettings(m_iPad,eGameSetting_InviteOnly,m_MoreOptionsParams.bInviteOnly?1:0);
	app.SetGameSettings(m_iPad,eGameSetting_FriendsOfFriends,m_MoreOptionsParams.bAllowFriendsOfFriends?1:0);

	app.CheckGameSettingsChanged(true,m_iPad);

	if(m_MoreOptionsParams.dwTexturePack!=0)
	{
		TexturePack *pTexturePack = pMinecraft->skins->getTexturePackById(m_MoreOptionsParams.dwTexturePack);
		DLCTexturePack *pDLCTexPack=static_cast<DLCTexturePack *>(pTexturePack);
		m_pDLCPack=pDLCTexPack->getDLCInfoParentPack();

		if(m_pDLCPack && !m_pDLCPack->hasPurchasedFile( DLCManager::e_DLCType_Texture, L"" ))
		{
			{
#ifdef _XBOX
				DLC_INFO *pDLCInfo = app.GetDLCInfoForTrialOfferID(m_pDLCPack->getPurchaseOfferId());
				ULONGLONG ullOfferID_Full;

				if(pDLCInfo!=nullptr)
				{
					ullOfferID_Full=pDLCInfo->ullOfferID_Full;
				}
				else
				{
					ullOfferID_Full=pTexturePack->getDLCPack()->getPurchaseOfferId();
				}

				TelemetryManager->RecordUpsellPresented(m_iPad, eSet_UpsellID_Texture_DLC, ullOfferID_Full & 0xFFFFFFFF);
#endif

#if defined(_WINDOWS64) || defined(_DURANGO)
				UINT uiIDA[1];
				uiIDA[0]=IDS_CONFIRM_OK;
				ui.RequestAlertMessage(IDS_WARNING_DLC_TRIALTEXTUREPACK_TITLE, IDS_USING_TRIAL_TEXUREPACK_WARNING, uiIDA, 1, m_iPad,&TrialTexturePackWarningReturned,this);
#elif defined(__PS3__) || defined(__ORBIS__) || defined(__PSVITA__)
				UINT uiIDA[2];
				uiIDA[0]=IDS_CONFIRM_OK;
				uiIDA[1]=IDS_CONFIRM_CANCEL;
				ui.RequestAlertMessage(IDS_WARNING_DLC_TRIALTEXTUREPACK_TITLE, IDS_USING_TRIAL_TEXUREPACK_WARNING, uiIDA, 2, m_iPad,&TrialTexturePackWarningReturned,this);
#endif

#if defined _XBOX_ONE || defined __ORBIS__
				StorageManager.SetSaveDisabled(true);
#endif
				return;
			}
		}			
	}
	app.SetGameHostOption(eGameHostOption_WasntSaveOwner, (!m_bIsSaveOwner)); 

#if defined _XBOX_ONE || defined __ORBIS__
	app.SetGameHostOption(eGameHostOption_DisableSaving, m_MoreOptionsParams.bDisableSaving?1:0);
	StorageManager.SetSaveDisabled(m_MoreOptionsParams.bDisableSaving);

	int newWorldSize = 0;
	int newHellScale = 0;
	switch(m_MoreOptionsParams.newWorldSize)
	{
	case e_worldSize_Unknown:	
		newWorldSize = 0;
		newHellScale = 0;
		break;
	case e_worldSize_Classic:	
		newWorldSize = LEVEL_WIDTH_CLASSIC;	
		newHellScale = HELL_LEVEL_SCALE_CLASSIC;
		break;
	case e_worldSize_Small:		
		newWorldSize = LEVEL_WIDTH_SMALL;	
		newHellScale = HELL_LEVEL_SCALE_SMALL;
		break;
	case e_worldSize_Medium:	
		newWorldSize = LEVEL_WIDTH_MEDIUM;	
		newHellScale = HELL_LEVEL_SCALE_MEDIUM;
		break;
	case e_worldSize_Large:		
		newWorldSize = LEVEL_WIDTH_LARGE;	
		newHellScale = HELL_LEVEL_SCALE_LARGE;
		break;
	default:
		assert(0);
		break;
	}
	bool bUseMoat = !m_MoreOptionsParams.newWorldSizeOverwriteEdges;
	app.SetGameNewWorldSize(newWorldSize, bUseMoat);
	app.SetGameNewHellScale(newHellScale);
	app.SetGameHostOption(eGameHostOption_WorldSize, m_MoreOptionsParams.newWorldSize);

#endif

	if(m_MoreOptionsParams.bResetNether==TRUE)
	{
		UINT uiIDA[2];
		uiIDA[0]=IDS_DONT_RESET_END;
		uiIDA[1]=IDS_RESET_END;

		ui.RequestAlertMessage(IDS_RESET_END, IDS_RESET_END_INFO, uiIDA, 2, m_iPad,&UIScene_LoadMenu::CheckResetNetherReturned,this);
	}
	else
	{
		LaunchGame();
	}
}

void UIScene_LoadMenu::handleSliderMove(F64 sliderId, F64 currentValue)
{
	WCHAR TempString[256];
	int value = static_cast<int>(currentValue);
	switch(static_cast<int>(sliderId))
	{
	case eControl_Difficulty:
		m_sliderDifficulty.handleSliderMove(value);

		app.SetGameSettings(m_iPad,eGameSetting_Difficulty,value);
		swprintf( (WCHAR *)TempString, 256, L"%ls: %ls", app.GetString( IDS_SLIDER_DIFFICULTY ),app.GetString(m_iDifficultyTitleSettingA[value]));		
		m_sliderDifficulty.setLabel(TempString);
		break;
	}
}

void UIScene_LoadMenu::handleTouchBoxRebuild()
{
	m_bRebuildTouchBoxes = true;
}


void UIScene_LoadMenu::handleTimerComplete(int id)
{
#ifdef __PSVITA__
	if(m_bRebuildTouchBoxes)
	{
		GetMainPanel()->UpdateControl();
		ui.TouchBoxRebuild(this);
		m_bRebuildTouchBoxes = false;
	}
#endif

	switch(id)
	{
	case GAME_CREATE_ONLINE_TIMER_ID:
		{
			bool bMultiplayerAllowed = ProfileManager.IsSignedInLive( m_iPad ) && ProfileManager.AllowedToPlayMultiplayer(m_iPad);

			if(bMultiplayerAllowed != m_bMultiplayerAllowed)
			{
				if( bMultiplayerAllowed )
				{
					bool bGameSetting_Online=(app.GetGameSettings(m_iPad,eGameSetting_Online)!=0);
					m_MoreOptionsParams.bOnlineGame = bGameSetting_Online?TRUE:FALSE;
					if(bGameSetting_Online)
					{
						m_MoreOptionsParams.bInviteOnly = (app.GetGameSettings(m_iPad,eGameSetting_InviteOnly)!=0)?TRUE:FALSE;
						m_MoreOptionsParams.bAllowFriendsOfFriends = (app.GetGameSettings(m_iPad,eGameSetting_FriendsOfFriends)!=0)?TRUE:FALSE;
					}
					else
					{
						m_MoreOptionsParams.bInviteOnly = FALSE;
						m_MoreOptionsParams.bAllowFriendsOfFriends = FALSE;
					}
				}
				else
				{
					m_MoreOptionsParams.bOnlineGame = FALSE;
					m_MoreOptionsParams.bInviteOnly = FALSE;
					m_MoreOptionsParams.bAllowFriendsOfFriends = FALSE;
				}

				m_checkboxOnline.SetEnable(bMultiplayerAllowed);
				m_checkboxOnline.setChecked(m_MoreOptionsParams.bOnlineGame);

				m_bMultiplayerAllowed = bMultiplayerAllowed;
			}
		}
		break;
#ifdef _XBOX
	case CHECKFORAVAILABLETEXTUREPACKS_TIMER_ID:
		{

#if defined(__PS3__) || defined(__ORBIS__)
			for(int i=0;i<m_iTexturePacksNotInstalled;i++)
			{
				if(m_iConfigA[i]!=-1)
				{
					DLC_INFO *pDLCInfo=app.GetDLCInfoFromTPackID(m_iConfigA[i]);

					if(pDLCInfo)
					{
						wstring textureName = filenametowstring(pDLCInfo->chImageURL);

						if(hasRegisteredSubstitutionTexture(textureName)==false)
						{
							PBYTE pbImageData;
							int iImageDataBytes=0;
							SonyHttp::getDataFromURL(pDLCInfo->chImageURL,(void **)&pbImageData,&iImageDataBytes);

							if(iImageDataBytes!=0)
							{
								registerSubstitutionTexture(textureName,pbImageData,iImageDataBytes,true);
								m_texturePackList.addPack(m_iConfigA[i],textureName);
								m_iConfigA[i]=-1;
							}
						}
						else
						{
							m_texturePackList.addPack(m_iConfigA[i],textureName);
							m_iConfigA[i]=-1;
						}
					}
				}
			}

			bool bAllDone=true;
			for(int i=0;i<m_iTexturePacksNotInstalled;i++)
			{
				if(m_iConfigA[i]!=-1) 
				{
					bAllDone = false;
				}
			}

			if(bAllDone)
			{
				killTimer(CHECKFORAVAILABLETEXTUREPACKS_TIMER_ID);
			}
#endif

		}
		break;
#endif	
	}
}

void UIScene_LoadMenu::LaunchGame(void)
{
#ifdef _XBOX
	killTimer(CHECKFORAVAILABLETEXTUREPACKS_TIMER_ID);
#endif

	if( (m_bGameModeCreative == true || m_bHasBeenInCreative) || m_MoreOptionsParams.bHostPrivileges == TRUE)
	{			
		UINT uiIDA[2];
		uiIDA[0]=IDS_CONFIRM_OK;
		uiIDA[1]=IDS_CONFIRM_CANCEL;
		if(m_bGameModeCreative == true || m_bHasBeenInCreative)
		{
			if(!m_bGameModeCreative)
			{
				ui.RequestAlertMessage(IDS_TITLE_START_GAME, IDS_CONFIRM_START_SAVEDINCREATIVE, uiIDA, 2, m_iPad,&UIScene_LoadMenu::ConfirmLoadReturned,this);
			}
			else
			{
				if(m_bHasBeenInCreative)
				{
					if(m_levelGen != nullptr)
					{
						m_bIsCorrupt = false;
						LoadDataComplete(this);
					}
					else
					{

						PSAVE_DETAILS pSaveDetails=StorageManager.ReturnSavesInfo();
#ifndef _DURANGO
						app.DebugPrintf("Loading save s [%s]\n",pSaveDetails->SaveInfoA[(int)m_iSaveGameInfoIndex].UTF8SaveTitle,pSaveDetails->SaveInfoA[(int)m_iSaveGameInfoIndex].UTF8SaveFilename);
#endif
						C4JStorage::ESaveGameState eLoadStatus=StorageManager.LoadSaveData(&pSaveDetails->SaveInfoA[(int)m_iSaveGameInfoIndex],&LoadSaveDataReturned,this);
					}
				}
				else
				{
					ui.RequestAlertMessage(IDS_TITLE_START_GAME, IDS_CONFIRM_START_CREATIVE, uiIDA, 2, m_iPad,&UIScene_LoadMenu::ConfirmLoadReturned,this);
				}
			}
		}
		else
		{
			ui.RequestAlertMessage(IDS_TITLE_START_GAME, IDS_CONFIRM_START_HOST_PRIVILEGES, uiIDA, 2, m_iPad,&UIScene_LoadMenu::ConfirmLoadReturned,this);
		}
	}
	else
	{
		if(m_levelGen != nullptr)
		{
			m_bIsCorrupt = false;
			LoadDataComplete(this);
		}
		else
		{
			PSAVE_DETAILS pSaveDetails=StorageManager.ReturnSavesInfo();
#ifndef _DURANGO
			app.DebugPrintf("Loading save %s [%s]\n",pSaveDetails->SaveInfoA[(int)m_iSaveGameInfoIndex].UTF8SaveTitle,pSaveDetails->SaveInfoA[(int)m_iSaveGameInfoIndex].UTF8SaveFilename);
#endif
			C4JStorage::ESaveGameState eLoadStatus=StorageManager.LoadSaveData(&pSaveDetails->SaveInfoA[(int)m_iSaveGameInfoIndex],&LoadSaveDataReturned,this);
		}
	}
}

int UIScene_LoadMenu::CheckResetNetherReturned(void *pParam,int iPad,C4JStorage::EMessageResult result)
{
	UIScene_LoadMenu* pClass = static_cast<UIScene_LoadMenu *>(pParam);

	if(result==C4JStorage::EMessage_ResultDecline) 
	{
		pClass->LaunchGame();
	}
	else if(result==C4JStorage::EMessage_ResultAccept)
	{
		pClass->m_MoreOptionsParams.bResetNether=FALSE;
		pClass->LaunchGame();
	}
	else
	{
		pClass->m_bIgnoreInput=false;
	}
	return 0;
}

int UIScene_LoadMenu::ConfirmLoadReturned(void *pParam,int iPad,C4JStorage::EMessageResult result)
{
	UIScene_LoadMenu* pClass = static_cast<UIScene_LoadMenu *>(pParam);

	if(result==C4JStorage::EMessage_ResultAccept) 
	{
		if(pClass->m_levelGen != nullptr)
		{
			pClass->m_bIsCorrupt = false;
			pClass->LoadDataComplete(pClass);
		}
		else
		{
			PSAVE_DETAILS pSaveDetails=StorageManager.ReturnSavesInfo();
#ifndef _DURANGO
			app.DebugPrintf("Loading save %s [%s]\n",pSaveDetails->SaveInfoA[(int)pClass->m_iSaveGameInfoIndex].UTF8SaveTitle,pSaveDetails->SaveInfoA[(int)pClass->m_iSaveGameInfoIndex].UTF8SaveFilename);
#endif
			C4JStorage::ESaveGameState eLoadStatus=StorageManager.LoadSaveData(&pSaveDetails->SaveInfoA[(int)pClass->m_iSaveGameInfoIndex],&LoadSaveDataReturned,pClass);

#if TO_BE_IMPLEMENTED
			if(eLoadStatus==C4JStorage::ELoadGame_DeviceRemoved)
			{
				StorageManager.SetSaveDisabled(true);
				StorageManager.SetSaveDeviceSelected(m_iPad,false);
				UINT uiIDA[1];
				uiIDA[0]=IDS_OK;
				ui.RequestErrorMessage(IDS_STORAGEDEVICEPROBLEM_TITLE, IDS_FAILED_TO_LOADSAVE_TEXT, uiIDA, 1, m_iPad,&CScene_LoadGameSettings::DeviceRemovedDialogReturned,this);
			}
#endif
		}
	}
	else
	{
		pClass->m_bIgnoreInput=false;
	}
	return 0;
}

int UIScene_LoadMenu::LoadDataComplete(void *pParam)
{
	UIScene_LoadMenu* pClass = static_cast<UIScene_LoadMenu *>(pParam);

	if(!pClass->m_bIsCorrupt)
	{
		int iPrimaryPad = ProfileManager.GetPrimaryPad();
		bool isSignedInLive = true;
		bool isOnlineGame = pClass->m_MoreOptionsParams.bOnlineGame;
		int iPadNotSignedInLive = -1;
		bool isLocalMultiplayerAvailable = app.IsLocalMultiplayerAvailable();

		for(unsigned int i = 0; i < XUSER_MAX_COUNT; ++i)
		{
			if (ProfileManager.IsSignedIn(i) && ((i == iPrimaryPad) || isLocalMultiplayerAvailable))
			{
				if (isSignedInLive && !ProfileManager.IsSignedInLive(i))
				{
					iPadNotSignedInLive = i;
				}

				isSignedInLive = isSignedInLive && ProfileManager.IsSignedInLive(i);
			}
		}

		if (isOnlineGame && !isSignedInLive)
		{
#ifdef __ORBIS__
			assert(iPadNotSignedInLive != -1);
			int npAvailability = ProfileManager.getNPAvailability(iPadNotSignedInLive);
			if (npAvailability == SCE_NP_ERROR_AGE_RESTRICTION)
			{
				pClass->m_bIgnoreInput = false;
				UINT uiIDA[1];
				uiIDA[0]=IDS_OK;
				ui.RequestErrorMessage(IDS_ONLINE_SERVICE_TITLE, IDS_CONTENT_RESTRICTION, uiIDA, 1, iPadNotSignedInLive);
			}
			else
			{
				pClass->m_bIgnoreInput=true;
				UINT uiIDA[2];
				uiIDA[0] = IDS_PRO_NOTONLINE_ACCEPT;
				uiIDA[1] = IDS_CANCEL;
				ui.RequestAlertMessage( IDS_PRO_NOTONLINE_TITLE, IDS_PRO_NOTONLINE_TEXT, uiIDA, 2, iPadNotSignedInLive, &UIScene_LoadMenu::MustSignInReturnedPSN, pClass);
			}
		return 0;
#else
			pClass->m_bIgnoreInput=false;
			UINT uiIDA[1];
			uiIDA[0]=IDS_CONFIRM_OK;
			ui.RequestAlertMessage( IDS_PRO_NOTONLINE_TITLE, IDS_PRO_NOTONLINE_TEXT, uiIDA,1,ProfileManager.GetPrimaryPad());
			return 0;
#endif
		}

		bool noUGC = false;
		BOOL pccAllowed = TRUE;
		BOOL pccFriendsAllowed = TRUE;
		bool bContentRestricted = false;
		ProfileManager.AllowedPlayerCreatedContent(ProfileManager.GetPrimaryPad(),false,&pccAllowed,&pccFriendsAllowed);
#if defined(__PS3__) || defined(__PSVITA__)
		if(isOnlineGame)
		{
			ProfileManager.GetChatAndContentRestrictions(ProfileManager.GetPrimaryPad(),false,nullptr,&bContentRestricted,nullptr);
		}
#endif

#ifdef __ORBIS__
		bool bPlayStationPlus=true;
		int iPadWithNoPlaystationPlus=0;
		for(unsigned int i = 0; i < XUSER_MAX_COUNT; ++i)
		{
			if(ProfileManager.IsSignedIn(i) && ((i == iPrimaryPad) || isLocalMultiplayerAvailable))
			{
				if(!ProfileManager.HasPlayStationPlus(i))
				{
					bPlayStationPlus=false;
					iPadWithNoPlaystationPlus=i;
					break;
				}
			}
		}
#endif
		noUGC = !pccAllowed && !pccFriendsAllowed;

		if(!isOnlineGame || !isLocalMultiplayerAvailable)
		{
			if(isOnlineGame && noUGC )
			{
				pClass->setVisible( true );

				ui.RequestUGCMessageBox();

				pClass->m_bIgnoreInput=false;
			}
			else if(isOnlineGame && bContentRestricted )
			{
				pClass->setVisible( true );

				ui.RequestContentRestrictedMessageBox();
				pClass->m_bIgnoreInput=false;
			}
#ifdef __ORBIS__
			else if(isOnlineGame && (bPlayStationPlus==false))
			{
				pClass->setVisible( true );
				pClass->m_bIgnoreInput=false;

				if(ProfileManager.RequestingPlaystationPlus(iPadWithNoPlaystationPlus))
				{
					UINT uiIDA[1];
					uiIDA[0]=IDS_OK;
					ui.RequestAlertMessage(IDS_ERROR_NETWORK_TITLE, IDS_ERROR_NETWORK, uiIDA, 1, ProfileManager.GetPrimaryPad(), nullptr, nullptr);
					return 0;
				}

				int32_t iResult=sceNpCommerceDialogInitialize();

				SceNpCommerceDialogParam param;
				sceNpCommerceDialogParamInitialize(&param);
				param.mode=SCE_NP_COMMERCE_DIALOG_MODE_PLUS;
				param.features = SCE_NP_PLUS_FEATURE_REALTIME_MULTIPLAY; 
				param.userId = ProfileManager.getUserID(iPadWithNoPlaystationPlus);

				iResult=sceNpCommerceDialogOpen(&param);
			}
#endif		
			else
			{
				
#if defined(__ORBIS__) || defined(__PSVITA__)
				if(isOnlineGame)
				{
					bool chatRestricted = false;
					ProfileManager.GetChatAndContentRestrictions(ProfileManager.GetPrimaryPad(),false,&chatRestricted,nullptr,nullptr);
					if(chatRestricted)
					{
						ProfileManager.DisplaySystemMessage( SCE_MSG_DIALOG_SYSMSG_TYPE_TRC_PSN_CHAT_RESTRICTION, ProfileManager.GetPrimaryPad() );
					}
				}
#endif
				DWORD dwLocalUsersMask = CGameNetworkManager::GetLocalPlayerMask(ProfileManager.GetPrimaryPad());

				StartGameFromSave(pClass, dwLocalUsersMask);
			}
		}
		else
		{
			if(isOnlineGame && noUGC )
			{
				pClass->setVisible( true );
				ui.RequestUGCMessageBox();
				pClass->m_bIgnoreInput=false;
			}
			else if(isOnlineGame && bContentRestricted )
			{
				pClass->setVisible( true );
				ui.RequestContentRestrictedMessageBox();
				pClass->m_bIgnoreInput=false;
			}
#ifdef __ORBIS__
			else if(bPlayStationPlus==false)
			{
				pClass->setVisible( true );
				pClass->m_bIgnoreInput=false;

				if(ProfileManager.RequestingPlaystationPlus(iPadWithNoPlaystationPlus))
				{
					UINT uiIDA[1];
					uiIDA[0]=IDS_OK;
					ui.RequestAlertMessage(IDS_ERROR_NETWORK_TITLE, IDS_ERROR_NETWORK, uiIDA, 1, ProfileManager.GetPrimaryPad(), nullptr, nullptr);
					return 0;
				}

				int32_t iResult=sceNpCommerceDialogInitialize();

				SceNpCommerceDialogParam param;
				sceNpCommerceDialogParamInitialize(&param);
				param.mode=SCE_NP_COMMERCE_DIALOG_MODE_PLUS;
				param.features = SCE_NP_PLUS_FEATURE_REALTIME_MULTIPLAY; 
				param.userId = ProfileManager.getUserID(iPadWithNoPlaystationPlus);

				iResult=sceNpCommerceDialogOpen(&param);
			}
#endif
			else
			{
#ifdef _WINDOWS64
				DWORD dwLocalUsersMask = CGameNetworkManager::GetLocalPlayerMask(ProfileManager.GetPrimaryPad());
				StartGameFromSave(pClass, dwLocalUsersMask);
#else
				pClass->m_bRequestQuadrantSignin = true;
#endif
			}
		}
	}
	else
	{
		pClass->m_bIgnoreInput=false;

		UINT uiIDA[2];
		uiIDA[0]=IDS_CONFIRM_CANCEL;
		uiIDA[1]=IDS_CONFIRM_OK;
		ui.RequestAlertMessage(IDS_CORRUPT_OR_DAMAGED_SAVE_TITLE, IDS_CORRUPT_OR_DAMAGED_SAVE_TEXT, uiIDA, 2, pClass->m_iPad,&UIScene_LoadMenu::DeleteSaveDialogReturned,pClass);

	}

	return 0;
}

int UIScene_LoadMenu::LoadSaveDataReturned(void *pParam,bool bIsCorrupt, bool bIsOwner)
{
	UIScene_LoadMenu* pClass = static_cast<UIScene_LoadMenu *>(pParam);

	pClass->m_bIsCorrupt=bIsCorrupt;

#ifdef _WINDOWS64
	// Voxel - only load saves made after the tile ID expansion, to prevent loss of old worlds
	if(!bIsCorrupt)
	{
		unsigned int fileSize = StorageManager.GetSaveSize();
		if(fileSize > 0)
		{
			void *pvSaveData = malloc(fileSize);
			if(pvSaveData != nullptr)
			{
				StorageManager.GetSaveData(pvSaveData, &fileSize);

				ConsoleSaveFileOriginal saveHeaderPeek(L"", pvSaveData, fileSize, false, SAVE_FILE_PLATFORM_LOCAL);
				int originalSaveVersion = saveHeaderPeek.getOriginalSaveVersion();

				free(pvSaveData);

				if(originalSaveVersion < SAVE_FILE_VERSION_TILE_ID_EXPANSION)
				{
					app.DebugPrintf("Refusing to load save created with original save version %d (tile ID expansion requires >= %d)\n", originalSaveVersion, SAVE_FILE_VERSION_TILE_ID_EXPANSION);

					pClass->m_bIgnoreInput=false;

					UINT uiIDA[1];
					uiIDA[0]=IDS_CONFIRM_OK;
					ui.RequestAlertMessage(IDS_TITLE_START_GAME, IDS_SAVE_TRANSFER_WRONG_VERSION, uiIDA, 1, pClass->m_iPad);
					return 0;
				}
			}
		}
	}
#endif

#if defined(__PS3__) || defined(__ORBIS__) || defined (__PSVITA__)
	if(app.GetGameHostOption(eGameHostOption_WasntSaveOwner))
	{
		bIsOwner = false;
	}
#endif

	if(bIsOwner)
	{
		LoadDataComplete(pClass);
	}
	else
	{
		pClass->m_bIgnoreInput=false;

#if defined(__PS3__) || defined(__ORBIS__) || defined (__PSVITA__)
		UINT uiIDA[1];
		uiIDA[0]=IDS_CONFIRM_OK;
		ui.RequestErrorMessage(IDS_SAVEDATA_COPIED_TITLE, IDS_SAVEDATA_COPIED_TEXT, uiIDA, 1, 
			pClass->m_iPad,&UIScene_LoadMenu::TrophyDialogReturned,pClass);
		app.SetGameHostOption(eGameHostOption_WasntSaveOwner, true);
#endif
	}
	return 0;
}

int UIScene_LoadMenu::TrophyDialogReturned(void *pParam,int iPad,C4JStorage::EMessageResult result)
{
	UIScene_LoadMenu* pClass = static_cast<UIScene_LoadMenu *>(pParam);
	return LoadDataComplete(pClass);
}

int UIScene_LoadMenu::DeleteSaveDialogReturned(void *pParam,int iPad,C4JStorage::EMessageResult result)
{
	UIScene_LoadMenu* pClass = static_cast<UIScene_LoadMenu *>(pParam);

	if(result==C4JStorage::EMessage_ResultDecline) 
	{
		PSAVE_DETAILS pSaveDetails=StorageManager.ReturnSavesInfo();
		StorageManager.DeleteSaveData(&pSaveDetails->SaveInfoA[(int)pClass->m_iSaveGameInfoIndex],UIScene_LoadMenu::DeleteSaveDataReturned,pClass);
	}
	else
	{
		pClass->m_bIgnoreInput=false;
	}
	return 0;
}

int UIScene_LoadMenu::DeleteSaveDataReturned(void *pParam,bool bSuccess)
{
	UIScene_LoadMenu* pClass = static_cast<UIScene_LoadMenu *>(pParam);

	app.SetCorruptSaveDeleted(true);
	pClass->navigateBack();

	return 0;
}

void UIScene_LoadMenu::StartGameFromSave(UIScene_LoadMenu* pClass, DWORD dwLocalUsersMask)
{
	if(pClass->m_levelGen == nullptr)
	{
		INT saveOrCheckpointId = 0;
		bool validSave = StorageManager.GetSaveUniqueNumber(&saveOrCheckpointId);
		TelemetryManager->RecordLevelResume(pClass->m_iPad, eSen_FriendOrMatch_Playing_With_Invited_Friends, eSen_CompeteOrCoop_Coop_and_Competitive, app.GetGameSettings(pClass->m_iPad,eGameSetting_Difficulty), app.GetLocalPlayerCount(), g_NetworkManager.GetOnlinePlayerCount(), saveOrCheckpointId);
	}
	else
	{		
		StorageManager.ResetSaveData();
		StorageManager.SetSaveTitle(pClass->m_levelGen->getDefaultSaveName().c_str());
	}

	bool isClientSide = ProfileManager.IsSignedInLive(ProfileManager.GetPrimaryPad()) && pClass->m_MoreOptionsParams.bOnlineGame;
#ifdef __PSVITA__
	if(CGameNetworkManager::usingAdhocMode())
	{
		if(SQRNetworkManager_AdHoc_Vita::GetAdhocStatus())
			isClientSide = true;
	}
#endif

	bool isPrivate = (app.GetGameSettings(pClass->m_iPad,eGameSetting_InviteOnly)>0)?true:false;

	PSAVE_DETAILS pSaveDetails=StorageManager.ReturnSavesInfo();

	NetworkGameInitData *param = new NetworkGameInitData();
	param->seed = pClass->m_seed;
	param->saveData = nullptr;	
	param->levelGen = pClass->m_levelGen;
	param->texturePackId = pClass->m_MoreOptionsParams.dwTexturePack;
	param->levelName = pClass->m_levelName;

	Minecraft *pMinecraft = Minecraft::GetInstance();
	pMinecraft->skins->selectTexturePackById(pClass->m_MoreOptionsParams.dwTexturePack);

	app.SetGameHostOption(eGameHostOption_Difficulty,Minecraft::GetInstance()->options->difficulty);
	app.SetGameHostOption(eGameHostOption_FriendsOfFriends,app.GetGameSettings(pClass->m_iPad,eGameSetting_FriendsOfFriends));
	app.SetGameHostOption(eGameHostOption_Gamertags,app.GetGameSettings(pClass->m_iPad,eGameSetting_GamertagsVisible));

	app.SetGameHostOption(eGameHostOption_BedrockFog,app.GetGameSettings(pClass->m_iPad,eGameSetting_BedrockFog)?1:0);

	app.SetGameHostOption(eGameHostOption_PvP,pClass->m_MoreOptionsParams.bPVP);
	app.SetGameHostOption(eGameHostOption_TrustPlayers,pClass->m_MoreOptionsParams.bTrust );
	app.SetGameHostOption(eGameHostOption_FireSpreads,pClass->m_MoreOptionsParams.bFireSpreads );
	app.SetGameHostOption(eGameHostOption_TNT,pClass->m_MoreOptionsParams.bTNT );
	app.SetGameHostOption(eGameHostOption_HostCanFly,pClass->m_MoreOptionsParams.bHostPrivileges);
	app.SetGameHostOption(eGameHostOption_HostCanChangeHunger,pClass->m_MoreOptionsParams.bHostPrivileges);
	app.SetGameHostOption(eGameHostOption_HostCanBeInvisible,pClass->m_MoreOptionsParams.bHostPrivileges );

	app.SetGameHostOption(eGameHostOption_MobGriefing, pClass->m_MoreOptionsParams.bMobGriefing);
	app.SetGameHostOption(eGameHostOption_KeepInventory, pClass->m_MoreOptionsParams.bKeepInventory);
	app.SetGameHostOption(eGameHostOption_DoMobSpawning, pClass->m_MoreOptionsParams.bDoMobSpawning);
	app.SetGameHostOption(eGameHostOption_DoMobLoot, pClass->m_MoreOptionsParams.bDoMobLoot);
	app.SetGameHostOption(eGameHostOption_DoTileDrops, pClass->m_MoreOptionsParams.bDoTileDrops);
	app.SetGameHostOption(eGameHostOption_NaturalRegeneration, pClass->m_MoreOptionsParams.bNaturalRegeneration);
	app.SetGameHostOption(eGameHostOption_DoDaylightCycle, pClass->m_MoreOptionsParams.bDoDaylightCycle);

#ifdef _LARGE_WORLDS
	app.SetGameHostOption(eGameHostOption_WorldSize, pClass->m_MoreOptionsParams.worldSize+1 );  // 0 is GAME_HOST_OPTION_WORLDSIZE_UNKNOWN
#endif
	app.SetResetNether((pClass->m_MoreOptionsParams.bResetNether==TRUE)?true:false);
	app.ClearTerrainFeaturePosition();

	app.SetGameHostOption(eGameHostOption_GameType,pClass->m_iGameModeId );

	g_NetworkManager.HostGame(dwLocalUsersMask,isClientSide,isPrivate,MINECRAFT_NET_MAX_PLAYERS,0);

	param->settings = app.GetGameHostOption( eGameHostOption_All );

#ifdef _WINDOWS64
	{
		extern wchar_t g_Win64UsernameW[17];
		Minecraft::GetInstance()->user->name = g_Win64UsernameW;
	}
#endif
#ifndef _XBOX
	g_NetworkManager.FakeLocalPlayerJoined();
#endif

	LoadingInputParams *loadingParams = new LoadingInputParams();
	loadingParams->func = &CGameNetworkManager::RunNetworkGameThreadProc;
	loadingParams->lpParam = static_cast<LPVOID>(param);

	app.SetAutosaveTimerTime();

	UIFullscreenProgressCompletionData *completionData = new UIFullscreenProgressCompletionData();
	completionData->bShowBackground=TRUE;
	completionData->bShowLogo=FALSE;
	completionData->type = e_ProgressCompletion_CloseAllPlayersUIScenes;
	completionData->iPad = DEFAULT_XUI_MENU_USER;
	loadingParams->completionData = completionData;

	ui.NavigateToScene(ProfileManager.GetPrimaryPad(),eUIScene_FullscreenProgress, loadingParams);
}

void UIScene_LoadMenu::checkStateAndStartGame()
{
	if(m_MoreOptionsParams.bResetNether==TRUE)
	{
		UINT uiIDA[2];
		uiIDA[0]=IDS_DONT_RESET_END;
		uiIDA[1]=IDS_RESET_END;

		ui.RequestAlertMessage(IDS_RESET_END, IDS_RESET_END_INFO, uiIDA, 2, m_iPad,&UIScene_LoadMenu::CheckResetNetherReturned,this);
	}
	else
	{
		LaunchGame();
	}
}

int UIScene_LoadMenu::StartGame_SignInReturned(void *pParam,bool bContinue, int iPad)
{
	UIScene_LoadMenu* pClass = static_cast<UIScene_LoadMenu *>(pParam);

	if(bContinue==true)
	{
		if(ProfileManager.IsSignedIn(pClass->m_iPad))
		{
			int primaryPad = ProfileManager.GetPrimaryPad();
			bool noPrivileges = false;
			DWORD dwLocalUsersMask = 0;
			bool isSignedInLive = ProfileManager.IsSignedInLive(primaryPad);
			bool isOnlineGame = pClass->m_MoreOptionsParams.bOnlineGame;
			int iPadNotSignedInLive = -1;
			bool isLocalMultiplayerAvailable = app.IsLocalMultiplayerAvailable();

			for(unsigned int i = 0; i < XUSER_MAX_COUNT; ++i)
			{
				if (ProfileManager.IsSignedIn(i) && ((i == primaryPad) || isLocalMultiplayerAvailable))
				{
					if (isSignedInLive && !ProfileManager.IsSignedInLive(i))
					{
						iPadNotSignedInLive = i;
					}

					if( !ProfileManager.AllowedToPlayMultiplayer(i) ) noPrivileges = true;
					dwLocalUsersMask |= CGameNetworkManager::GetLocalPlayerMask(i);
					isSignedInLive = isSignedInLive && ProfileManager.IsSignedInLive(i);
				}
			}

			if (isOnlineGame && !isSignedInLive)
			{
#ifdef __ORBIS__
				assert(iPadNotSignedInLive != -1);

				int npAvailability = ProfileManager.getNPAvailability(iPadNotSignedInLive);
				if (npAvailability == SCE_NP_ERROR_AGE_RESTRICTION)
				{
					pClass->m_bIgnoreInput = false;
					UINT uiIDA[1];
					uiIDA[0]=IDS_OK;
					ui.RequestErrorMessage(IDS_ONLINE_SERVICE_TITLE, IDS_CONTENT_RESTRICTION, uiIDA, 1, iPadNotSignedInLive);
				}
				else
				{
					pClass->m_bIgnoreInput=true;
					UINT uiIDA[2];
					uiIDA[0] = IDS_PRO_NOTONLINE_ACCEPT;
					uiIDA[1] = IDS_CANCEL;
					ui.RequestAlertMessage( IDS_PRO_NOTONLINE_TITLE, IDS_PRO_NOTONLINE_TEXT, uiIDA, 2, iPadNotSignedInLive, &UIScene_LoadMenu::MustSignInReturnedPSN, pClass);
				}
				return 0;
#else
				pClass->m_bIgnoreInput=false;
				UINT uiIDA[1];
				uiIDA[0]=IDS_CONFIRM_OK;
				ui.RequestAlertMessage( IDS_PRO_NOTONLINE_TITLE, IDS_PRO_NOTONLINE_TEXT, uiIDA,1,ProfileManager.GetPrimaryPad());
				return 0;
#endif
			}

			bool noUGC = false;
			BOOL pccAllowed = TRUE;
			BOOL pccFriendsAllowed = TRUE;

			ProfileManager.AllowedPlayerCreatedContent(ProfileManager.GetPrimaryPad(),false,&pccAllowed,&pccFriendsAllowed);
			if(!pccAllowed && !pccFriendsAllowed) noUGC = true;

			if(isSignedInLive && isOnlineGame && (noPrivileges || noUGC) )
			{
				if( noUGC )
				{
					pClass->m_bIgnoreInput = false;
					pClass->setVisible( true );
					UINT uiIDA[1];
					uiIDA[0]=IDS_CONFIRM_OK;
					ui.RequestAlertMessage( IDS_FAILED_TO_CREATE_GAME_TITLE, IDS_NO_USER_CREATED_CONTENT_PRIVILEGE_CREATE, uiIDA,1,ProfileManager.GetPrimaryPad());
				}
				else
				{
					pClass->m_bIgnoreInput = false;
					pClass->setVisible( true );
					UINT uiIDA[1];
					uiIDA[0]=IDS_CONFIRM_OK;
					ui.RequestAlertMessage( IDS_NO_MULTIPLAYER_PRIVILEGE_TITLE, IDS_NO_MULTIPLAYER_PRIVILEGE_HOST_TEXT, uiIDA,1,ProfileManager.GetPrimaryPad());
				}
			}
			else
			{
#if defined( __ORBIS__) || defined(__PSVITA__)
				if(isOnlineGame)
				{
					for(unsigned int i = 0; i < XUSER_MAX_COUNT; i++)
					{
						if(ProfileManager.IsSignedInLive(i))
						{
							bool chatRestricted = false;
							ProfileManager.GetChatAndContentRestrictions(i,false,&chatRestricted,nullptr,nullptr);
							if(chatRestricted)
							{
								ProfileManager.DisplaySystemMessage( SCE_MSG_DIALOG_SYSMSG_TYPE_TRC_PSN_CHAT_RESTRICTION, i );
							}
						}
					}
				}
#endif
				StartGameFromSave(pClass, dwLocalUsersMask);
			}
		}
	}
	else
	{
		pClass->m_bIgnoreInput=false;
	}

	return 0;
}

void UIScene_LoadMenu::handleGainFocus(bool navBack)
{
	if(navBack)
	{
		m_checkboxOnline.setChecked(m_MoreOptionsParams.bOnlineGame == TRUE);
	}
}

#ifdef __ORBIS__
int UIScene_LoadMenu::MustSignInReturnedPSN(void *pParam,int iPad,C4JStorage::EMessageResult result)
{
    UIScene_LoadMenu* pClass = (UIScene_LoadMenu *)pParam;
	pClass->m_bIgnoreInput = false;

    if(result==C4JStorage::EMessage_ResultAccept) 
    {
        SQRNetworkManager_Orbis::AttemptPSNSignIn(&UIScene_LoadMenu::StartGame_SignInReturned, pClass, false, iPad);
    }
    return 0;
}
#endif
