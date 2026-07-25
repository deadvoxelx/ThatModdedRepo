#include "stdafx.h"
#include "..\Common\Consoles_App.h"
#include "..\User.h"
#include "..\..\Minecraft.Client\Minecraft.h"
#include "..\..\Minecraft.Client\MinecraftServer.h"
#include "..\..\Minecraft.Client\PlayerList.h"
#include "..\..\Minecraft.Client\ServerPlayer.h"
#include "..\..\Minecraft.World\Level.h"
#include "..\..\Minecraft.World\LevelSettings.h"
#include "..\..\Minecraft.World\BiomeSource.h"
#include "..\..\Minecraft.World\LevelType.h"

#include "stb_image_write.h"

extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pImmediateContext;
extern IDXGISwapChain* g_pSwapChain;

CConsoleMinecraftApp app;

CConsoleMinecraftApp::CConsoleMinecraftApp() : CMinecraftApp()
{
	m_bShutdown = false;
}

void CConsoleMinecraftApp::SetRichPresenceContext(int iPad, int contextId)
{
	ProfileManager.SetRichPresenceContextValue(iPad,CONTEXT_GAME_STATE,contextId);
}

void CConsoleMinecraftApp::StoreLaunchData()
{
}
void CConsoleMinecraftApp::ExitGame()
{
	m_bShutdown = true;
}
void CConsoleMinecraftApp::FatalLoadError()
{
}

static const int THUMBNAIL_SIZE = 64;

void CConsoleMinecraftApp::CaptureSaveThumbnail()
{
	if (!g_pSwapChain || !g_pd3dDevice || !g_pImmediateContext)
		return;

	// Release any previous capture
	if (m_ThumbnailBuffer.Allocated())
		m_ThumbnailBuffer.Release();

	// Get the backbuffer
	ID3D11Texture2D* pBackBuffer = nullptr;
	HRESULT hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	if (FAILED(hr))
		return;

	D3D11_TEXTURE2D_DESC backDesc = {};
	pBackBuffer->GetDesc(&backDesc);

	// Create a staging texture at backbuffer size to read pixels
	D3D11_TEXTURE2D_DESC stagingDesc = backDesc;
	stagingDesc.Usage = D3D11_USAGE_STAGING;
	stagingDesc.BindFlags = 0;
	stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagingDesc.MiscFlags = 0;

	ID3D11Texture2D* pStaging = nullptr;
	hr = g_pd3dDevice->CreateTexture2D(&stagingDesc, nullptr, &pStaging);
	if (FAILED(hr))
	{
		pBackBuffer->Release();
		return;
	}

	g_pImmediateContext->CopyResource(pStaging, pBackBuffer);
	pBackBuffer->Release();

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	hr = g_pImmediateContext->Map(pStaging, 0, D3D11_MAP_READ, 0, &mapped);
	if (FAILED(hr))
	{
		pStaging->Release();
		return;
	}

	// Downsample to THUMBNAIL_SIZE x THUMBNAIL_SIZE with simple box filter
	unsigned char* thumb = new unsigned char[THUMBNAIL_SIZE * THUMBNAIL_SIZE * 4];

	// Crop to square (center crop), then scale down
	UINT srcSize = (backDesc.Width < backDesc.Height) ? backDesc.Width : backDesc.Height;
	UINT offsetX = (backDesc.Width - srcSize) / 2;
	UINT offsetY = (backDesc.Height - srcSize) / 2;

	for (int ty = 0; ty < THUMBNAIL_SIZE; ty++)
	{
		for (int tx = 0; tx < THUMBNAIL_SIZE; tx++)
		{
			// Map thumbnail pixel to source region
			UINT sx = offsetX + (tx * srcSize) / THUMBNAIL_SIZE;
			UINT sy = offsetY + (ty * srcSize) / THUMBNAIL_SIZE;

			const unsigned char* src = (const unsigned char*)mapped.pData + sy * mapped.RowPitch + sx * 4;
			unsigned char* dst = thumb + (ty * THUMBNAIL_SIZE + tx) * 4;

			dst[0] = src[0]; // R (or B depending on format, but BGRA->RGBA swap below)
			dst[1] = src[1]; // G
			dst[2] = src[2]; // B
			dst[3] = 0xFF;   // A
		}
	}

	g_pImmediateContext->Unmap(pStaging, 0);
	pStaging->Release();

	// If backbuffer is BGRA, swap to RGBA for PNG
	if (backDesc.Format == DXGI_FORMAT_B8G8R8A8_UNORM || backDesc.Format == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB)
	{
		for (int i = 0; i < THUMBNAIL_SIZE * THUMBNAIL_SIZE; i++)
		{
			unsigned char tmp = thumb[i * 4];
			thumb[i * 4] = thumb[i * 4 + 2];
			thumb[i * 4 + 2] = tmp;
		}
	}

	// Encode to PNG in memory using stbi_write_png_to_func
	struct PngBuffer { unsigned char* data; int size; int capacity; } pngBuf = {};
	pngBuf.capacity = THUMBNAIL_SIZE * THUMBNAIL_SIZE * 4 + 256;
	pngBuf.data = (unsigned char*)malloc(pngBuf.capacity);
	pngBuf.size = 0;

	stbi_write_png_to_func([](void* ctx, void* data, int size) {
		PngBuffer* buf = (PngBuffer*)ctx;
		if (buf->size + size > buf->capacity)
		{
			buf->capacity = (buf->size + size) * 2;
			buf->data = (unsigned char*)realloc(buf->data, buf->capacity);
		}
		memcpy(buf->data + buf->size, data, size);
		buf->size += size;
	}, &pngBuf, THUMBNAIL_SIZE, THUMBNAIL_SIZE, 4, thumb, THUMBNAIL_SIZE * 4);
	delete[] thumb;

	if (pngBuf.size > 0)
	{
		m_ThumbnailBuffer.m_type = ImageFileBuffer::e_typePNG;
		m_ThumbnailBuffer.m_pBuffer = pngBuf.data;
		m_ThumbnailBuffer.m_bufferSize = pngBuf.size;
	}
	else
	{
		free(pngBuf.data);
	}
}
void CConsoleMinecraftApp::GetSaveThumbnail(PBYTE *pbData,DWORD *pdwSize)
{
	// On a save caused by a create world, the thumbnail capture won't have happened
	if (m_ThumbnailBuffer.Allocated())
	{
		if (pbData)
		{
			*pbData  = new BYTE[m_ThumbnailBuffer.GetBufferSize()];
			*pdwSize = m_ThumbnailBuffer.GetBufferSize();
			memcpy(*pbData, m_ThumbnailBuffer.GetBufferPointer(), *pdwSize);
		}
		m_ThumbnailBuffer.Release();
	}
	else
	{
		// No capture happened (e.g. first save on world creation) leave thumbnail as nullptr
		if (pbData)  *pbData  = nullptr;
		if (pdwSize) *pdwSize = 0;
	}
}
void CConsoleMinecraftApp::ReleaseSaveThumbnail()
{
}

void CConsoleMinecraftApp::GetScreenshot(int iPad,PBYTE *pbData,DWORD *pdwSize)
{
}

void CConsoleMinecraftApp::TemporaryCreateGameStart()
{
	////////////////////////////////////////////////////////////////////////////////////////////// From CScene_Main::OnInit

	app.setLevelGenerationOptions(nullptr);

	// From CScene_Main::RunPlayGame
	Minecraft *pMinecraft=Minecraft::GetInstance();
	app.ReleaseSaveThumbnail();
	ProfileManager.SetLockedProfile(0);
	extern wchar_t g_Win64UsernameW[17];
	pMinecraft->user->name = g_Win64UsernameW;
	app.ApplyGameSettingsChanged(0);

	////////////////////////////////////////////////////////////////////////////////////////////// From CScene_MultiGameJoinLoad::OnInit
	MinecraftServer::resetFlags();

	// From CScene_MultiGameJoinLoad::OnNotifyPressEx
	app.SetTutorialMode( false );
	app.SetCorruptSaveDeleted(false);

	////////////////////////////////////////////////////////////////////////////////////////////// From CScene_MultiGameCreate::CreateGame

	app.ClearTerrainFeaturePosition();
	wstring wWorldName = L"TestWorld";

	StorageManager.ResetSaveData();
	StorageManager.SetSaveTitle(wWorldName.c_str());

	bool isFlat = false;
	int64_t seedValue = 0; // BiomeSource::findSeed(isFlat?LevelType::lvl_flat:LevelType::lvl_normal);	// 4J - was (new Random())->nextLong() - now trying to actually find a seed to suit our requirements

	NetworkGameInitData *param = new NetworkGameInitData();
	param->seed = seedValue;
	param->saveData = nullptr;

	app.SetGameHostOption(eGameHostOption_Difficulty,0);
	app.SetGameHostOption(eGameHostOption_FriendsOfFriends,0);
	app.SetGameHostOption(eGameHostOption_Gamertags,1);
	app.SetGameHostOption(eGameHostOption_BedrockFog,1);

	app.SetGameHostOption(eGameHostOption_GameType,GameType::CREATIVE->getId() ); // LevelSettings::GAMETYPE_SURVIVAL
	app.SetGameHostOption(eGameHostOption_LevelType, 0 );
	app.SetGameHostOption(eGameHostOption_LevelTypeFarlands, 0 );
	app.SetGameHostOption(eGameHostOption_Structures, 1 );
	app.SetGameHostOption(eGameHostOption_BonusChest, 0 );

	app.SetGameHostOption(eGameHostOption_PvP, 1);
	app.SetGameHostOption(eGameHostOption_TrustPlayers, 1 );
	app.SetGameHostOption(eGameHostOption_FireSpreads, 1 );
	app.SetGameHostOption(eGameHostOption_TNT, 1 );
	app.SetGameHostOption(eGameHostOption_HostCanFly, 1);
	app.SetGameHostOption(eGameHostOption_HostCanChangeHunger, 1);
	app.SetGameHostOption(eGameHostOption_HostCanBeInvisible, 1 );

	param->settings = app.GetGameHostOption( eGameHostOption_All );

	g_NetworkManager.FakeLocalPlayerJoined();

	LoadingInputParams *loadingParams = new LoadingInputParams();
	loadingParams->func = &CGameNetworkManager::RunNetworkGameThreadProc;
	loadingParams->lpParam = static_cast<LPVOID>(param);

	// Reset the autosave time
	app.SetAutosaveTimerTime();

	C4JThread* thread = new C4JThread(loadingParams->func, loadingParams->lpParam, "RunNetworkGame");
	thread->Run();
}

int CConsoleMinecraftApp::GetLocalTMSFileIndex(WCHAR *wchTMSFile,bool bFilenameIncludesExtension,eFileExtensionType eEXT)
{
	return -1;
}

int CConsoleMinecraftApp::LoadLocalTMSFile(WCHAR *wchTMSFile)
{
	return -1;
}

int CConsoleMinecraftApp::LoadLocalTMSFile(WCHAR *wchTMSFile, eFileExtensionType eExt)
{
	return -1;
}

void CConsoleMinecraftApp::FreeLocalTMSFiles(eTMSFileType eType)
{
}
