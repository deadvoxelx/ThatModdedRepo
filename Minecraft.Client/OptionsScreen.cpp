#include "OptionsScreen.h"
#include "ControlsScreen.h"
#include "DirtBackgroundScreen.h"
#include "SlideButton.h"
#include "SmallButton.h"
#include "VideoSettingsScreen.h"
#include "Minecraft.h"
#include "Options.h"
#include "Button.h"
#include "Screen.h"
#include "Tesselator.h"
#include "Textures.h"
#include "..\Minecraft.World\net.minecraft.locale.h"
#include <vector>

extern ConsoleUIController ui;
static OptionsScreen *pendingRestore = nullptr;

OptionsScreen::OptionsScreen(Screen *lastScreen, Options *options)
{
	title = L"Options";

    this->lastScreen = lastScreen;
    this->options = options;
}

void OptionsScreen::init()
{
    this->title = app.GetString(IDS_HELP_AND_OPTIONS);

#ifdef _WINDOWS64
    if (minecraft->level != nullptr)
    {
        ui.showComponent(ProfileManager.GetPrimaryPad(), eUIComponent_Logo, eUILayer_Scene, static_cast<EUIGroup>(ProfileManager.GetPrimaryPad() + 1), false);
    }
#endif

    const Options::Option* items[4] =
    {
        Options::Option::MUSIC,
        Options::Option::SOUND,
        Options::Option::INVERT_MOUSE,
        Options::Option::SENSITIVITY
    };

    for (int i = 0; i < 8; i++)
    {
        int x = width / 2 - 150 - 6 / 2 + i % 2 * (150 + 6);
        int y = height / 5 + 24 * (i >> 1);
        if (i < 4)
        {
            const Options::Option* item = items[i];
            if (item == Options::Option::INVERT_MOUSE)
            {
                buttons.push_back(new Button(SETTINGS_BUTTON_ID, x, y, 150, 20, app.GetString(IDS_SETTINGS)));
            }
            else if (item == Options::Option::SENSITIVITY)
            {
                buttons.push_back(new Button(SENSITIVITY_BUTTON_ID, x, y, 150, 20, app.GetString(IDS_SENSITIVITY)));
            }
            else if (!item->isProgress())
            {
                buttons.push_back(new SmallButton(item->getId(), x, y, item, options->getMessage(item)));
            }
            else
            {
                buttons.push_back(new SlideButton(item->getId(), x, y, item, options->getMessage(item), options->getProgressValue(item)));
            }
        }
        else
        {
            int id;
            int string;
            switch (i)
            {
            case 4:
                id = VIDEO_BUTTON_ID;
                string = IDS_GRAPHICS;
                break;
            case 5:
                id = UI_BUTTON_ID;
                string = IDS_USER_INTERFACE;
                break;
            case 6:
                id = CONTROLS_BUTTON_ID;
                string = IDS_CONTROLS;
                break;
            default:
                id = CREDITS_BUTTON_ID;
                string = IDS_CREDITS;
                break;
            }
            buttons.push_back(new Button(id, x, y, 150, 20, app.GetString(string)));
        }
    }

    buttons.push_back(new Button(200, width / 2 - 100, height / 8 + 24 * 6, app.GetString(IDS_DONE)));
}

void OptionsScreen::buttonClicked(Button *button)
{
    if (!button->active) return;
    if (button->id < 100 && (dynamic_cast<SmallButton*>(button) != nullptr))
    {
        options->toggle(((SmallButton*)button)->getOption(), 1);
        button->msg = options->getMessage(Options::Option::getItem(button->id));
        ui.PlayUISFX(eSFX_Press);
    }
    if (button->id == VIDEO_BUTTON_ID)
    {
        minecraft->options->save();
        pendingRestore = this;
        minecraft->setScreen(new DirtBackgroundScreen());
        ui.NavigateToScene(ProfileManager.GetPrimaryPad(), eUIScene_SettingsGraphicsMenu);
        ui.PlayUISFX(eSFX_Press);
    }
    if (button->id == SETTINGS_BUTTON_ID)
    {
        minecraft->options->save();
        pendingRestore = this;
        minecraft->setScreen(new DirtBackgroundScreen());
        ui.NavigateToScene(ProfileManager.GetPrimaryPad(), eUIScene_SettingsOptionsMenu);
        ui.PlayUISFX(eSFX_Press);
    }
    if (button->id == SENSITIVITY_BUTTON_ID)
    {
        minecraft->options->save();
        pendingRestore = this;
        minecraft->setScreen(new DirtBackgroundScreen());
        ui.NavigateToScene(ProfileManager.GetPrimaryPad(), eUIScene_SettingsControlMenu);
        ui.PlayUISFX(eSFX_Press);
    }
    if (button->id == UI_BUTTON_ID)
    {
        minecraft->options->save();
        pendingRestore = this;
        minecraft->setScreen(new DirtBackgroundScreen());
        ui.NavigateToScene(ProfileManager.GetPrimaryPad(), eUIScene_SettingsUIMenu);
        ui.PlayUISFX(eSFX_Press);
    }
    if (button->id == CREDITS_BUTTON_ID)
    {
        minecraft->options->save();
        pendingRestore = this;
        minecraft->setScreen(new DirtBackgroundScreen());
        ui.NavigateToScene(ProfileManager.GetPrimaryPad(), eUIScene_Credits);
        ui.PlayUISFX(eSFX_Press);
    }
    if (button->id == CONTROLS_BUTTON_ID)
    {
        minecraft->options->save();
        minecraft->setScreen(new ControlsScreen(this, options));
        ui.PlayUISFX(eSFX_Press);
    }
    if (button->id == 200)
    {
        minecraft->options->save();
        minecraft->setScreen(lastScreen);
        ui.PlayUISFX(eSFX_Back);
    }
}

void OptionsScreen::render(int xm, int ym, float a)
{
    renderDirtBackground(0);

    const int px0 = width / 2 - 162;
    const int px1 = width / 2 + 162;
    const int py0 = height / 9;
    const int py1 = height / 6 + 260;

    minecraft->textures->bindTexture(L"gui/settingsBG.png");
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 1, 1, 1);
    Tesselator *t = Tesselator::getInstance();
    t->begin();
    t->color(0xffffff);
    t->vertexUV(static_cast<float>(px0), static_cast<float>(py1), 0.0f, 0.0f, 1.0f);
    t->vertexUV(static_cast<float>(px1), static_cast<float>(py1), 0.0f, 1.0f, 1.0f);
    t->vertexUV(static_cast<float>(px1), static_cast<float>(py0), 0.0f, 1.0f, 0.0f);
    t->vertexUV(static_cast<float>(px0), static_cast<float>(py0), 0.0f, 0.0f, 0.0f);
    t->end();

    font->draw(title, width / 2 - font->width(title) / 2, 35, 0x404040);
    Screen::render(xm, ym, a);
}

void OptionsScreen::checkPendingRestore()
{
    if (pendingRestore == nullptr) return;

    Minecraft *mc = Minecraft::GetInstance();
    if (mc == nullptr)
    {
        pendingRestore = nullptr;
        return;
    }

    if (ui.GetTopScene(ProfileManager.GetPrimaryPad()) != nullptr) return;

    OptionsScreen *restore = pendingRestore;
    pendingRestore = nullptr;
    mc->setScreen(restore);
}
