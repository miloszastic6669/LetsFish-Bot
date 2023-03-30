#include "bot.h"
#include "functions.h"
#include "gameInteraction.h"
#include "gameWindow.h"
#include "serialization.h"
#include "winInteraction.h"

#define SPIN_ROD 1
#define NORMAL_ROD 2

int debug(GameWindow* windowData)
{
	std::cout << "Debug...\n";
	HWND hwnd = GetDesktopWindow();
	HDC dc = GetDC(NULL);
	cv::Mat castImage, pullImage, gameImage;
	LoadConfig(*windowData);
	windowData->calculateAll();
	bool left = false;

	gameInteraction c;
	c.setGameInfo(windowData);

	while (true)
	{
		if (GetAsyncKeyState(GetKeyCode("W")) & 1)
		{
			std::cout << "refreshing page\n";
			c.refreshPage(10);
			/*if (windowData.canCast(&hwnd))
			{
				std::cout << "Can Cast\n";
			}
			else
			{
				std::cout << "Can not Cast\n";
			}

			if(windowData.canPull(&hwnd))
			{
				std::cout << "Can Pull\n";
			}
			else
			{
				std::cout << "Can not Pull\n";
			}*/
		}

		if (GetAsyncKeyState(GetKeyCode("Q")) & 1)
		{
			std::cout << "peace out\n";
			return 0;
		}
		Sleep(10);
	}
	return -69;
}

int mainBot(GameWindow* windowData, gameInteraction* c, HWND* hwnd, HDC* hdc, int rodType, std::atomic<bool>* stop_running)
{
	switch (rodType)
	{
	case SPIN_ROD:
		while (true)
		{
			if (*stop_running)
				return -80083;
			for (int i = 0; i < 10; i++)
			{
				c->escape(); // in case of a level up
				if(*stop_running)
					return -80083;
			}

			
			while (!windowData->canPull(hwnd))
			{
				if (*stop_running)
					return -80083;
				
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				
				if (windowData->canCast(hwnd))//in case the first cast call missed 
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
					c->cast();
				}

			}
			if (*stop_running)
				return -80083;
			std::cout << "can pull\n";
			c->pullSpin(hwnd, hdc, stop_running);

			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			
			c->escape();
		}
		break;
	case NORMAL_ROD:
		while (true)
		{
			
			if (*stop_running)
				return -80083;

			if (windowData->canCast(hwnd))
				c->cast();
			while (!windowData->canPull(hwnd))
			{
				
				if (*stop_running)
					return -80083;

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				if (windowData->canCast(hwnd))//in case the first cast call missed
					c->cast();
			}
			std::cout << "can pull\n";
			c->pull(hwnd, hdc, stop_running);
			std::this_thread::sleep_for(std::chrono::seconds(1));
			c->escape();
		}
		break;
	default:
		std::cout << "wrong rod type!\n";
		break;
	}

	return 69420;
}