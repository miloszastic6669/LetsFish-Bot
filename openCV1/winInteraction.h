#pragma once
#include <thread>
#include <Windows.h>
#include "functions.h"

struct Coord;


void SetCursorPos(const Coord& pos);
void ClickMouseLeft();
void MouseLeftDown();
void MouseLeftUp();
void MouseMoveClick(const Coord& pos);
void MouseMoveClick(int& x, int& y);
void PressKey(const char* key);
void PressKey(std::string& key);
void PressKey(char key);
void PressKeyNew(char key);