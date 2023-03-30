#pragma once
#include <Windows.h>
#include <thread>
#include <atomic>
class GameWindow;
class gameInteraction;


int debug(GameWindow* windowData);

int mainBot(GameWindow* windowData, gameInteraction* c, HWND* hwnd, HDC* hdc, int rodType, std::atomic<bool>* stop_running);
