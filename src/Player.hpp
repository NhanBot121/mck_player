#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player {
public: 
    static void play(std::string fname);
    static void play();
    static void pause();
    static void prev();
    static void next();
    static bool autoNext(bool is_auto_on);
};

#endif