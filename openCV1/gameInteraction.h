#pragma once
#include "winInteraction.h"
#include "gameWindow.h"


class gameInteraction
{
private:
	GameWindow* game;
public:
	void calculate();
	void setGameInfo(GameWindow* c);
	void refreshPage(int delay_sec); // refresh, wait and close ads until can cast
	void cast(); //press cast/pull button
	void pull(HWND* hwnd, HDC* hdc, std::atomic<bool>* stop_running); //pull and dont let the fish get away
	void pullSpin(HWND* hwnd, HDC* hdc, std::atomic<bool>* stop_running);
	void escape();
};

