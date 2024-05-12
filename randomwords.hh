#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <windows.h>

#ifndef RANDOMWORDS_HH
#define RANDOMWORDS_HH



using std::string;
using vectors = std::vector<std::string>;
using cstring = const char *;

class wordScreensaver{

private:
    const cstring COL1 = "\u001b[35m";
    const cstring COL2 = "\u001b[32m";
    const cstring COL3 = "\u001b[34m";
    const cstring COL4 = "\u001b[97m";
    const cstring COL5 = "\u001b[31m";
    const cstring COL6 = "\u001b[95m";
    const cstring COL7 = "\u001b[33m";
    // lengths to set conditions with
    const int WRAP_LEN = 18;
    const int LENGTH_LIMIT = 44;
    const int FIRSTP_LIMIT = 22;
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

    vectors words;
    std::string word = "";
    cstring filename = "words.txt";
    std::ifstream file;
    int randomBuffer = 0;
    string past = "";
    int fontsize = 100;

    void fillwords();

    void setfontsize(int fontsize);

    void setConsoleFullscreen();
  
    void greet();

    void setSplitLogic(string current, char outputWord[]);

    int sleepLoopExited(int n);
    
    void printBuffer(int n);

    void setFilename(cstring filename);

public:

    wordScreensaver(int fontSize, cstring filename);
    
    cstring getFilename();

    int run();
    
};

#endif 