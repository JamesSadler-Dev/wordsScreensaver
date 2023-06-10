#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <windows.h>

void setfontsize(int height);
void setConsoleFullscreen();
int sleepLoopExited(int n);
void printBuffer(int n);

using namespace std;

const char *col1 = "\u001b[35m";
const char *col2= "\u001b[32m";
const char *col3= "\u001b[34m";
const char *col4= "\u001b[97m";
const char *col5= "\u001b[31m";
const char *col6= "\u001b[95m";
const char *col7= "\u001b[33m";
//lengths to set conditions with
const int wraplen = 18;
const int xlCutoff = 17;
const int padleftLongMin = 15;
const int padleftShortMin = 13;
const int padleftShortOneLineMin = 10;
const int padleftTinyMax= 6;


/*
wordsScreensaver Main Loop
*/
int main(){

    setfontsize(135);
    setConsoleFullscreen();
    srand(time(0));
    vector<string> words;
    string word = "";
    ifstream file("words.txt");

    char outputWord[100];
    //let's initialize some values
    for (int i = 0 ; i < 1 ; i++){
        outputWord[i] = 0;
    }

    while (getline(file, word))
    {
        words.push_back(word);
    }
    

    //
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
        const char* totalWordArr = current.c_str();

        //guardian pattern
        if (totalLen < 2 || current.find_first_of("") == 0){
            continue;
        }

        // Skip repeats
        if (!past.empty() || past == ""){
            if (totalWordArr == past) {
                continue;
            } else {
                past = totalWordArr;
            }
        } else {
            past = totalWordArr;
        }
        
        /* 
        *   CASE 1: LINE IS REALLY LONG JUST PRINT NORMALLY
        */
        

        if (totalLen > 44){
            sprintf(outputWord, "%s%s",col2, totalWordArr);
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
                    sprintf(outputWord, "%s%23s",col1, totalWordArr);
                    signalType="2a";
                } else if (totalLen > padleftLongMin){
                    sprintf(outputWord, "%s%21s",col1, totalWordArr);
                    signalType="2b";    
                } else {
                    sprintf(outputWord, "%s%17s",col2, totalWordArr);
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
                const char* p1Array = firstPart.c_str();
                const char* p2Array = lastPart.c_str();
                int fpLength= firstPart.length();
                int lpLength= lastPart.length();

                /* CASE 3: 
                *    IF FIRSTPART IS LONGER THAN LONG CUTOFF
                */
                if (fpLength > padleftLongMin){
                    // LONG FIRSTPART --- SHORT LASTPART
                    if (lpLength < padleftTinyMax) {
                        if (fpLength > xlCutoff + 2){
                            sprintf(outputWord, "%s%22s\n%14s",col2, p1Array , p2Array);
                            signalType="3a";
                        } else {
                            sprintf(outputWord, "%s%20s\n%14s",col1, p1Array , p2Array);
                            signalType="3b";
                        }
                        
                    // FIRSTPART LONGER THAN LONG CUTOFF AND SHORTER THAN XLCUTOFF -- NORMAL LASTPART
                    } else if (fpLength < xlCutoff){
                        //DON'T PAD LASTPART AS MUCH IF IT's TOO SHORT
                        if (lpLength < 9){
                            sprintf(outputWord, "%s%19s\n%16s",col2, p1Array , p2Array);
                            signalType="3c";
                        }else {
                            sprintf(outputWord, "%s%19s\n%18s",col1, p1Array , p2Array);
                            signalType="3d";
                        }  
                    } else {
                        //FIRSTPART EVEN LONGER THAN XL CUTOFF
                        // if FIRSTPART too long just format it normally
                        if (fpLength > 24){
                            sprintf(outputWord, "%s%s",col2, totalWordArr);
                            signalType="3e";
                        // long first part and long last part   
                        } else if (fpLength > 20){
                            if (lpLength > xlCutoff){
                                sprintf(outputWord, "%s%22s\n%21s",col2, p1Array , p2Array);
                                signalType="3f";
                            } else {
                                sprintf(outputWord, "%s%22s\n%18s",col2, p1Array , p2Array);
                                signalType="3g";
                            }
                        // long firstpart and lastpart
                        } else if (lpLength > xlCutoff + 1){
                            sprintf(outputWord, "%s%21s\n%22s",col2, p1Array , p2Array);
                            signalType="3h";
                        
                        // lastpart too short print normally
                        } else if (lpLength < padleftTinyMax){
                            sprintf(outputWord, "%s%s",col2, totalWordArr);
                            signalType="3i";
                        } else if (lpLength < 9){
                            sprintf(outputWord, "%s%21s\n%16s",col2, p1Array , p2Array);
                            signalType="3j";
                        }else {
                            sprintf(outputWord, "%s%22s\n%21s",col2, p1Array , p2Array);
                            signalType="3k";
                        }
                    }

                    /* CASE 4: 
                    *    IF FIRSTPART IS LONGER THAN SHORT CUTOFF
                    */
                } else if (fpLength > padleftShortMin ) {
                    if (lpLength < padleftTinyMax) {
                        sprintf(outputWord, "%s%19s\n%14s",col1, p1Array , p2Array);
                        signalType="4a";
                    //DON'T PAD LASTPART AS MUCH IF IT's TOO SHORT
                    } else if (lpLength < 10){
                        sprintf(outputWord, "%s%18s\n%15s",col2, p1Array , p2Array);
                        signalType="4b";
                    } else {
                        sprintf(outputWord, "%s%19s\n%17s",col2, p1Array , p2Array);
                        signalType="4c";
                    }
                
                    /* CASE 5: 
                    *
                    *    IF FIRSTPART IS SHORTER THAN PAD LONG OR SHORT CUTOFF
                    * 
                    */
                } else if (fpLength < padleftShortMin) {
                    
                    if (fpLength == lpLength){
                        sprintf(outputWord, "%s%16s\n%16s",col1, p1Array , p2Array);
                        signalType="5a";
                    } else if (fpLength <= padleftTinyMax) {
                        if (lpLength > xlCutoff + 2){
                            sprintf(outputWord, "%s%14s\n%23s",col1, p1Array , p2Array);
                            signalType="5b";
                        }else if (lpLength > padleftLongMin){
                            sprintf(outputWord, "%s%14s\n%19s",col1, p1Array , p2Array);
                            signalType="5c";
                        }
                        else {
                            sprintf(outputWord, "%s%14s\n%19s",col1, p1Array , p2Array);
                            signalType="5d";
                        }
                        
                    // if firstpart between padleftTiny-padLeftShort
                    } else if (fpLength < 9){
                        sprintf(outputWord, "%s%15s\n%17s",col2, p1Array , p2Array);
                        signalType="5e";
                    
                    //If LASTPART is LESS than tiny or medium cutoff
                    } else if (lpLength < padleftTinyMax){
                        sprintf(outputWord, "%s%16s\n%13s",col2, p1Array , p2Array);  
                        signalType="5f";
                           
                    //DON'T PAD LASTPART AS MUCH IF IT's TOO SHORT
                    } else if (lpLength < 9){
                        sprintf(outputWord, "%s%16s\n%14s",col2, p1Array , p2Array);
                        signalType="5g";
     
                    } else if (fpLength >= 11){ 
                        if (lpLength > xlCutoff){
                            sprintf(outputWord, "%s%17s\n%20s",col2, p1Array , p2Array); 
                            signalType="5h";    
                            } else {
                            sprintf(outputWord, "%s%17s\n%16s",col2, p1Array , p2Array);   
                            signalType="5i";  
                            }    
                    } else if (fpLength >= 9){
                            if (lpLength > xlCutoff){
                                sprintf(outputWord, "%s%16s\n%23s",col2, p1Array , p2Array);  
                                signalType="5j";   
                            } else if (lpLength >= 11) {
                                sprintf(outputWord, "%s%16s\n%17s",col2, p1Array , p2Array);     
                                signalType="5k";
                            } else {
                                sprintf(outputWord, "%s%16s\n%16s",col2, p1Array , p2Array);     
                                signalType="5h";
                            }
                    // if LASTPART is LONGER than padding cutoff
                    } else if (lpLength > padleftShortMin){
                        sprintf(outputWord, "%s%16s\n%21s",col2, p1Array , p2Array);      
                        signalType="5l";   
                    } else {
                        sprintf(outputWord, "%s%17s\n%18s",col2, p1Array , p2Array);  
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
            if (totalLen > xlCutoff){
                sprintf(outputWord, "%s%21s",col3, totalWordArr);
                signalType = "za";
            } else if (totalLen > padleftLongMin){
                sprintf(outputWord, "%s%20s",col3, totalWordArr);
                signalType = "zb";
            } else if (totalLen > padleftShortMin){
                sprintf(outputWord, "%s%19s",col5, totalWordArr);
                signalType = "zb";
            } else if (totalLen > padleftShortOneLineMin){
                sprintf(outputWord, "%s%17s",col6, totalWordArr);
                signalType = "zc";
            } else if (totalLen < padleftTinyMax) {
                sprintf(outputWord, "%s%13s",col7, totalWordArr);
                signalType = "zd";
            } else if (totalLen > 10) {
                sprintf(outputWord, "%s%16s",col4, totalWordArr);
                signalType = "ze";
            } else {
                sprintf(outputWord, "%s%15s",col4, totalWordArr);
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
    /*
    */

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


    //finish these methods: one to build the string one to format it
    //char * buildString()
    //char * buildTwoLineString()
    //void formatColorAndString(){

