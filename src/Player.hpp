#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>

class Player {
    std::vector<std::string> playlist;

    bool auto_next = false;
    bool is_playing = false;

public: 
    static void playAudio(const std::string& fileName);
    static void pauseAudio();
    static void resumeAudio();

    static void playVideo(const std::string& fileName);

    static void playPlaylist(const std::string& playlist);

    static void play();
    static void pause();
    static void prev();
    static void next();
    static bool autoNext(bool is_auto_on);
};

#endif