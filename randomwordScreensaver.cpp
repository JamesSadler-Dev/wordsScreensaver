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
    cfi.FontWeight = FW_EXTRABOLD;

    std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 500, 500 });
}

void setConsoleFullscreen(){
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
    COORD newScreenBufferSize = { static_cast<SHORT>(screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left + 1),
                              static_cast<SHORT>(screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top + 1) };
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), newScreenBufferSize);
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
        //Skip this word but don't exit program with kb right function
        if (GetAsyncKeyState(VK_RIGHT)){
            return 0;
        }
            //sleep for one second n times in the loop to be more responsive
            this_thread::sleep_for(chrono::seconds(1));
        }
        //onto the next word if we reached 0
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
    string word = "";
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
    int randomBuffer1= 0;
    int randomBuffer2= 0;
    string signalType = "";
    string past = "";
    // screensaver loop
    while (true)
    {

        //these prevent us from having to recalculate in the loop needlessly
        int randomIndex = rand() % words.size();
        string current = words[randomIndex];
        int totalLen= current.length();
        const char* totalCArray = current.c_str();

        //guardian pattern
        if (totalLen < 2){
            continue;
        }

        // Skip repeats
        if (!past.empty()){
            if (totalCArray == past) {
                continue;
            } else {
                past = totalCArray;
            }
        } else {
            past = totalCArray;
        }
        
        /* 
        *   CASE 1: LINE IS REALLY LONG JUST PRINT NORMALLY
        */
        

        if (totalLen > 44){
            sprintf(outputWord, "\u001b[32m%s", totalCArray);
            signalType="1a";
        }
        else if ( totalLen > wraplen ){

            int indexOfSpace= current.find_last_of(" ");
            

                /* CASE 2: 
                *    IF LONGER THAN WRAPLEN BUT NO SPACES
                */
            if (indexOfSpace == -1){
                //extra padding if it's too long
                if (totalLen > 19){
                    sprintf(outputWord, "\u001b[35m%23s", totalCArray);
                    signalType="2a";
                } else if (totalLen > 15){
                    sprintf(outputWord, "\u001b[35m%21s", totalCArray);
                    signalType="2b";    
                } else {
                    sprintf(outputWord, "\u001b[32m%17s", totalCArray);
                    signalType="2c";
                }
            }
                /* CASE 3: 
                *    IF FIRSTPART IS LONGER THAN LONG CUTOFF
                */
            else {
                //
                //having these awesome variables avoids doing this calculation many times
                string firstPart= current.substr(0,indexOfSpace);
                string lastPart= current.substr(indexOfSpace+1);
                const char* fPartCArray = firstPart.c_str();
                const char* LPartCArray = lastPart.c_str();
                int fpLength= firstPart.length();
                int lpLength= lastPart.length();

                /* CASE 3: 
                *    IF FIRSTPART IS LONGER THAN LONG CUTOFF
                */
                if (fpLength > padleftLongMin + 2){
                    // LONG FIRSTPART --- SHORT LASTPART
                    if (lpLength < padleftTinyMax) {
                        if (fpLength > xlCutoff + 2){
                            sprintf(outputWord, "\u001b[32m%22s\n%14s", fPartCArray , LPartCArray);
                            signalType="3a";
                        } else {
                            sprintf(outputWord, "\u001b[35m%20s\n%14s", fPartCArray , LPartCArray);
                            signalType="3b";
                        }
                        
                    // FIRSTPART LONGER THAN LONG CUTOFF AND SHORTER THAN XLCUTOFF -- NORMAL LASTPART
                    } else if (fpLength < xlCutoff){
                        //DON'T PAD LASTPART AS MUCH IF IT's TOO SHORT
                        if (lpLength < 9){
                            sprintf(outputWord, "\u001b[32m%19s\n%16s", fPartCArray , LPartCArray);
                            signalType="3c";
                        }else {
                            sprintf(outputWord, "\u001b[35m%19s\n%18s", fPartCArray , LPartCArray);
                            signalType="3d";
                        }  
                    } else {
                        //FIRSTPART EVEN LONGER THAN XL CUTOFF
                        // if FIRSTPART too long just format it normally
                        if (fpLength > 24){
                            sprintf(outputWord, "\u001b[32m%s", totalCArray);
                            signalType="3e";
                        // long first part and long last part   
                        } else if (fpLength > 20){
                            if (lpLength > xlCutoff){
                                sprintf(outputWord, "\u001b[32m%22s\n%21s", fPartCArray , LPartCArray);
                                signalType="3f";
                            } else {
                                sprintf(outputWord, "\u001b[32m%22s\n%18s", fPartCArray , LPartCArray);
                                signalType="3g";
                            }
                        // long firstpart and lastpart
                        } else if (lpLength > xlCutoff + 1){
                            sprintf(outputWord, "\u001b[32m%21s\n%22s", fPartCArray , LPartCArray);
                            signalType="3h";
                        
                        // lastpart too short print normally
                        } else if (lpLength < padleftTinyMax){
                            sprintf(outputWord, "\u001b[32m%s", totalCArray);
                            signalType="3i";
                        } else if (lpLength < 9){
                            sprintf(outputWord, "\u001b[32m%21s\n%16s", fPartCArray , LPartCArray);
                            signalType="3j";
                        }else {
                            sprintf(outputWord, "\u001b[32m%22s\n%21s", fPartCArray , LPartCArray);
                            signalType="3k";
                        }
                    }

                    /* CASE 4: 
                    *    IF FIRSTPART IS LONGER THAN SHORT CUTOFF
                    */
                } else if (fpLength > padleftShortMin ) {
                    if (lpLength < padleftTinyMax) {
                        sprintf(outputWord, "\u001b[35m%19s\n%14s", fPartCArray , LPartCArray);
                        signalType="4a";
                    //DON'T PAD LASTPART AS MUCH IF IT's TOO SHORT
                    } else if (lpLength < 10){
                        sprintf(outputWord, "\u001b[32m%18s\n%15s", fPartCArray , LPartCArray);
                        signalType="4b";
                    } else {
                        sprintf(outputWord, "\u001b[32m%19s\n%17s", fPartCArray , LPartCArray);
                        signalType="4c";
                    }
                
                    /* CASE 5: 
                    *
                    *    IF FIRSTPART IS SHORTER THAN PAD LONG OR SHORT CUTOFF
                    * 
                    */
                } else if (fpLength < padleftShortMin) {
                    
                    if (fpLength == lpLength){
                        sprintf(outputWord, "\u001b[35m%16s\n%16s", fPartCArray , LPartCArray);
                        signalType="5a";
                    } else if (fpLength <= padleftTinyMax) {
                        if (lpLength > xlCutoff + 2){
                            sprintf(outputWord, "\u001b[35m%14s\n%23s", fPartCArray , LPartCArray);
                            signalType="5b";
                        }else if (lpLength > padleftLongMin){
                            sprintf(outputWord, "\u001b[35m%14s\n%19s", fPartCArray , LPartCArray);
                            signalType="5c";
                        }
                        else {
                            sprintf(outputWord, "\u001b[35m%14s\n%19s", fPartCArray , LPartCArray);
                            signalType="5d";
                        }
                        
                    // if firstpart between padleftTiny-padLeftShort
                    } else if (fpLength < 9){
                        sprintf(outputWord, "\u001b[32m%15s\n%17s", fPartCArray , LPartCArray);
                        signalType="5e";
                    
                    //If LASTPART is LESS than tiny or medium cutoff
                    } else if (lpLength < padleftTinyMax){
                        sprintf(outputWord, "\u001b[32m%16s\n%13s", fPartCArray , LPartCArray);  
                        signalType="5f";
                           
                    //DON'T PAD LASTPART AS MUCH IF IT's TOO SHORT
                    } else if (lpLength < 9){
                        sprintf(outputWord, "\u001b[32m%16s\n%14s", fPartCArray , LPartCArray);
                        signalType="5g";
     
                    } else if (fpLength >= 11){ 
                        if (lpLength > xlCutoff){
                            sprintf(outputWord, "\u001b[32m%17s\n%20s", fPartCArray , LPartCArray); 
                            signalType="5h";    
                            } else {
                            sprintf(outputWord, "\u001b[34m%17s\n%16s", fPartCArray , LPartCArray);   
                            signalType="5i";  
                            }    
                    } else if (fpLength >= 9){
                            if (lpLength > xlCutoff){
                                sprintf(outputWord, "\u001b[32m%16s\n%23s", fPartCArray , LPartCArray);  
                                signalType="5j";   
                            } else if (lpLength >= 11) {
                                sprintf(outputWord, "\u001b[32m%16s\n%17s", fPartCArray , LPartCArray);     
                                signalType="5k";
                            } else {
                                sprintf(outputWord, "\u001b[32m%16s\n%16s", fPartCArray , LPartCArray);     
                                signalType="5h";
                            }
                    // if LASTPART is LONGER than padding cutoff
                    } else if (lpLength > padleftShortMin){
                        sprintf(outputWord, "\u001b[32m%16s\n%21s", fPartCArray , LPartCArray);      
                        signalType="5l";   
                    } else {
                        sprintf(outputWord, "\u001b[35m%17s\n%18s", fPartCArray , LPartCArray);  
                        signalType="5m";
                }
                }
        }
        }

        /* CASE 6: 
        *
        *    ELSE BRANCH REACHED IF WRAP LOGIC NOT ACTIVATED
        * 
        */
        else  {
            if (totalLen > padleftLongMin + 4){
                sprintf(outputWord, "\u001b[34m%21s", totalCArray);
                signalType = "za";
            } else if (totalLen > padleftLongMin + 2){
                sprintf(outputWord, "\u001b[34m%20s", totalCArray);
                signalType = "zb";
            } else if (totalLen > padleftLongMin){
                sprintf(outputWord, "\u001b[31m%19s", totalCArray);
                signalType = "zb";
            } else if (totalLen > padleftShortOneLineMin){
                sprintf(outputWord, "\u001b[95m%17s", totalCArray);
                signalType = "zc";
            } else if (totalLen < padleftTinyMax) {
                sprintf(outputWord, "\u001b[33m%13s", totalCArray);
                signalType = "zd";
            } else if (totalLen > 10) {
                sprintf(outputWord, "\u001b[97m%16s", totalCArray);
                signalType = "ze";
            } else {
                sprintf(outputWord, "\u001b[97m%15s", totalCArray);
                signalType = "zf";
            }

        }
        system("cls");
        randomBuffer1 = (rand() % 4) + 4;
        randomBuffer2 = (rand() % 3) + 1;

        printBuffer(9);

        std::cout << outputWord;
        //DEBUG OUTPUT
        //cout << "\n\t\t"+ signalType;

        printBuffer(randomBuffer2);

        if (sleepLoopExited(30)){
            return 0;
        }
    }
}
