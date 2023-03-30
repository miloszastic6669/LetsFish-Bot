#include "winInteraction.h"
#include "gameWindow.h"

void SetCursorPos(const Coord& pos)
{
	SetCursorPos(pos.x, pos.y);
}

void ClickMouseLeft()
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;

	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(INPUT));
	std::this_thread::sleep_for(std::chrono::milliseconds(30));

	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));
}

void MouseLeftDown()
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;

	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(INPUT));
}

void MouseLeftUp()
{
	INPUT input{ 0 };
	input.type = INPUT_MOUSE;

	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(INPUT));
}

void MouseMoveClick(const Coord& pos)
{
	SetCursorPos(pos);
	
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
	ClickMouseLeft();
}

void MouseMoveClick(int& x, int& y)
{
	Coord pos(x, y);
	MouseMoveClick(pos);
}

void PressKey(const char* key)
{
	std::string s = key;
	PressKey(s);
}

void PressKey(std::string& key)
{
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = GetKeyCode(key);
	SendInput(1, &input, sizeof(INPUT));
}

void PressKey(char key) {
	INPUT input = {};
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VkKeyScan(key);
	input.ki.dwFlags = 0;

	SendInput(1, &input, sizeof(INPUT));
}

void PressKeyNew(char key)//works more often than PressKey()
{
	UINT mappedkey;
	INPUT input = { 0 };
	mappedkey = MapVirtualKey(LOBYTE(key), 0);
	input.type = INPUT_KEYBOARD;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	input.ki.wScan = mappedkey;
	SendInput(1, &input, sizeof(input));
	Sleep(10);
	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}
