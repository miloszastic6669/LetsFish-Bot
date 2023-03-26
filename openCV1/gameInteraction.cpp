#include "gameInteraction.h"

void gameInteraction::calculate()
{
	game->calculateAll();
}

void gameInteraction::setGameInfo(GameWindow* c)
{
	game = c;
}

void gameInteraction::refreshPage()
{
	PressKeyNew(0x74);
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetDC(NULL);
	std::this_thread::sleep_for(std::chrono::seconds(10));
	Coord pos;
	pos = game->getCloseAds();
	while(!game->canCast(&hwnd))
	{
		MouseMoveClick(pos);
		std::cout << "pressing esc...\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	ReleaseDC(hwnd, hdc);
}

void gameInteraction::cast()
{
	MouseMoveClick(game->getPullButton());
}

void gameInteraction::pull(HWND* hwnd, HDC* hdc)
{
	COLORREF colorBar = GetPixel(hdc, game->getGreenBar());
	
	bool press = true;
	
	MouseMoveClick(game->getPullButton());
	while(game->isCatching(hdc, hwnd))
	{
		//update color
		colorBar = GetPixel(hdc, game->getGreenBar());

		//quit button
		if (GetAsyncKeyState(GetKeyCode("Q")) & 1)
			return;

		//check whether press or release
		//rgb press: (61,99,135)
		if (IsSimilarColor(colorBar, 61, 99, 135, 1))
			press = true;
		else
			press = false;

		if (press)
			MouseLeftDown();
		else
			MouseLeftUp();
	}
}

void gameInteraction::pullSpin(HWND* hwnd, HDC* hdc)
{
	bool press = true;

	MouseMoveClick(game->getPullButton());
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	while (true)
	{
		//pull whenever can pull
		if (game->canPull(hwnd))
		{
			MouseLeftDown();
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
				if (game->isCatchingSpin(hdc, hwnd)) // start pulling whenever catching
				{
					std::cout << "started catching\n";
					pull(hwnd, hdc);
					return;
				}
				if(!game->canPull(hwnd))
				{
					MouseLeftUp();
					break;
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	caught:;

}

void gameInteraction::escape()
{
	PressKeyNew(VK_ESCAPE);
}

