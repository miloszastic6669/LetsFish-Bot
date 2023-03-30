#include "gameWindow.h"

void GameWindow::setUpperLeft(Coord c) { upperLeft = c; }

void GameWindow::setBottomRight(Coord c) { bottomRight = c; }

void GameWindow::setPullButton(Coord c) { pullButton = c; }

void GameWindow::setExitCaughtButton(Coord c) { exitCaughtButton = c; }

void GameWindow::calcPullButton()
{
	pullButton.x = upperLeft.x + (getGameWindowWidth() * pullButtonRatio.x);
	pullButton.y = upperLeft.y + (getGameWindowHeight() * pullButtonRatio.y);
}

void GameWindow::calcGreenBarRight()
{
	greenBarRight.x = upperLeft.x + (getGameWindowWidth() * greenBarRightRatio.x);
	greenBarRight.y = upperLeft.y + (getGameWindowHeight() * greenBarRightRatio.y);
}

void GameWindow::calcExitCaughtButton()
{
	exitCaughtButton.x = upperLeft.x + (getGameWindowWidth() * exitCaughtRatio.x);
	exitCaughtButton.y = upperLeft.y + (getGameWindowHeight() * exitCaughtRatio.y);
}

void GameWindow::calcGreenBar()
{
	greenBar.x = upperLeft.x + (getGameWindowWidth() * greenBarRatio.x);
	greenBar.y = upperLeft.y + (getGameWindowHeight() * greenBarRatio.y);
}

void GameWindow::calcCloseAds()
{
	closeAds.x = upperLeft.x + (getGameWindowWidth() * closeAdRatio.x);
	closeAds.y = upperLeft.y + (getGameWindowHeight() * closeAdRatio.y);
}

void GameWindow::calculateAll()//calculates coordinates of all buttons and resizes images
{
	calcPullButton();
	calcExitCaughtButton();
	calcGreenBar();
	calcGreenBarRight();
	calcCloseAds();

	//img resizing
	double scale = getGameWindowWidth()/899.0f;
	castImg = ResizeMatByPercentage(castImg, scale);
	pullImg = ResizeMatByPercentage(pullImg, scale);
	caughtImg = ResizeMatByPercentage(caughtImg, scale);
	failedImg = ResizeMatByPercentage(failedImg, scale);
	adImg = ResizeMatByPercentage(adImg, scale);

}

Coord GameWindow::getUpperLeft() const { return upperLeft; }

Coord GameWindow::getBottomRight() const { return bottomRight; }

Coord GameWindow::getPullButton() const { return pullButton; }

Coord GameWindow::getExitCaughtButton()const { return exitCaughtButton; }

Coord GameWindow::getGreenBar() const{	return greenBar; }

Coord GameWindow::getGreenBarRight() const { return greenBarRight; }

Coord GameWindow::getCloseAds() const { return closeAds; }

int GameWindow::getGameWindowHeight() const 
{
	return abs(bottomRight.y - upperLeft.y); 
}

int GameWindow::getGameWindowWidth() const 
{
	return abs(bottomRight.x - upperLeft.x);
}

Coord GameWindow::RatioToCoord(const Ratio& x)
{
	Coord r;
	r.x = upperLeft.x + (getGameWindowWidth() * x.x);
	r.y = upperLeft.y + (getGameWindowHeight() * x.y);
	return r;
}

bool GameWindow::canCast(HWND* hwnd)
{
	int x = RatioToCoord(castRectULRatio).x;
	int y = RatioToCoord(castRectULRatio).y;
	int w = abs(RatioToCoord(castRectULRatio).x - RatioToCoord(castRectBRRatio).x);
	int h = abs(RatioToCoord(castRectULRatio).y - RatioToCoord(castRectBRRatio).y);

	const cv::Mat game = CaptureScreenMat(*hwnd,x , y, w, h);
	return ContainsImage(castImg, game);
}

bool GameWindow::canPull(HWND* hwnd)
{
	int x = RatioToCoord(pullRectULRatio).x;
	int y = RatioToCoord(pullRectULRatio).y;
	int w = abs(RatioToCoord(pullRectULRatio).x - RatioToCoord(pullRectBRRatio).x);
	int h = abs(RatioToCoord(pullRectULRatio).y - RatioToCoord(pullRectBRRatio).y);
	const cv::Mat game = CaptureScreenMat(*hwnd, x, y, w, h);
	return ContainsImage(pullImg, game);
}

bool GameWindow::canCloseAds(HWND* hwnd)
{
	int x = RatioToCoord(adImgULRatio).x;
	int y = RatioToCoord(adImgULRatio).y;
	int w = abs(RatioToCoord(adImgULRatio).x - RatioToCoord(adImgBRRatio).x);
	int h = abs(RatioToCoord(adImgULRatio).y - RatioToCoord(adImgBRRatio).y);
	const cv::Mat game = CaptureScreenMat(*hwnd, x, y, w, h);
	return ContainsImage(adImg, game, 0.95);
}

bool GameWindow::isCatchingSpin(HDC* hdc, HWND* hwnd)
{
	COLORREF color = GetPixel(*hdc, getGreenBarRight().x, getGreenBarRight().y);

	if (IsSimilarColor(color, 61, 99, 135,1))//quickest check first,
		return true;

	return false;
}

bool GameWindow::isCatching(HDC* hdc, HWND* hwnd)
{
	COLORREF color = GetPixel(*hdc, getGreenBarRight().x, getGreenBarRight().y);

	if(IsSameColor(color, 61, 99, 135))//quickest check first,
		return true;

	if (canPull(hwnd))
		return true;
	
	return false;
	

}
	
bool GameWindow::isOnCaughtScreen(HWND* hwnd)
{
	int x = RatioToCoord(caughtRectULRatio).x;
	int y = RatioToCoord(caughtRectULRatio).y;
	int w = abs(RatioToCoord(caughtRectULRatio).x - RatioToCoord(caughtRectBRRatio).x);
	int h = abs(RatioToCoord(caughtRectULRatio).y - RatioToCoord(caughtRectBRRatio).y);
	const cv::Mat game = CaptureScreenMat(*hwnd, x, y, w, h);
	
	return ContainsImage(caughtImg, game);
}

bool GameWindow::isOnFailedScreen(HWND* hwnd)
{
	int x = RatioToCoord(failedRectULRatio).x;
	int y = RatioToCoord(failedRectULRatio).y;
	int w = abs(RatioToCoord(failedRectULRatio).x - RatioToCoord(failedRectBRRatio).x);
	int h = abs(RatioToCoord(failedRectULRatio).y - RatioToCoord(failedRectBRRatio).y);
	const cv::Mat game = CaptureScreenMat(*hwnd, x, y, w, h);
	return (ContainsImage(failedImg, game));

}

cv::Mat GameWindow::getCastImg(HWND hwnd)
{
	int x = RatioToCoord(castRectULRatio).x;
	int y = RatioToCoord(castRectULRatio).y;
	int w = abs(RatioToCoord(castRectULRatio).x - RatioToCoord(castRectBRRatio).x);
	int h = abs(RatioToCoord(castRectULRatio).y - RatioToCoord(castRectBRRatio).y);

	
	cv::Mat game = CaptureScreenMat(hwnd, x, y, w, h);
	return game;
}

cv::Mat GameWindow::getPullImg(HWND hwnd)
{
	int x = RatioToCoord(pullRectULRatio).x;
	int y = RatioToCoord(pullRectULRatio).y;
	int w = abs(RatioToCoord(pullRectULRatio).x - RatioToCoord(pullRectBRRatio).x);
	int h = abs(RatioToCoord(pullRectULRatio).y - RatioToCoord(pullRectBRRatio).y);

	cv::Mat game = CaptureScreenMat(hwnd, x, y, w, h);
	return game;
}

