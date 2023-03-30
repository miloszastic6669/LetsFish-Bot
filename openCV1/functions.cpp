#include "functions.h"
#include "gameWindow.h"

int dick::msToFps(int ms)
{
    return int(round(1000.f / ms));
}

int dick::fpsToMs(float fps)
{
    return int(round(1000.f / fps));
}

Coord GetMousePosition()
{
    POINT p;
    Coord pos(0,0);
    if(GetCursorPos(&p))
    {
        pos.x = p.x;
        pos.y = p.y;
        return pos;
    }else
    {
        std::cout << "Error getting mouse coordinates\n";
        return pos;
    }

}

bool IsSameColor(const COLORREF& c, int r, int g, int b)
{
    if (GetRValue(c) == r && GetGValue(c) == g && GetBValue(c) == b)
        return true;
    return false;
}

bool IsSimilarColor(const COLORREF& c, int r, int g, int b, int threshold)
{
    int colorR = GetRValue(c);
    int colorG = GetGValue(c);
    int colorB = GetBValue(c);

    if (((r - threshold) <= colorR && colorR <= (r + threshold)) &&
        ((g - threshold) <= colorG && colorG <= (g + threshold)) &&
        ((b - threshold) <= colorB && colorB <= (b + threshold)))
        return true;
    return false;

}

COLORREF GetPixel(HDC* hdc, Coord pos)
{
    return GetPixel(*hdc, pos.x, pos.y);
}

int GetKeyCode(const char* keyName)
{
    int vkCode = -1;
    SHORT result = VkKeyScanA(*keyName);
    if (result != -1)
    {
        vkCode = LOBYTE(result);
    }
    return vkCode;
}

int GetKeyCode(std::string keyName)
{
   return GetKeyCode(keyName.c_str());
}

int GetScreenDepth()
{
    HDC hdcScreen = GetDC(NULL);
    int screenDepth = GetDeviceCaps(hdcScreen, BITSPIXEL);
    ReleaseDC(NULL, hdcScreen);
    return screenDepth;
}

BITMAPINFOHEADER CreateBitmapHeader(int width, int height)
{
    BITMAPINFOHEADER  bi;

    // create a bitmap
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    return bi;
}

cv::Mat CaptureScreenMat(HWND hwnd, int x, int y, int w, int h)
{
    cv::Mat src;

    // get handles to a device context (DC)
    HDC hwindowDC = GetDC(hwnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    // create mat object
    src.create(h, w, CV_8UC4);

    // create a bitmap
    HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, w, h);
    BITMAPINFOHEADER bi = CreateBitmapHeader(w, h);

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);

    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, w, h, hwindowDC, x, y, w, h, SRCCOPY);                  //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, h, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);   //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
}

cv::Mat CaptureScreenMat(HWND hwnd, const GameWindow& p)
{
    int x = p.getUpperLeft().x;
    int y = p.getUpperLeft().y;
    int w = p.getGameWindowWidth();
    int h = p.getGameWindowHeight();

    return CaptureScreenMat(hwnd, x, y, w, h);
}

cv::Mat CaptureScreenMat(HWND hwnd, Coord UL, Coord BR)
{
    int x = UL.x;
    int y = UL.y;
    int w = abs(BR.x - x);
    int h = abs(BR.y - y);

    return CaptureScreenMat(hwnd, x, y, w, h);
}

void DrawRectangle(cv::Mat& image, int x, int y, int width, int height, int thickness, cv::Scalar color, int alpha) {
    cv::Mat overlay;
    image.copyTo(overlay);

    // Draw the rectangle on the overlay image
    cv::rectangle(overlay, cv::Rect(x, y, width, height), color, thickness);

    // Apply transparency to the rectangle
    cv::addWeighted(overlay, alpha / 255.0, image, 1 - alpha / 255.0, 0, image);
}

void SaveScreenshotToJPEG(const std::string& filename, int x, int y, int w, int h)
{
    cv::Mat screenshot = CaptureScreenMat(GetDesktopWindow(), x, y, w, h);
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(20);
    cv::imwrite(filename, screenshot, compression_params);
}


//config
void CreateNewConfig()
{
    std::cout << "Config name:\n";
    std::string name;
    std::cin >> name;
    name += ".json";
    GameWindow p;
    std::cout << "Move your Mouse to upper left corner of the game and press X\n";
    while(true)
    {
        if(GetAsyncKeyState(GetKeyCode("X")) & 1)
        {
            p.setUpperLeft(GetMousePosition());
            std::cout << "Upper left corner = (" << p.getUpperLeft().x << "," << p.getUpperLeft().y << ")\n";
            break;
        }
        Sleep(20);
    }
    std::cout << "Move your Mouse to bottom right corner of the game and press X\n";
    while (true)
    {
        if (GetAsyncKeyState(GetKeyCode("X")) & 1)
        {
            p.setBottomRight(GetMousePosition());
            std::cout << "Bottom right corner = (" << p.getBottomRight().x << "," << p.getBottomRight().y << ")\n";
            break;
        }
        Sleep(20);
    }
    serializeToJson(p, name);
}

std::vector<std::string> EnumerateFilesInRoot()
{
    std::vector<std::string> filenames;

    for (const auto& entry : std::filesystem::directory_iterator("."))
    {
        if (entry.is_regular_file())
        {
            filenames.push_back(entry.path().filename().string());
        }
    }

    return filenames;
}

std::vector<std::string> GetConfigs() 
{
    std::vector<std::string> configs;

    std::vector<std::string> files = EnumerateFilesInRoot();

    for(const auto& f : files)
    {
        if (f.find(".json") != std::string::npos)
        {
            configs.push_back(f.substr(0, f.length() - 5)); // only file names without extensions
        }
    }
    return configs;
}

void LoadConfig(GameWindow& p)
{
    int config;
    std::string name;
    while(true)
    {
        std::cout << "Configs:\n";
        std::vector<std::string> configNames = GetConfigs();
        for(int i = 0; i < configNames.size(); i++)
        {
            std::cout << i << ".  " << configNames[i] << "\n";
        }

        std::cout << "Config:\n";        
        std::cin >> config;
        
        if ((config + 1) > configNames.size())
        {
            std::cout << "wrong config!\n";
            continue;
        }
        name = configNames[config] + ".json";
        std::cout << "loading in \"" << name << "\" ...\n";
        break;
    }
    
    deserializeFromJson(p, name);
}

//threading
void exit_thread()
{
    while (true)
    {
        if (GetAsyncKeyState(GetKeyCode("Q")) & 1)
        {
            std::cout << "panic button pressed!\n";
            exit(69420);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void sleepFor(long long time_sec, long long print_interval)
{
    while(time_sec > 0)
    {
        std::cout << "\nrefreshing in -> " << time_sec << "s\n";
        std::this_thread::sleep_for(std::chrono::seconds(print_interval));
        time_sec -= print_interval;
    }
}



cv::Mat convertToGray(cv::Mat& src)
{
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    return gray;
}

cv::Mat ResizeMatByPercentage(cv::Mat& src, double scale)
{
    int newWidth = static_cast<int>(src.cols * scale);
    int newHeight = static_cast<int>(src.rows * scale);

    cv::Mat resized;
    cv::resize(src, resized, cv::Size(newWidth, newHeight));
    return resized; 
}

bool ContainsImage(const cv::Mat& srcOrig, const cv::Mat& needleOrig, double threshold)
{
    cv::Mat src, needle;
    
    cv::cvtColor(srcOrig, src, cv::COLOR_BGR2GRAY);
    cv::cvtColor(needleOrig, needle, cv::COLOR_BGR2GRAY);

    cv::Mat result;
    cv::matchTemplate(src, needle, result, cv::TM_CCOEFF_NORMED);

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    return (maxVal >= threshold);
}

cv::Vec3b getPixelColor(const cv::Mat& img, int x, int y)
{
    return img.at<cv::Vec3b>(cv::Point(x,y));
}

