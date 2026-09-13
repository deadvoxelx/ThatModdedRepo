#pragma once

#include "UIScene.h"

#ifdef _WINDOWS64
class UIScene_ModsMenu : public UIScene
{
private:
	enum EControls
	{
		eControl_ModsList,
		eControl_ModFolderButton,
	};

	enum EState
	{
		e_ListIdle,
		e_ListRepopulate,
	};

	struct ModEntry
	{
		wstring name;
		wstring version;
		wstring modId;
		wstring error;
		bool loaded;
		uint64_t size;
	};

protected:
	UIControl_SaveList m_buttonListMods;
	UIControl_Button m_buttonModFolder;
	UIControl_Label m_labelModsListTitle, m_labelNoMods, m_labelModName, m_labelModID, m_labelModVersion, m_labelModSize, m_labelModStatus, m_labelRubyLauncher;
	UIControl m_controlModsTimer;
	UIControl_BitmapIcon m_icon;

private:
	UI_BEGIN_MAP_ELEMENTS_AND_NAMES(UIScene)
		UI_MAP_ELEMENT( m_buttonListMods, "SavesList")
		UI_MAP_ELEMENT( m_labelModsListTitle, "SavesListTitle")
		UI_MAP_ELEMENT( m_labelNoMods, "NoGames")
		UI_MAP_ELEMENT( m_labelModName, "ModName")
		UI_MAP_ELEMENT( m_labelModID, "ModID")
		UI_MAP_ELEMENT( m_labelModVersion, "ModVersion")
		UI_MAP_ELEMENT( m_labelModSize, "ModSize")
		UI_MAP_ELEMENT( m_labelModStatus, "ModStatus")
		UI_MAP_ELEMENT( m_labelRubyLauncher, "RubyLauncher")
		UI_MAP_ELEMENT( m_controlModsTimer, "SavesTimer")
		UI_MAP_ELEMENT( m_buttonModFolder, "Button1")
		UI_MAP_ELEMENT( m_icon, "MinecraftIcon")
	UI_END_MAP_ELEMENTS_AND_NAMES()

	int m_iState;
	int m_iModListIndex;
	bool m_bFlashReportedFolderPress;
	vector<ModEntry> m_mods;

public:
	UIScene_ModsMenu(int iPad, void *initData, UILayer *parentLayer);
	virtual ~UIScene_ModsMenu();

	virtual void updateTooltips();
	virtual void updateComponents();
	virtual void handleGainFocus(bool navBack);

	virtual void handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled);
	virtual void handleFocusChange(F64 controlId, F64 childId);
	virtual void handleInitFocus(F64 controlId, F64 childId);
	virtual void handlePress(F64 controlId, F64 childId);

	virtual EUIScene getSceneType() { return eUIScene_ModsMenu; }

	virtual void tick();

protected:
	virtual wstring getMoviePath();

	void refreshModsList();
	void displayModInfo(int listIndex);
	void openModsFolder();
	void replaceTheIcon();
};
#endif
