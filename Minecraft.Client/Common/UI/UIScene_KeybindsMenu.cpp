#include "stdafx.h"
#include "UI.h"
#include "Screen.h"
#include "UIScene_KeybindsMenu.h"
#include "..\..\Options.h"
#include "..\..\Minecraft.h"
#ifdef _WINDOWS64
#include "..\..\Windows64\KeyboardMouseInput.h"
#endif

static_assert(KEYBINDS_BUTTON_COUNT == Options::keyMappings_length, "KeybindsLength");

UIScene_KeybindsMenu::UIScene_KeybindsMenu(int iPad, void *initData, UILayer *parentLayer) : UIScene(iPad, parentLayer)
{
	initialiseMovie();

	selectedKey = -1;
	m_waitForMouseRelease = false;

	Options *options = Minecraft::GetInstance()->options;

	for (int i = 0; i < Options::keyMappings_length; i++)
	{
		m_keyButtons[i].init(UIString(options->getKeyDescription(i) + L": " + options->getKeyMessage(i)), i);
	}

	doHorizontalResizeCheck();
}

UIScene_KeybindsMenu::~UIScene_KeybindsMenu()
{
}

wstring UIScene_KeybindsMenu::getMoviePath()
{
	return L"KeybindsMenu";
}

void UIScene_KeybindsMenu::updateTooltips()
{
	ui.SetTooltips( m_iPad, IDS_TOOLTIPS_SELECT,IDS_TOOLTIPS_BACK);
}

void UIScene_KeybindsMenu::updateComponents()
{
	bool bNotInGame=(Minecraft::GetInstance()->level==nullptr);

	m_parentLayer->showComponent(m_iPad,eUIComponent_Logo,false);
	m_parentLayer->showComponent(m_iPad,eUIComponent_MenuBackground,true);
	if(bNotInGame)
	{
		m_parentLayer->showComponent(m_iPad,eUIComponent_Panorama,true);
	}
	else
	{
		m_parentLayer->showComponent(m_iPad,eUIComponent_Panorama,false);
	}
}

void UIScene_KeybindsMenu::handleReload()
{
	doHorizontalResizeCheck();
}

void UIScene_KeybindsMenu::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
{
	if (selectedKey >= 0)
	{
		handled = true;
		return;
	}

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

void UIScene_KeybindsMenu::tick()
{
	UIScene::tick();

#ifdef _WINDOWS64
	Options *options = Minecraft::GetInstance()->options;
	if (selectedKey >= 0)
	{
		int vk = g_KBMInput.GetPressedKey();

		if (m_waitForMouseRelease)
		{
			bool anyDown = false;
			for (int i = 0; i < KeyboardMouseInput::MAX_MOUSE_BUTTONS; i++)
			{
				if (g_KBMInput.IsMouseButtonDown(i)) { anyDown = true; break; }
			}
			if (!anyDown) m_waitForMouseRelease = false;
		}

		if (vk == VK_ESCAPE)
		{
			m_keyButtons[selectedKey].setLabel(UIString(options->getKeyDescription(selectedKey) + L": " + options->getKeyMessage(selectedKey)), true);
			selectedKey = -1;
		}
		else if (vk != 0)
		{
			options->setKey(selectedKey, vk);
			m_keyButtons[selectedKey].setLabel(UIString(options->getKeyDescription(selectedKey) + L": " + options->getKeyMessage(selectedKey)), true);
			selectedKey = -1;
		}
		else if (!m_waitForMouseRelease)
		{
			for (int i = 0; i < KeyboardMouseInput::MAX_MOUSE_BUTTONS; i++)
			{
				if (g_KBMInput.IsMouseButtonPressed(i))
				{
					options->setKey(selectedKey, i);
					m_keyButtons[selectedKey].setLabel(UIString(options->getKeyDescription(selectedKey) + L": " + options->getKeyMessage(selectedKey)), true);
					selectedKey = -1;
					break;
				}
			}
		}
	}
#endif
}

void UIScene_KeybindsMenu::handlePress(F64 controlId, F64 childId)
{
	Options *options = Minecraft::GetInstance()->options;

	if (selectedKey >= 0 && selectedKey < Options::keyMappings_length)
	{
		m_keyButtons[selectedKey].setLabel(UIString(options->getKeyDescription(selectedKey) + L": " + options->getKeyMessage(selectedKey)), true);
		selectedKey = -1;
	}

	int id = static_cast<int>(controlId);
	if (id >= 0 && id < Options::keyMappings_length)
	{
		selectedKey = id;
		m_keyButtons[id].setLabel(UIString(options->getKeyDescription(id) + L": " + L"> " + options->getKeyMessage(id) + L" <"), true);
		ui.PlayUISFX(eSFX_Press);

#ifdef _WINDOWS64
		m_waitForMouseRelease = false;
		for (int i = 0; i < KeyboardMouseInput::MAX_MOUSE_BUTTONS; i++)
		{
			if (g_KBMInput.IsMouseButtonDown(i)) { m_waitForMouseRelease = true; break; }
		}
#endif
	}
}