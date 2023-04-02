#pragma once

#include <iostream>
#include <Windows.h>
#include <conio.h>

#include "functions.h"

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

struct Coord
{
	Coord() { x = 0; y = 0; }
	Coord(int x1, int y1) { x = x1; y = y1; }
	
	int x;
	int y;
	
	const Coord& operator+(const Coord& rhs)
	{
		return Coord(x + rhs.x, y + rhs.y);
	}
};

struct Ratio
{
	Ratio() { x = 0; y = 0; }
	Ratio(float x1, float y1) { x = x1; y = y1; }
	float x;
	float y;
	Ratio& operator=(const Ratio& rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}
};

class GameWindow
{
private:
	Coord upperLeft;
	Coord bottomRight;
	Coord pullButton;
	Coord exitCaughtButton;
	Coord greenBar;
	Coord greenBarRight;
	Coord closeAds;

	//ratios
	const Ratio exitCaughtRatio = { 0.9054505005561735f, 0.20170109356014582f };
	const Ratio pullButtonRatio = { 0.5127919911012235f, 0.7885783718104495f };
	const Ratio greenBarRatio = { 0.5116796440489433f, 0.38760631834750914f };	//in the middle of the bar
	const Ratio greenBarRightRatio = { 0.6840934371523916f, 0.4993924665856622f }; //----has rgb (61,99,135) when pulling----
	const Ratio closeAdRatio = { 0.9645232815964523f, 0.1709090909090909f };

	//rectangle Ratios
	const Ratio castRectULRatio = { 0.42157953281423804f, 0.7521263669501823f };
	const Ratio castRectBRRatio = { 0.4938820912124583f, 0.8080194410692588f };

	const Ratio pullRectULRatio = { 0.489432703003337f, 0.7630619684082625f };
	const Ratio pullRectBRRatio = { 0.5450500556173526f, 0.7934386391251519f };

	const Ratio caughtRectULRatio = { 0.7195121951219512f, 0.713939393939394f };
	const Ratio caughtRectBRRatio = { 0.8126385809312638f, 0.793939393939394f };

	const Ratio failedRectULRatio = { 0.27605321507760533f, 0.6509090909090909f };
	const Ratio failedRectBRRatio = { 0.44124168514412415f, 0.7175757575757575f };

	const Ratio adImgULRatio = { 0.39356984478935697f, 0.0048484848484848485f };
	const Ratio adImgBRRatio = { 0.4623059866962306f, 0.044848484848484846f };

	const Ratio lvlUpULRatio = { 0.4124168514412417f, 0.14909090909090908f };
	const Ratio lvlUpBRRatio = { 0.5266075388026608f, 0.18424242424242424f };

	
	//needle images taken on chrome zoom 125% 2560x1440 Monitor
	cv::Mat castImg = cv::imread("assets/cast.jpg");
	cv::Mat pullImg = cv::imread("assets/pull.jpg");
	cv::Mat caughtImg = cv::imread("assets/caught.jpg");
	cv::Mat failedImg = cv::imread("assets/failed.jpg");
	cv::Mat adImg = cv::imread("assets/ad.jpg"); // not working
	cv::Mat lvlUpImg = cv::imread("assets/lvlup.jpg");

	void calcPullButton();
	void calcExitCaughtButton();
	void calcGreenBar();
	void calcGreenBarRight();
	void calcCloseAds();
public:
	void setUpperLeft(Coord c);
	void setBottomRight(Coord c);
	void setPullButton(Coord c);
	void setExitCaughtButton(Coord c);
	void calculateAll();

	Coord getUpperLeft() const;
	Coord getBottomRight() const;
	Coord getPullButton() const;
	Coord getExitCaughtButton() const;
	Coord getGreenBar() const;
	Coord getGreenBarRight() const;
	Coord getCloseAds() const;
	int getGameWindowHeight() const;
	int getGameWindowWidth() const;
	
	cv::Mat getCastImg(HWND hwnd);
	cv::Mat getPullImg(HWND hwnd);

	Coord RatioToCoord(const Ratio& x);

	bool canCast(HWND* hwnd);
	bool canPull(HWND* hwnd);
	bool canCloseAds(HWND* hwnd);
	bool isCatching(HDC* hdc, HWND* hwnd);
	bool isCatchingSpin(HDC* hdc, HWND* hwnd);
	bool isOnCaughtScreen(HWND* hwnd);
	bool isOnFailedScreen(HWND* hwnd);
	bool isOnLvlUpScreen(HWND* hwnd);

	GameWindow& operator=(const GameWindow& rhs)
	{
		return *this;
	}
};

