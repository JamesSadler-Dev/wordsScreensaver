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
using std::string;

void setfontsize(int height);
void setConsoleFullscreen();
void setSplitLogic(string current,char outputWord[]);
int sleepLoopExited(int n);
void printBuffer(int n);


//wordsScreensaver Main loop
int main()
{

    setfontsize(130);
    setConsoleFullscreen();
    srand(time(0));
    vector<string> words;
    string word = "";
    ifstream file("words.txt");


    while (getline(file, word))
    {
        words.push_back(word);
    }

    //
    int randomBuffer = 0;
;   string past = "";

    // screensaver loop
    while (true)
    {

        // these prevent us from having to recalculate in the loop needlessly
        int randomIndex = rand() % words.size();
        string current = words[randomIndex];


        // guardian pattern
        if (current.length() < 2 || current.find_first_of("") == 0)
        {
            continue;
        }

        // Skip repeats one time
        if (!past.empty() || past == "")
        {
            if (current == past)
            {
                continue;
            }
            else{
                past = current;
            }
        }
        else{
            past = current;
        }

        char outputWord[100];
        setSplitLogic(current,outputWord);

        system("cls");
        randomBuffer = (rand() % 3) + 1;

        printBuffer(9);

        std::cout << outputWord;
        
        printBuffer(randomBuffer);

        if (sleepLoopExited(30))
        {
            return 0;
        }
    }
}
/*
 */

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

void setSplitLogic(string current,char outputWord[])
{
        const char *COL1 = "\u001b[35m";
        const char *COL2 = "\u001b[32m";
        const char *COL3 = "\u001b[34m";
        const char *COL4 = "\u001b[97m";
        const char *COL5 = "\u001b[31m";
        const char *COL6 = "\u001b[95m";
        const char *COL7 = "\u001b[33m";
        // lengths to set conditions with
        const int WRAP_LEN = 18;
        const int LENGTH_LIMIT= 44;
        const int FIRSTP_LIMIT= 22;
        const int PAD_XXXL = 20;
        const int PAD_XXL = 19;
        const int PAD_XL = 18;
        const int PAD_L = 17;
        const int PAD_M = 15;
        const int PAD_S = 13;
        const int PAD_XS = 11;
        const int PAD_XXS = 10;
        const int PAD_XXXS = 9;
        const int PAD_XXXXS = 6;
        //
        outputWord[0] = 0;
        outputWord[1] = 0;
        int totalLen = current.length();
        const char *totalWordArr = current.c_str();


        /*
         *   CASE 1: LINE IS REALLY LONG JUST PRINT NORMALLY
         */

        if (totalLen > LENGTH_LIMIT)
        {
             sprintf(outputWord, "%s%s", COL2, totalWordArr);
        }
        else if (totalLen > WRAP_LEN)
        {

            int indexOfSpace = current.find_last_of(" ");

            /* CASE 2:
             *    IF LONGER THAN WRAP_LEN BUT NO SPACES
             */
            if (indexOfSpace == -1)
            {
                // extra padding if it's too long
                if (totalLen > PAD_XXL)
                {
                     sprintf(outputWord, "%s%23s", COL1, totalWordArr);
                }
                else if (totalLen > PAD_M)
                {
                     sprintf(outputWord, "%s%21s", COL1, totalWordArr);
                }
                else
                {
                     sprintf(outputWord, "%s%17s", COL2, totalWordArr);
                }
            }
            /* CASE 3:
             *    IF FIRSTPART IS LONGER THAN LONG CUTOFF
             */
            else
            {
                //
                // having these awesome variables avoids doing this calculation many times
                string firstPart = current.substr(0, indexOfSpace);
                string lastPart = current.substr(indexOfSpace + 1);
                const char *p1Array = firstPart.c_str();
                const char *p2Array = lastPart.c_str();
                int fpLength = firstPart.length();
                int lpLength = lastPart.length();

                /* CASE 3:
                 *    IF FIRSTPART IS LONGER THAN LONG CUTOFF
                 */
                if (fpLength > PAD_M)
                {
                    // LONG FIRSTPART --- SHORT LASTPART
                    if (lpLength < PAD_XXXXS)
                    {
                        if (fpLength > PAD_XXL)
                        {
                             sprintf(outputWord, "%s%22s\n%14s", COL2, p1Array, p2Array);
                        }
                        else
                        {
                             sprintf(outputWord, "%s%20s\n%14s", COL1, p1Array, p2Array);
                        }

                        // FIRSTPART LONGER THAN LONG CUTOFF AND SHORTER THAN XLCUTOFF -- NORMAL LASTPART
                    }
                    else if (fpLength < PAD_L)
                    {
                        // DON'T PAD LASTPART AS MUCH IF IT's TOO SHORT
                        if (lpLength < PAD_XXXS)
                        {
                             sprintf(outputWord, "%s%19s\n%16s", COL2, p1Array, p2Array);
                        }
                        else
                        {
                             sprintf(outputWord, "%s%19s\n%18s", COL1, p1Array, p2Array);
                        }
                    }
                    else
                    {
                        // FIRSTPART EVEN LONGER THAN XL CUTOFF
                        //  if FIRSTPART too long just format it normally
                        if (fpLength > FIRSTP_LIMIT)
                        {
                             sprintf(outputWord, "%s%s", COL2, totalWordArr);
                            // long first part and long last part
                        }
                        else if (fpLength > PAD_XXXL)
                        {
                            if (lpLength > PAD_L)
                            {
                                 sprintf(outputWord, "%s%22s\n%21s", COL2, p1Array, p2Array);
                            }
                            else
                            {
                                 sprintf(outputWord, "%s%22s\n%18s", COL2, p1Array, p2Array);
                            }
                            // long firstpart and lastpart
                        }
                        else if (lpLength > PAD_XL)
                        {
                             sprintf(outputWord, "%s%21s\n%22s", COL2, p1Array, p2Array);

                            // lastpart too short print normally
                        }
                        else if (lpLength < PAD_XXXXS)
                        {
                             sprintf(outputWord, "%s%s", COL2, totalWordArr);
                        }
                        else if (lpLength < PAD_XXXS)
                        {
                             sprintf(outputWord, "%s%21s\n%16s", COL2, p1Array, p2Array);
                        }
                        else
                        {
                             sprintf(outputWord, "%s%22s\n%21s", COL2, p1Array, p2Array);
                        }
                    }

                    /* CASE 4:
                     *    IF FIRSTPART IS LONGER THAN SHORT CUTOFF
                     */
                }
                else if (fpLength > PAD_S)
                {
                    if (lpLength < PAD_XXXXS)
                    {
                         sprintf(outputWord, "%s%19s\n%14s", COL1, p1Array, p2Array);
                        // DON'T PAD LASTPART AS MUCH IF IT's TOO SHORT
                    }
                    else if (lpLength < PAD_XXS)
                    {
                         sprintf(outputWord, "%s%18s\n%15s", COL2, p1Array, p2Array);
                    }
                    else
                    {
                         sprintf(outputWord, "%s%19s\n%17s", COL2, p1Array, p2Array);
                    }

                    /* CASE 5:
                     *
                     *    IF FIRSTPART IS SHORTER THAN PAD LONG OR SHORT CUTOFF
                     *
                     */
                }
                else if (fpLength < PAD_S)
                {

                    if (fpLength == lpLength)
                    {
                         sprintf(outputWord, "%s%16s\n%16s", COL1, p1Array, p2Array);
                    }
                    else if (fpLength <= PAD_XXXXS)
                    {
                        if (lpLength > PAD_XXL)
                        {
                             sprintf(outputWord, "%s%14s\n%23s", COL1, p1Array, p2Array);
                        }
                        else if (lpLength > PAD_M)
                        {
                             sprintf(outputWord, "%s%14s\n%19s", COL1, p1Array, p2Array);
                        }
                        else
                        {
                             sprintf(outputWord, "%s%14s\n%19s", COL1, p1Array, p2Array);
                        }

                        // if firstpart between padleftTiny-padLeftShort
                    }
                    else if (fpLength < PAD_XXXS)
                    {
                         sprintf(outputWord, "%s%15s\n%17s", COL2, p1Array, p2Array);

                        // If LASTPART is LESS than tiny or medium cutoff
                    }
                    else if (lpLength < PAD_XXXXS)
                    {
                         sprintf(outputWord, "%s%16s\n%13s", COL2, p1Array, p2Array);

                        // DON'T PAD LASTPART AS MUCH IF IT's TOO SHORT
                    }
                    else if (lpLength < PAD_XXXS)
                    {
                         sprintf(outputWord, "%s%16s\n%14s", COL2, p1Array, p2Array);
                    }
                    else if (fpLength >= PAD_XS)
                    {
                        if (lpLength > PAD_L)
                        {
                             sprintf(outputWord, "%s%17s\n%20s", COL2, p1Array, p2Array);
                        }
                        else
                        {
                             sprintf(outputWord, "%s%17s\n%16s", COL2, p1Array, p2Array);
                        }
                    }
                    else if (fpLength >= PAD_XXXS)
                    {
                        if (lpLength > PAD_L)
                        {
                             sprintf(outputWord, "%s%16s\n%23s", COL2, p1Array, p2Array);
                        }
                        else if (lpLength >= PAD_XS)
                        {
                             sprintf(outputWord, "%s%16s\n%17s", COL2, p1Array, p2Array);
                        }
                        else
                        {
                             sprintf(outputWord, "%s%16s\n%16s", COL2, p1Array, p2Array);
                        }
                        // if LASTPART is LONGER than padding cutoff
                    }
                    else if (lpLength > PAD_S)
                    {
                         sprintf(outputWord, "%s%16s\n%21s", COL2, p1Array, p2Array);
                    }
                    else
                    {
                         sprintf(outputWord, "%s%17s\n%18s", COL2, p1Array, p2Array);
                    }
                }
            }
        }
        
        /* CASE 6:
         *
         *    ELSE BRANCH REACHED IF WRAP LOGIC NOT ACTIVATED
         *
         */
        else
        {
            if (totalLen > PAD_L)
            {
                 sprintf(outputWord, "%s%21s", COL3, totalWordArr);
            }
            else if (totalLen > PAD_M)
            {
                 sprintf(outputWord, "%s%20s", COL3, totalWordArr);
            }
            else if (totalLen > PAD_S)
            {
                  sprintf(outputWord, "%s%19s", COL5, totalWordArr);
            }
            else if (totalLen > PAD_XXS)
            {
                  sprintf(outputWord, "%s%17s", COL6, totalWordArr);
            }
            else if (totalLen < PAD_XXXXS)
            {
                 sprintf(outputWord, "%s%13s", COL7, totalWordArr);
            }
            else if (totalLen > PAD_XXS)
            {
                  sprintf(outputWord, "%s%16s", COL4, totalWordArr);
            }
            else
            {
                 sprintf(outputWord, "%s%15s", COL4, totalWordArr);
            }
        }
    }

int sleepLoopExited(int n)
{
    // Sleep for n seconds
    for (int i = 0; i < n; i++)
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
        this_thread::sleep_for(chrono::seconds(1));
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



// finish these methods: one to build the string one to format it
// char * buildString()
// char * buildTwoLineString()
// void formatColorAndString(){
