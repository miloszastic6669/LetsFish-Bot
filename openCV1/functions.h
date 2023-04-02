#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <Windows.h>
#include <atomic>
#include <conio.h>
#include <thread>

#include "serialization.h"

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

struct Coord;
struct Ratio;
class GameWindow;

namespace dick
{
	int msToFps(int);
	int fpsToMs(float);
}


//image
int GetKeyCode(const char*);
int GetKeyCode(std::string keyName);
int GetScreenDepth();// in bits
cv::Mat CaptureScreenMat(HWND hwnd, int x, int y, int w, int h);
cv::Mat CaptureScreenMat(HWND hwnd, const GameWindow& p);
void DrawRectangle(cv::Mat& image, int x, int y, int width, int height, int thickness, cv::Scalar color, int alpha);
void SaveScreenshotToJPEG(const std::string& filename, int x, int y, int w, int h);
Coord GetMousePosition();
bool IsSameColor(const COLORREF& c, int r, int g, int b);
bool IsSimilarColor(const COLORREF& c, int r, int g, int b, int threshold);
COLORREF GetPixel(HDC* hdc, Coord pos);

//config files
void CreateNewConfig();
std::vector<std::string> EnumerateFilesInRoot();
std::vector<std::string> GetConfigs();
void LoadConfig(GameWindow& p);

//multithreading
void exit_thread(); // quit program if q is pressed
void lvlup_thread(GameWindow* d, std::atomic<bool>* stop_bool); //  

void sleepFor(long long time, long long print_interval, std::atomic<bool>* stop_running, GameWindow* d);



//template/pixel matching etc.
cv::Mat ResizeMatByPercentage(cv::Mat& src, double scale);
bool ContainsImage(const cv::Mat& source, const cv::Mat& target, double threshold = 0.8);
cv::Vec3b getPixelColor(const cv::Mat& img, int x, int y);