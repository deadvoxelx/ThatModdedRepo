#include "stdafx.h"
#include "ControlsScreen.h"
#include "Options.h"
#include "SmallButton.h"
#include "..\Minecraft.World\net.minecraft.locale.h"
#ifdef _WINDOWS64
#include "Windows64\KeyboardMouseInput.h"
#endif

extern ConsoleUIController ui;

ControlsScreen::ControlsScreen(Screen *lastScreen, Options *options)
{
	// 4J - added initialisers
	title = L"Controls";
	selectedKey = -1;

	this->lastScreen = lastScreen;
    this->options = options;
}

int ControlsScreen::getLeftScreenPosition()
{
	return width / 2 - 155;
}

void ControlsScreen::init()
{
    Language *language = Language::getInstance();

    int leftPos = getLeftScreenPosition();
    for (int i = 0; i < Options::keyMappings_length; i++)
	{
        buttons.push_back(new SmallButton(i, leftPos + i % 3 * ROW_WIDTH / 1.5 + 15, height / 12 + ROW_HEIGHT * (i / 3), BUTTON_WIDTH, 20, options->getKeyMessage(i)));
    }

    buttons.push_back(new Button(200, width / 2 - 100, height / 6 + 24 * 7, app.GetString(IDS_DONE)));
}

void ControlsScreen::buttonClicked(Button *button)
{
    for (int i = 0; i < Options::keyMappings_length; i++)
	{
        buttons[i]->msg = options->getKeyMessage(i);
    }
    if (button->id == 200)
	{
        //minecraft->setScreen(lastScreen);
        ui.PlayUISFX(eSFX_Back);
        ui.NavigateToScene(ProfileManager.GetPrimaryPad(), eUIScene_HelpAndOptionsMenu);
    }
	else
	{
        selectedKey = button->id;
        button->msg = L"> " + options->getKeyMessage(button->id) + L" <";
        ui.PlayUISFX(eSFX_Press);
    }
}

void ControlsScreen::keyPressed(wchar_t eventCharacter, int eventKey)
{
    if (selectedKey >= 0)
	{
        return;
    }
	else
	{
        Screen::keyPressed(eventCharacter, eventKey);
    }
}

void ControlsScreen::tick()
{
    if (selectedKey >= 0)
	{
#ifdef _WINDOWS64
        int vk = g_KBMInput.GetPressedKey();
        if (vk == VK_ESCAPE)
        {
            buttons[selectedKey]->msg = options->getKeyMessage(selectedKey);
            selectedKey = -1;
        }
        else if (vk != 0)
        {
            options->setKey(selectedKey, vk);
            buttons[selectedKey]->msg = options->getKeyMessage(selectedKey);
            selectedKey = -1;
        }
#endif
    }
    Screen::tick();
}

void ControlsScreen::render(int xm, int ym, float a)
{
    renderBackground();
    renderDirtBackground(0);

    int leftPos = getLeftScreenPosition();
    for (int i = 0; i < Options::keyMappings_length; i++)
	{
        drawString(font, options->getKeyDescription(i), leftPos + i % 3 * ROW_WIDTH / 1.5 + 15, height / 12 + ROW_HEIGHT * (i / 3) - 10, 0xffffffff);
    }

    Screen::render(xm, ym, a);
}
