#pragma once

#ifdef _WINDOWS64

#include <windows.h>

enum KBM_ACTION
{
	KBM_ACTION_ATTACK = 0,
	KBM_ACTION_USE,
	KBM_ACTION_PICK_ITEM,
	KBM_ACTION_FORWARD,
	KBM_ACTION_BACK,
	KBM_ACTION_LEFT,
	KBM_ACTION_RIGHT,
	KBM_ACTION_JUMP,
	KBM_ACTION_SNEAK,
	KBM_ACTION_SNEAK_ALT,
	KBM_ACTION_SPRINT,
	KBM_ACTION_DASH,
	KBM_ACTION_INVENTORY,
	KBM_ACTION_DROP,
	KBM_ACTION_CHAT,
	KBM_ACTION_COORDS,
	KBM_ACTION_FULLBRIGHT,
	KBM_ACTION_ZOOM,
	KBM_ACTION_COUNT
};

class KeyboardMouseInput
{
public:
	static const int MAX_KEYS = 256;

	static const int MOUSE_LEFT = 0;
	static const int MOUSE_RIGHT = 1;
	static const int MOUSE_MIDDLE = 2;
	static const int MAX_MOUSE_BUTTONS = 3;

	static const int KEY_FORWARD = 'W';
	static const int KEY_BACKWARD = 'S';
	static const int KEY_LEFT = 'A';
	static const int KEY_RIGHT = 'D';
	static const int KEY_JUMP = VK_SPACE;
	static const int KEY_SNEAK = VK_LSHIFT;
	static const int KEY_SNEAK_ALT = VK_CAPITAL;
	static const int KEY_SPRINT = VK_LCONTROL;
	static const int KEY_INVENTORY = 'E';
	static const int KEY_DROP = 'B';
	static const int KEY_CRAFTING = 'C';
	static const int KEY_CRAFTING_ALT = 'R';
	static const int KEY_CHAT = 'T';
	static const int KEY_CHAT_ALT = VK_OEM_2;
	static const int KEY_ZOOM = 'V';
	static const int KEY_FULLBRIGHT = 'F';
	static const int KEY_HIDE_COORDS = 'H';
	static const int KEY_DASH = VK_TAB;
	static const int KEY_CONFIRM = VK_RETURN;
	static const int KEY_CANCEL = VK_ESCAPE;
	static const int KEY_PAUSE = VK_ESCAPE;
	static const int KEY_TOGGLE_HUD = VK_F1;
	static const int KEY_SCREENSHOT = VK_F2;
	static const int KEY_DEBUG_INFO = VK_F3;
	static const int KEY_DEBUG_MENU = VK_F4;
	static const int KEY_THIRD_PERSON = VK_F5;
	static const int KEY_DEBUG_CONSOLE = VK_F6;
	static const int KEY_HOST_SETTINGS = VK_OEM_3;
	static const int KEY_FULLSCREEN = VK_F11;

	void Init();
	void Tick();
	void ClearAllState();

	void OnKeyDown(int vkCode);
	void OnKeyUp(int vkCode);
	void OnMouseButtonDown(int button);
	void OnMouseButtonUp(int button);
	void OnMouseMove(int x, int y);
	void OnMouseWheel(int delta);
	void OnRawMouseDelta(int dx, int dy);

	bool IsKeyDown(int vkCode) const;
	bool IsKeyPressed(int vkCode) const;
	bool IsKeyReleased(int vkCode) const;

	void ConsumeKey(int vkCode);

	int GetPressedKey() const;

	void ConsumeMouseButtons();
	void SetKeyBinding(int action, int key);
	int GetKeyBinding(int action) const;
	bool IsActionDown(int action) const;
	bool IsActionPressed(int action) const;
	bool IsActionReleased(int action) const;

	bool IsMouseButtonDown(int button) const;
	bool IsMouseButtonPressed(int button) const;
	bool IsMouseButtonReleased(int button) const;

	int GetMouseX() const { return m_mouseX; }
	int GetMouseY() const { return m_mouseY; }

	int GetMouseDeltaX() const { return m_mouseDeltaX; }
	int GetMouseDeltaY() const { return m_mouseDeltaY; }

	int GetMouseWheel();
	int PeekMouseWheel() const { return m_mouseWheelAccum; }
	void ConsumeMouseWheel() { if (m_mouseWheelAccum != 0) m_mouseWheelConsumed = true; m_mouseWheelAccum = 0; }
	bool WasMouseWheelConsumed() const { return m_mouseWheelConsumed; }

	// Per-frame delta consumption for low-latency mouse look.
	// Reads and clears the raw accumulators (not the per-tick snapshot).
	void ConsumeMouseDelta(float &dx, float &dy);

	void SetMouseGrabbed(bool grabbed);
	bool IsMouseGrabbed() const { return m_mouseGrabbed; }

	void SetCursorHiddenForUI(bool hidden);
	bool IsCursorHiddenForUI() const { return m_cursorHiddenForUI; }

	void SetWindowFocused(bool focused);
	bool IsWindowFocused() const { return m_windowFocused; }

	bool HasAnyInput() const { return m_hasInput; }

	void SetKBMActive(bool active) { m_kbmActive = active; }
	bool IsKBMActive() const { return m_kbmActive; }

	void SetScreenCursorHidden(bool hidden) { m_screenWantsCursorHidden = hidden; }
	bool IsScreenCursorHidden() const { return m_screenWantsCursorHidden; }

	// Text input: buffer characters typed while the native keyboard scene is open
	void OnChar(wchar_t c);
	bool ConsumeChar(wchar_t &outChar);
	void ClearCharBuffer();

	float GetMoveX() const;
	float GetMoveY() const;

	float GetLookX(float sensitivity) const;
	float GetLookY(float sensitivity) const;

private:
	int m_keyBindings[KBM_ACTION_COUNT];

	bool m_keyDown[MAX_KEYS];
	bool m_keyDownPrev[MAX_KEYS];

	bool m_keyPressedAccum[MAX_KEYS];
	bool m_keyReleasedAccum[MAX_KEYS];
	bool m_keyPressed[MAX_KEYS];
	bool m_keyReleased[MAX_KEYS];

	bool m_mouseButtonDown[MAX_MOUSE_BUTTONS];
	bool m_mouseButtonDownPrev[MAX_MOUSE_BUTTONS];

	bool m_mouseBtnPressedAccum[MAX_MOUSE_BUTTONS];
	bool m_mouseBtnReleasedAccum[MAX_MOUSE_BUTTONS];
	bool m_mouseBtnPressed[MAX_MOUSE_BUTTONS];
	bool m_mouseBtnReleased[MAX_MOUSE_BUTTONS];

	int m_mouseX;
	int m_mouseY;

	int m_mouseDeltaX;
	int m_mouseDeltaY;
	int m_mouseDeltaAccumX;
	int m_mouseDeltaAccumY;

	int m_mouseWheelAccum;
	bool m_mouseWheelConsumed;

	bool m_mouseGrabbed;

	bool m_cursorHiddenForUI;

	bool m_windowFocused;

	bool m_hasInput;

	bool m_kbmActive;

	bool m_screenWantsCursorHidden;

	static const int CHAR_BUFFER_SIZE = 32;
	wchar_t m_charBuffer[CHAR_BUFFER_SIZE];
	int m_charBufferHead;
	int m_charBufferTail;
};

extern KeyboardMouseInput g_KBMInput;

#endif
