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

void setfontsize(int height){

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0; // Width of each character in the font
    cfi.dwFontSize.Y = height; // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;

    std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font

    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void setConsoleFullscreen(){
    HWND hWnd = GetConsoleWindow();
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_BORDER & ~WS_CAPTION);
    ShowWindow(hWnd, SW_MAXIMIZE);
}

int sleepLoopExited(int n){
        // Sleep for n seconds
    for (int i = 0 ; i < n ; i++){
        //check for escape key or clicking

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            return 1;
        }
        if (GetAsyncKeyState(VK_LBUTTON))
        {
            return 1;
        }
        if (GetAsyncKeyState(VK_RBUTTON))
        {
            return 1;
        }
        if (GetAsyncKeyState(VK_SPACE))
        {
            return 1;
        }

            this_thread::sleep_for(chrono::seconds(1));
        }
        return 0;
    }

void printBuffer(int n){
    for (int i = 0 ; i < n ; i++){
        cout << "\n";
    }        
}




//
int main()
{

    setfontsize(135);
    setConsoleFullscreen();
    srand(time(0));
    vector<string> words;
    string word;
    ifstream file("words.txt");
    char outputWord[100];


    while (getline(file, word))
    {
        words.push_back(word);
    }
    
    int wraplen = 18;
    int padleftLongMin = 13;
    int padleftShortMin = 13;
    int padleftShortOneLineMin = 10;
    int padleftTinyMax= 6;
    int xlCutoff = 17;
    // screensaver loop
    while (true)
    {
        system("cls");
        int randomIndex = rand() % words.size();
    
        if (words[randomIndex].length() > 44){
            sprintf(outputWord, "\u001b[32m%s", words[randomIndex].c_str());
        }
        else if ( words[randomIndex].length() > wraplen ){

            int indexOfSpace= words[randomIndex].find_last_of(" ");
            
                /* CASE 1: 
                *    IF LONGER THAN WRAPLEN BUT NO SPACES
                *    NOTE
                */
            if (indexOfSpace == -1){
                //extra padding if it's too long
                if (words[randomIndex].length() > 15){
                    sprintf(outputWord, "\u001b[32m%20s", words[randomIndex].c_str());
                } else {
                    sprintf(outputWord, "\u001b[32m%17s", words[randomIndex].c_str());
                }
            }
                /* CASE 2: 
                *    IF FIRSTPART IS LONGER THAN LONG CUTOFF
                */
            else {
                string firstPart= words[randomIndex].substr(0,indexOfSpace);
                string lastPart= words[randomIndex].substr(indexOfSpace+1);

                if (firstPart.length() > padleftLongMin + 2){
                    // ----- SHORT LASTPART ----
                    if (lastPart.length() < padleftTinyMax) {
                        if (firstPart.length() > xlCutoff){
                            sprintf(outputWord, "\u001b[32m%20s\n%14s", firstPart.c_str() , lastPart.c_str());
                        } else {
                            sprintf(outputWord, "\u001b[32m%19s\n%14s", firstPart.c_str() , lastPart.c_str());
                        }
                        
                    // FIRSTPART LONGER THAN LONG CUTOFF AND SHORTER THAN XLCUTOFF
                    } else if (firstPart.length() < xlCutoff){
                        //DON'T PAD LASTPART AS MUCH IF IT's TOO SHORT
                        if (lastPart.length() < 9){
                            sprintf(outputWord, "\u001b[32m%19s\n%16s", firstPart.c_str() , lastPart.c_str());
                        }else {
                            sprintf(outputWord, "\u001b[32m%19s\n%16s", firstPart.c_str() , lastPart.c_str());
                        }
                        //FIRSTPART EVEN LONGER THAN XL CUTOFF
                    } else {
                        if (lastPart.length() < 9){
                            sprintf(outputWord, "\u001b[32m%21s\n%16s", firstPart.c_str() , lastPart.c_str());
                        }else {
                            sprintf(outputWord, "\u001b[32m%21s\n%17s", firstPart.c_str() , lastPart.c_str());
                        }
                    }

                    /* CASE 3: 
                    *    IF FIRSTPART IS LONGER THAN SHORT CUTOFF
                    */
                } else if (firstPart.length() > padleftShortMin ) {
                    if (lastPart.length() < padleftTinyMax) {
                        sprintf(outputWord, "\u001b[32m%19s\n%14s", firstPart.c_str() , lastPart.c_str());
                    //DON'T PAD LASTPART AS MUCH IF IT's TOO SHORT
                    } else if (lastPart.length() < 10){
                        sprintf(outputWord, "\u001b[32m%18s\n%15s", firstPart.c_str() , lastPart.c_str());
                    } else {
                        sprintf(outputWord, "\u001b[32m%19s\n%17s", firstPart.c_str() , lastPart.c_str());
                    }
                
                    /* CASE 4: 
                    *    IF FIRSTPART IS SHORTER THAN PAD LONG OR SHORT CUTOFF 
                    */
                } else if (firstPart.length() < padleftShortMin) {
                    if (firstPart.length() < padleftTinyMax) {
                        sprintf(outputWord, "\u001b[32m%14s\n%16s", firstPart.c_str() , lastPart.c_str());
                    //DON'T PAD LASTPART AS MUCH IF IT's TOO SHORT
                    // if firstpart between padleftTiny-padLeftShort and lastPart > 10
                    } else if (lastPart.length() < 9){
                        sprintf(outputWord, "\u001b[32m%16s\n%14s", firstPart.c_str() , lastPart.c_str());
                    } else if (lastPart.length() < padleftTinyMax){
                        sprintf(outputWord, "\u001b[32m%16s\n%13s", firstPart.c_str() , lastPart.c_str());
                    } else if (lastPart.length() >= 9 && firstPart.length() > lastPart.length()){
                        sprintf(outputWord, "\u001b[32m%17s\n%16s", firstPart.c_str() , lastPart.c_str()); 
                    } else if (lastPart.length() >= 9 && firstPart.length() < lastPart.length()){
                        sprintf(outputWord, "\u001b[32m%16s\n%18s", firstPart.c_str() , lastPart.c_str());   
                    } else {
                        sprintf(outputWord, "\u001b[32m%15s\n%16s", firstPart.c_str() , lastPart.c_str());  
                }
                }
        }
        }

        //ELSE BRANCH REACHED IF WRAP LOGIC NOT ACTIVATED
        else  {
            if (words[randomIndex].length() > padleftLongMin + 2){
                sprintf(outputWord, "\u001b[32m%20s", words[randomIndex].c_str());
            } else if (words[randomIndex].length() > padleftLongMin){
                sprintf(outputWord, "\u001b[32m%19s", words[randomIndex].c_str());
            } else if (words[randomIndex].length() > padleftShortOneLineMin){
                sprintf(outputWord, "\u001b[32m%17s", words[randomIndex].c_str());
            } else if (words[randomIndex].length() < padleftTinyMax) {
                sprintf(outputWord, "\u001b[32m%13s", words[randomIndex].c_str());
            } else {
                sprintf(outputWord, "\u001b[32m%15s", words[randomIndex].c_str());
            }

        }


        printBuffer(9);

        cout << outputWord;

        printBuffer(3);

        if (sleepLoopExited(1)){
            return 0;
        }
    }
}
