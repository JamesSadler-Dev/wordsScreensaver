#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <windows.h>

using namespace std;

namespace screensaver{
    void setfontsize(int height)
    {

        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0;      // Width of each character in the font
        cfi.dwFontSize.Y = height; // Height
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_EXTRABOLD;

        std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {500, 500});
    }

void setConsoleFullscreen()
    {
        // windows api, get X console window property change constants and set property
        HWND hWnd = GetConsoleWindow();
        SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_BORDER & ~WS_CAPTION);
        ShowWindow(hWnd, SW_MAXIMIZE);
        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.dwSize = 1;
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenBufferInfo);
        // Set the console screen buffer size to the maximum window size.
        COORD newScreenBufferSize = {static_cast<SHORT>(screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left + 1),
                                    static_cast<SHORT>(screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top + 1)};
        SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), newScreenBufferSize);
    }

    int sleepLoopExited(int n)
    {
        // Sleep for n seconds
        for (int i = 0; i < (n * 10); i++)
        {
            // check for escape key or clicking
            if (GetAsyncKeyState(VK_ESCAPE))
            {
                return 1;
            }
            else if (GetAsyncKeyState(VK_LBUTTON))
            {
                return 1;
            }
            else if (GetAsyncKeyState(VK_RBUTTON))
            {
                return 1;
            }
            else if (GetAsyncKeyState(VK_SPACE))
            {
                return 1;
            }
            // Skip this word but don't exit program with kb right function
            else if (GetAsyncKeyState(VK_RIGHT))
            {
                return 0;
            }
            // sleep for one second n times in the loop to be more responsive
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        // onto the next word if we reached 0
        return 0;
}

void printBuffer(int n)
    {
        for (int i = 0; i < n; i++)
        {
            cout << "\n";
        }
    }
}