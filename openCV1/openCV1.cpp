#pragma once
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

	while(true)
	{
		
		
		if (GetAsyncKeyState(GetKeyCode("W")) & 1)
		{
			std::cout << "refreshing page\n";
			c.refreshPage();
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

int mainBot(GameWindow* windowData, int rodType)
{
	//handles
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetDC(NULL);

	//setup window data and game interaction
	LoadConfig(*windowData);
	windowData->calculateAll();
	gameInteraction c;
	c.setGameInfo(windowData);
	
	const double refreshTime_sec = 1200;//refresh every 20 minutes	
	auto start = std::chrono::high_resolution_clock::now();
	
	switch (rodType) 
	{
	case SPIN_ROD:
		while (true)
		{

			//-----------------------------------------------------------
			//refresh page every 5 minutes
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
			auto timeToRefresh = refreshTime_sec - elapsedTime;
			std::cout << "time to refresh:" << timeToRefresh << " seconds...\n";
			if (elapsedTime > refreshTime_sec)
			{
				start = std::chrono::high_resolution_clock::now();
				std::cout << "refreshing page\n";
				c.refreshPage();

			}
			//-----------------------------------------------------------

			for(int i = 0; i < 10; i++)
			{
				c.escape(); // for safety;
			}

			if (GetAsyncKeyState(GetKeyCode("Q")) & 1)
				return -80083;
			if (windowData->canCast(&hwnd))
				c.cast();
			while (!windowData->canPull(&hwnd))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				if (windowData->canCast(&hwnd))//in case the first cast call missed 
					c.cast();

				if (GetAsyncKeyState(GetKeyCode("Q")) & 1)
					return -80083;
			}
			std::cout << "can pull\n";
			c.pullSpin(&hwnd, &hdc);

			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			if (windowData->isOnCaughtScreen(&hwnd))
				std::cout << "gotcha bitch\n";
			if (windowData->isOnFailedScreen(&hwnd))
				std::cout << "mission failed\n";
			std::cout << "caught i guess...\n";
			c.escape();
		}
		ReleaseDC(hwnd, hdc);
		break;
	case NORMAL_ROD: 
		while (true)
		{
			//-----------------------------------------------------------
			//refresh page every 5 minutes
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
			auto timeToRefresh = refreshTime_sec - elapsedTime;
			std::cout << "time to refresh:" << timeToRefresh << " seconds...\n";
			if (elapsedTime > refreshTime_sec)
			{
				start = std::chrono::high_resolution_clock::now();
				std::cout << "refreshing page\n";
				c.refreshPage();

			}
			//-----------------------------------------------------------
			
			
			if (GetAsyncKeyState(GetKeyCode("Q")) & 1)
				return -80083;
			if (windowData->canCast(&hwnd))
				c.cast();
			while (!windowData->canPull(&hwnd))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				if (windowData->canCast(&hwnd))//in case the first cast call missed
					c.cast();
			}
			std::cout << "can pull\n";
			c.pull(&hwnd, &hdc);
			if (windowData->isOnCaughtScreen(&hwnd))
				std::cout << "gotcha bitch\n";
			if (windowData->isOnFailedScreen(&hwnd))
				std::cout << "mission failed\n";
			c.escape();
		}
		ReleaseDC(hwnd, hdc);
		break;
	default:
		std::cout << "wrong rod type!\n";
		break;
	}

	return 69420;
}

int main()
{
	// getting/setting coordinate data
	GameWindow windowData;
	cv::Mat image;
	HWND hwnd = GetDesktopWindow();
	Coord cock;
	HDC hdc = GetDC(NULL);
	while(true)
	{
		std::cout << "1. New config\n";
		std::cout << "2. Load existing config\n";
		std::cout << "3. Debug\n";
		std::cout << "4. Start bot\n";
		int choice;
		std::cin >> choice;
		

		int rodType;
		switch(choice)
		{
		case 1:
			CreateNewConfig();
			break;
		case 2:
			if(GetConfigs().size() == 0)
			{
				std::cout << "There are no configs in the root directory!\n";
				break;
			}
			LoadConfig(windowData);
			goto exit_loop;
			
		case 3:
			debug(&windowData);
			break;
		case 4: 
			std::cout << "what rod are you using?\n";
			std::cout << "1. spin or sea rod\n2. normal rod\n";
			std::cin >> rodType;
			mainBot(&windowData, rodType);
			break;
		default:
			std::cout << "Are you stupid or something?\n";
			break;
		}
	}
	exit_loop: 
	


	// grabbing, updating and processing image 
	std::cout << "Press Q to quit\n";
	cv::Mat gameImage;
	while(true)
	{
		gameImage = CaptureScreenMat(hwnd, windowData);
		cv::imshow("cock", gameImage);
		cv::waitKey(dick::fpsToMs(165));
		
		if (GetAsyncKeyState(GetKeyCode("Q")) & 1)
			break;
	}
	
	return 42069420;
}