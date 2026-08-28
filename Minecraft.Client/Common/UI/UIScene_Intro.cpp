#include "stdafx.h"
#include "UI.h"
#include "UIScene_Intro.h"


UIScene_Intro::UIScene_Intro(int iPad, void *initData, UILayer *parentLayer) : UIScene(iPad, parentLayer)
{
	// Setup all the Iggy references we need for this scene
	initialiseMovie();
	m_bIgnoreNavigate = false;
	m_bAnimationEnded = false;

	bool bSkipESRB = false;
	bool bChina = false;
#if defined(__PS3__) || defined(__ORBIS__) || defined(__PSVITA__)
	bSkipESRB = app.GetProductSKU() != e_sku_SCEA;
#elif defined(_XBOX) || defined(_DURANGO)
	bSkipESRB = !ProfileManager.LocaleIsUSorCanada();
#endif

#ifdef _DURANGO
	bChina = ProfileManager.LocaleIsChina();
#endif
	// 4J Stu - These map to values in the Actionscript
#ifdef _WINDOWS64
	int platformIdx = 0;
#elif defined(_XBOX)
	int platformIdx = 1;
#elif defined(_DURANGO)
	int platformIdx = 2;
#elif defined(__PS3__)
	int platformIdx = 3;
#elif defined(__ORBIS__)
	int platformIdx = 4;
#elif defined(__PSVITA__)
	int platformIdx = 5;
#endif

	IggyDataValue result;
	IggyDataValue value[3];
	value[0].type = IGGY_DATATYPE_number;
	value[0].number = platformIdx;

	value[1].type = IGGY_DATATYPE_boolean;
	value[1].boolval = bChina?true:bSkipESRB;

	value[2].type = IGGY_DATATYPE_boolean;
	value[2].boolval = bChina;

	IggyResult out = IggyPlayerCallMethodRS ( getMovie() , &result, IggyPlayerRootPath( getMovie() ), m_funcSetIntroPlatform , 3 , value );

#ifdef __PSVITA__
	// initialise vita touch controls with ids
	m_TouchToSkip.init(0);
#endif
}

wstring UIScene_Intro::getMoviePath()
{
	return L"Intro";
}

void UIScene_Intro::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
{
	ui.AnimateKeyPress(m_iPad, key, repeat, pressed, released);

	switch(key)
	{
	case ACTION_MENU_OK:
#ifdef __ORBIS__
	case ACTION_MENU_TOUCHPAD_PRESS:
#endif
		if(!m_bIgnoreNavigate)
		{
			m_bIgnoreNavigate = true;
			ui.NavigateToScene(0,eUIScene_MainMenu);
		}
		break;
	}
}

#ifdef __PSVITA__
void UIScene_Intro::handleTouchInput(unsigned int iPad, S32 x, S32 y, int iId, bool bPressed, bool bRepeat, bool bReleased)
{
	if(bReleased)
	{
		bool handled = false;
		handleInput(iPad, ACTION_MENU_OK, false, true, false, handled);
	}
}
#endif

void UIScene_Intro::handleAnimationEnd()
{
	if(!m_bIgnoreNavigate)
	{
		m_bIgnoreNavigate = true;
		ui.NavigateToScene(0,eUIScene_MainMenu);
	}
}

void UIScene_Intro::handleGainFocus(bool navBack)
{
	if( m_bAnimationEnded )
	{
		ui.NavigateToScene(0,eUIScene_MainMenu);
	}
}
