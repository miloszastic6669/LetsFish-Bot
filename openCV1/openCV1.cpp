#pragma once
#include "functions.h"
#include "gameInteraction.h"
#include "gameWindow.h"
#include "serialization.h"
#include "winInteraction.h"
#include "bot.h"



std::atomic<bool> stop_running = false;

int main()
{
	// getting/setting coordinate data
	GameWindow windowData;
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetDC(NULL);
	
	const int refreshTime_sec = 600; //10 minutes

	std::thread panic_thread(exit_thread);
	std::thread lvl_up_thread(lvlup_thread, &windowData, &stop_running);
	//using multithreading
	int choice; 
	while(true)
	{
		std::cout << "1. New config\n";
		//std::cout << "3. Debug\n";
		std::cout << "2. Start bot\n";
		std::cin >> choice;
		std::cin.ignore();
		int rodType;
		std::atomic<bool> stop_running = false;

		switch (choice)
		{
		case 1:
			CreateNewConfig();
			break;

		case 2:
			std::cout << "what rod are you using?\n";
			std::cout << "1. spin or sea rod\n2. normal rod\n";
			std::cin >> rodType;

			//setup window data and game interaction
			LoadConfig(windowData);
			windowData.calculateAll();
			gameInteraction game;
			game.setGameInfo(&windowData);
			//setup bot stopping variable

			while(true)
			{
				stop_running = false;
				std::cout << "starting new main thread...\n";
				std::thread main_thread(mainBot, &windowData, &game, &hwnd, &hdc, rodType, &stop_running);
				
				sleepFor(refreshTime_sec, 100, &stop_running ,&windowData);
				stop_running = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				main_thread.join();

				std::cout << "refreshing page...\n";
				game.refreshPage(10);

				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
			break;
		
		/*case 3:  //may need that soon...
			debug(&windowData);
			break;*/
		default:

			std::cout << "Are you stupid or something?\n";
			break;
		}

	}

	panic_thread.join();// to wait until the thread is finished running
	return 42069420;
}