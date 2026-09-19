#include "stdafx.h"
#include "UI.h"
#include "UIScene_ModsMenu.h"
#include <algorithm>
#include <fstream>

#ifdef _WINDOWS64
#include <shellapi.h>

#include "Common/ModPaths.h"
#include "Host/RubyLauncherHost.h"

static const wchar_t *MODS_FOLDER_PATH = L"mods";
static const wchar_t *ICON_TEXTURE_NAME = L"rubyLauncher";
static const wchar_t *ICON_FILE = L"Common/res/1_2_2/gui/rubyLauncher.png";

static wstring formatFileSize(uint64_t bytes)
{
	WCHAR buffer[64];

	if (bytes >= 1024ULL * 1024ULL * 1024ULL)
	{
		swprintf(buffer, 64, L"%.2f GB", bytes / (1024.0 * 1024.0 * 1024.0));
	}
	else if (bytes >= 1024ULL * 1024ULL)
	{
		swprintf(buffer, 64, L"%.1f MB", bytes / (1024.0 * 1024.0));
	}
	else if (bytes >= 1024ULL)
	{
		swprintf(buffer, 64, L"%.1f KB", bytes / 1024.0);
	}
	else
	{
		swprintf(buffer, 64, L"%llu B", bytes);
	}

	return buffer;
}

static wstring createModsFolder()
{	// Checks if the folder already exists or not
	// If it doesnt, it creates it
	wstring folder = MODS_FOLDER_PATH;

	if (CreateDirectoryW(folder.c_str(), nullptr) == FALSE)
	{
		DWORD lastError = GetLastError();
		if (lastError != ERROR_ALREADY_EXISTS)
		{
			app.DebugPrintf("failed to create '%ls' (error %lu) :(\n", folder.c_str(), lastError);
		}
	}

	return folder;
}

wstring UIScene_ModsMenu::getMoviePath()
{
	return L"ModsMenu";
}

UIScene_ModsMenu::UIScene_ModsMenu(int iPad, void *initData, UILayer *parentLayer) : UIScene(iPad, parentLayer)
{
	initialiseMovie();
	
	m_iState = e_ListIdle;
	m_iModListIndex = 0;
	m_bFlashReportedFolderPress = false;
	m_buttonListMods.init(eControl_ModsList);
	m_labelModsListTitle.init( IDS_MODS_MENU );
	m_labelNoMods.init( IDS_NO_MODS );
	m_labelModName.init( IDS_MOD_NAME );
	m_labelModID.init( IDS_MOD_ID );
	m_labelModVersion.init( IDS_MOD_VERSION );
	m_labelModSize.init( IDS_MOD_SIZE );
	m_labelModStatus.init( IDS_MOD_STATUS );
	m_labelRubyLauncher.init( IDS_RUBY_LAUNCHER );
	m_labelNoMods.setVisible(false);
	m_controlModsTimer.setVisible(true);
	m_buttonModFolder.init(IDS_MOD_FOLDER, eControl_ModFolderButton);

	replaceTheIcon();
	refreshModsList();
}

void UIScene_ModsMenu::replaceTheIcon()
{	// Drawing the texture from res/1_2_2/gui
	// Aint no way in hell im figuring out how to actually draw it from the swf anytime soon
	// Its still fully bound to the swf component hpowever, so itll always position and scale correctly
	if (!hasRegisteredSubstitutionTexture(ICON_TEXTURE_NAME))
	{
		std::ifstream file(ICON_FILE, std::ios::binary | std::ios::ate);

		if (file.is_open())
		{
			std::streamsize size = file.tellg();
			file.seekg(0, std::ios::beg);

			if (size > 0)
			{
				PBYTE data = new BYTE[static_cast<size_t>(size)];

				if (file.read(reinterpret_cast<char *>(data), size))
				{
					registerSubstitutionTexture(ICON_TEXTURE_NAME, data, static_cast<DWORD>(size), true);
				}
				else
				{
					delete[] data;
				}
			}
		}
	}

	m_icon.setTextureName(ICON_TEXTURE_NAME);
}

UIScene_ModsMenu::~UIScene_ModsMenu()
{
}

void UIScene_ModsMenu::refreshModsList()
{
	wstring folder = createModsFolder();

	m_mods.clear();
	m_buttonListMods.clearList();
	m_iModListIndex = 0;

	vector<RubyModInfo> installed = RubyLoader::getInstalledMods();

	for (const RubyModInfo &info : installed)
	{
		ModEntry entry;
		entry.name = RubyPaths::toWide(info.name);
		entry.version = RubyPaths::toWide(info.version);
		entry.modId = RubyPaths::toWide(info.modId);
		entry.error = RubyPaths::toWide(info.error);
		entry.loaded = info.loaded;
		entry.size = info.size;
		m_mods.push_back(entry);

		wstring label = entry.name;
		if (!entry.version.empty())
		{
			label += L" v" + entry.version;
		}
		if (!entry.loaded)
		{
			label += L" (not loaded)";
		}

		m_buttonListMods.addItem(label, wstring(L""));
	}

	app.DebugPrintf("ModsMenu - found %d mod%s in %ls\n", (int)m_mods.size(), (m_mods.size() == 1) ? "" : "s", folder.c_str());

	m_controlModsTimer.setVisible(false);
	m_labelNoMods.setVisible(m_buttonListMods.getItemCount() == 0);

	displayModInfo(m_mods.empty() ? -1 : 0);
}

void UIScene_ModsMenu::displayModInfo(int listIndex)
{
	if (listIndex < 0 || listIndex >= static_cast<int>(m_mods.size()))
	{
		m_labelModName.setLabel(UIString(IDS_MOD_NAME));
		m_labelModSize.setLabel(UIString(IDS_MOD_SIZE));
		return;
	}

	const ModEntry &mod = m_mods[listIndex];

	wstring nameText = mod.name;
	wstring versionText = mod.version;
	wstring idText = mod.modId;
	wstring sizeText = formatFileSize(mod.size);
	wstring statusText = mod.loaded ? L"Loaded" : (mod.error);

	m_labelModName.setLabel( UIString( wstring( app.GetString( IDS_MOD_NAME ) ) + nameText ) );
	m_labelModID.setLabel( UIString( wstring( app.GetString( IDS_MOD_ID ) ) + idText ) );
	m_labelModVersion.setLabel( UIString( wstring( app.GetString( IDS_MOD_VERSION ) ) + versionText ) );
	m_labelModSize.setLabel( UIString( wstring( app.GetString( IDS_MOD_SIZE ) ) + sizeText ) );
	m_labelModStatus.setLabel( UIString( wstring( app.GetString( IDS_MOD_STATUS ) ) + statusText ) );
}

void UIScene_ModsMenu::openModsFolder()
{	// Direct file management, to make adding mods simple
	wstring folder = createModsFolder();

	WCHAR fullPath[MAX_PATH];
	DWORD length = GetFullPathNameW(folder.c_str(), MAX_PATH, fullPath, nullptr);
	HINSTANCE result = ShellExecuteW(nullptr, L"open", fullPath, nullptr, nullptr, SW_SHOWNORMAL);

	if (length == 0 || length >= MAX_PATH)
	{
		return;
	}

	if (reinterpret_cast<INT_PTR>(result) <= 32)
	{
		app.DebugPrintf("couldnt open mods folder </3\n", fullPath, static_cast<int>(reinterpret_cast<INT_PTR>(result)));
	}
}

void UIScene_ModsMenu::updateTooltips()
{
	ui.SetTooltips( DEFAULT_XUI_MENU_USER, IDS_TOOLTIPS_SELECT, IDS_TOOLTIPS_BACK );
}

void UIScene_ModsMenu::updateComponents()
{
	m_parentLayer->showComponent(m_iPad, eUIComponent_Panorama, true);
	m_parentLayer->showComponent(m_iPad, eUIComponent_Logo, false);
	m_parentLayer->showComponent(m_iPad, eUIComponent_MenuBackground, true);
}

void UIScene_ModsMenu::handleGainFocus(bool navBack)
{
	UIScene::handleGainFocus(navBack);
	SetFocusToElement(eControl_ModFolderButton);
	updateTooltips();
	replaceTheIcon();

	if (navBack)
	{
		refreshModsList();
	}
}

void UIScene_ModsMenu::tick()
{
	UIScene::tick();

	switch (m_iState)
	{
	case e_ListIdle:
		break;
	case e_ListRepopulate:
		m_iState = e_ListIdle;
		refreshModsList();
		break;
	}
}

void UIScene_ModsMenu::handleInitFocus(F64 controlId, F64 childId)
{
	app.DebugPrintf(app.USER_SR, "UIScene_ModsMenu::handleInitFocus - %d , %d\n", static_cast<int>(controlId), static_cast<int>(childId));

	if (static_cast<int>(controlId) == eControl_ModsList)
	{
		m_iModListIndex = static_cast<int>(childId);
		displayModInfo(m_iModListIndex);
	}
}

void UIScene_ModsMenu::handleFocusChange(F64 controlId, F64 childId)
{
	app.DebugPrintf(app.USER_SR, "UIScene_ModsMenu::handleFocusChange - %d , %d\n", static_cast<int>(controlId), static_cast<int>(childId));

	switch (static_cast<int>(controlId))
	{
	case eControl_ModsList:
		m_iModListIndex = static_cast<int>(childId);
		displayModInfo(m_iModListIndex);
		break;
	};

	updateTooltips();
}

void UIScene_ModsMenu::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
{
	ui.AnimateKeyPress(m_iPad, key, repeat, pressed, released);

	switch (key)
	{
	case ACTION_MENU_CANCEL:
		if (pressed)
		{
			navigateBack();
			handled = true;
		}
		break;

	case ACTION_MENU_OK:
		if (pressed && !repeat && controlHasFocus(eControl_ModFolderButton))
		{
			m_bFlashReportedFolderPress = false;

			sendInputToMovie(key, repeat, pressed, released);

			if (m_bFlashReportedFolderPress)
			{
				app.DebugPrintf("ModsMenu - mods folder button press reported by the swf\n");
			}
			else
			{
				app.DebugPrintf("ModsMenu - swf did not report the mods folder button press, handling it here\n");
				handlePress(eControl_ModFolderButton, 0);
			}

			handled = true;
			break;
		}
		sendInputToMovie(key, repeat, pressed, released);
		handled = true;
		break;

	case ACTION_MENU_UP:
	case ACTION_MENU_DOWN:
	case ACTION_MENU_PAGEUP:
	case ACTION_MENU_PAGEDOWN:
		sendInputToMovie(key, repeat, pressed, released);
		handled = true;
		break;
	case ACTION_MENU_OTHER_STICK_UP:
		sendInputToMovie(ACTION_MENU_UP, repeat, pressed, released);
		handled = true;
		break;
	case ACTION_MENU_OTHER_STICK_DOWN:
		sendInputToMovie(ACTION_MENU_DOWN, repeat, pressed, released);
		handled = true;
		break;
	}
}

void UIScene_ModsMenu::handlePress(F64 controlId, F64 childId)
{
	switch (static_cast<int>(controlId))
	{
	case eControl_ModsList:
		{
			int listIndex = static_cast<int>(childId);
			
			ui.PlayUISFX(eSFX_Press);

			if (listIndex >= 0 && listIndex < static_cast<int>(m_mods.size()))
			{
				app.DebugPrintf("selected mod '%ls'\n", m_mods[listIndex].name.c_str());
			}
		}
		break;

	case eControl_ModFolderButton:
		m_bFlashReportedFolderPress = true;
		ui.PlayUISFX(eSFX_Press);
		openModsFolder();
		break;
	}
}
#endif
