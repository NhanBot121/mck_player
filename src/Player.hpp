#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player {
public: 
    void play(std::string fname);
    void play();
    void pause();
    void prev();
    void next();
    bool autoNext(bool is_auto_on);
};

#endif