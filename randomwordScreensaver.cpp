#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <windows.h>
#include "screensaverUtil.hh"

using namespace std;
using std::string;
using vectors = std::vector<std::string>;
using cstring = const char *;

void setSplitLogic(string current,char outputWord[]);


//wordsScreensaver Main loop
int main()
{
    screensaver::setfontsize(130);
    screensaver::setConsoleFullscreen();
    srand(time(0));
    vectors words;
    string word = "";
    ifstream file("words.txt");
    int randomBuffer = 0;
    string past = "";

    while (getline(file, word))
    {
        words.push_back(word);
    }

    //error out if there's no second word to switch to
    if (words.size() < 2){
        cout << "ERROR: NOT ENOUGH WORDS IN DATABASE" << "\n";

        for (int i=3; i > 0; i--){
            cout << "\n" << "Closing in " << i;
            this_thread::sleep_for(chrono::seconds(1));
        }
        return 1;
    }

    // screensaver loop
    while (true)
    {

        // these prevent us from having to recalculate in the loop needlessly
        int randomIndex = rand() % words.size();
        string current = words[randomIndex];

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
        
        // guardian pattern

        if (current.length() < 2 || current == "" || outputWord[0] == ' ' || outputWord[1] == ' ')
        {
            continue;
        }


        system("cls");
        randomBuffer = (rand() % 3) + 1;

        screensaver::printBuffer(9);

        std::cout << outputWord;
        
        screensaver::printBuffer(randomBuffer);

        if (screensaver::sleepLoopExited(30))
        {
            system("cls");
            screensaver::printBuffer(9);
            std::cout << "\t\u001b[0m\u001b[4m" << "Welcome";
            std::cout << "\u001b[0m";
            screensaver::printBuffer(3);
            this_thread::sleep_for(chrono::milliseconds(250));
            return 0;
        }
    }
}



void setSplitLogic(string current,char outputWord[])
{
        cstring COL1 = "\u001b[35m";
        cstring COL2 = "\u001b[32m";
        cstring COL3 = "\u001b[34m";
        cstring COL4 = "\u001b[97m";
        cstring COL5 = "\u001b[31m";
        cstring COL6 = "\u001b[95m";
        cstring COL7 = "\u001b[33m";
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
        outputWord[0] = ' ';
        int totalLen = current.length();
        cstring totalWordArr = current.c_str();

        /*
            CASE 0: Don't even process it if there's whitespace to begin with,
            that should've been stripped with the textformatter.py
        */
        if (current[0] == ' ' || current[0] == '\t' || current[0] == '\r'){
            ; //empty block on purpose
        }

        /*
         *   CASE 1: LINE IS REALLY LONG JUST PRINT NORMALLY
         */

        else if (totalLen > LENGTH_LIMIT)
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
                cstring p1Array = firstPart.c_str();
                cstring p2Array = lastPart.c_str();
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
            else
            {
                 sprintf(outputWord, "%s%15s", COL4, totalWordArr);
            }
        }
    }
