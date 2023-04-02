#include "gameInteraction.h"

void gameInteraction::calculate()
{
	game->calculateAll();
}

void gameInteraction::setGameInfo(GameWindow* c)
{
	game = c;
}

void gameInteraction::refreshPage(int delay_sec)
{
	PressKeyNew(0x74);
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetDC(NULL);
	std::this_thread::sleep_for(std::chrono::seconds(delay_sec));
	Coord pos;
	pos = game->getCloseAds();
	int timesClicked = 0;
	std::cout << "\npressing esc ->";
	while(!game->canCast(&hwnd))
	{
		MouseMoveClick(pos);
		std::cout << "/";
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		timesClicked++;
		if(timesClicked > 200)//try refreshing again with bigger delay if the game bugged out or refreshed too early
		{
			refreshPage(delay_sec + 5);
		}
	}
	std::cout << "\n";

	ReleaseDC(hwnd, hdc);
}

void gameInteraction::cast()
{
	MouseMoveClick(game->getPullButton());
}

void gameInteraction::pull(HWND* hwnd, HDC* hdc, std::atomic<bool>* stop_running)
{
	COLORREF colorBar = GetPixel(hdc, game->getGreenBar());
	
	
	MouseMoveClick(game->getPullButton());
	while(game->isCatching(hdc, hwnd))
	{
		//return if timer runs out
		if (*stop_running)
			return;

		//update color
		colorBar = GetPixel(hdc, game->getGreenBar());

		//check whether press or release
		//rgb press: (61,99,135)
		IsSimilarColor(colorBar, 66, 99, 135, 6) ? MouseLeftDown() : MouseLeftUp();
	}
}

void gameInteraction::pullSpin(HWND* hwnd, HDC* hdc, std::atomic<bool>* stop_running)
{
	bool press = true;

	MouseMoveClick(game->getPullButton());
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	while (true)
	{
		if (*stop_running)
			return;
		//pull whenever can pull
		if (game->canPull(hwnd))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(150));
			MouseLeftDown();
			
			while (true)
			{
				if (*stop_running)
					return;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
				if (game->isCatchingSpin(hdc, hwnd)) // start pulling whenever catching
				{
					if (*stop_running)
						return;
					std::cout << "started catching\n";
					pull(hwnd, hdc, stop_running);
					return;
				}
				if(!game->canPull(hwnd))
				{
					if (*stop_running)
						return;
					MouseLeftUp();
					break;
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

}

void gameInteraction::escape()
{
	PressKeyNew(VK_ESCAPE);
}

