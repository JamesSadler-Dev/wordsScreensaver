#include "randomwords.hh"


    int main(){
        wordScreensaver scr = wordScreensaver(90,"words.txt");
        scr.run();
        return 0;
    }