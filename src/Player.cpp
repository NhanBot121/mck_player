#include "Player.hpp"
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <mutex>

std::mutex terminalMutex;  // Protects terminal access between threads

Player::Player() : curr(nullptr), is_playing(false), is_displaying(false), stopFlag(false), is_auto_next(false) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
    }
}

Player::~Player() {
    stopAudioThread();
    Mix_CloseAudio();
    SDL_Quit();
}

void Player::playAudio(const std::string& fileName) {
    Mix_Music* music = Mix_LoadMUS(fileName.c_str());
    if (!music) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
    } else {
        if (Mix_PlayMusic(music, 1) == -1) {
            std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
        } else {
            is_playing = true;
            while (Mix_PlayingMusic() && !stopFlag) {
                SDL_Delay(100);  // Small delay to prevent busy-waiting
            }
            is_playing = false;
            if (is_auto_next) {
                next();
            }
        }
        Mix_FreeMusic(music);
    }
}

void Player::startAudioThread(const std::string& fileName) {
    stopAudioThread();  // Stop any currently playing audio
    stopFlag = false;

    audioThread = std::thread([this, fileName] {
        playAudio(fileName);
    });

    audioThread.detach();  // Detach the thread
}

void Player::play() {
    if (isCurrValid()) {
        startAudioThread(*curr);
    }
}

void Player::stopAudioThread() {
    stopFlag = true;
    if (audioThread.joinable()) {
        audioThread.join();  // Wait for the audio thread to finish
    }
}


// Pause the currently playing audio
void Player::pause() {
    if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
        Mix_PauseMusic();
        std::cout << "Audio paused." << std::endl;
    } else {
        std::cout << "No audio to pause or already paused." << std::endl;
    }
}

// Resume the paused audio
void Player::resume() {
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
        std::cout << "Audio resumed." << std::endl;
    } else {
        std::cout << "No paused audio to resume." << std::endl;
    }
}

bool Player::isPlaying()
{
    return is_playing;
}

void Player::stop() {
    stopAudioThread();
}

void Player::next() {
    if (isCurrValid() && curr != &playlistToPlay.back()) {
        ++curr;
        startAudioThread(*curr);
    } else {
        std::cout << "Already at the last song." << std::endl;
    }
}

void Player::prev() {
    if (isCurrValid() && curr != &playlistToPlay[0]) {
        --curr;
        startAudioThread(*curr);
    } else {
        std::cout << "Already at the first song." << std::endl;
    }
}

void Player::auto_next(bool option)
{   
    is_auto_next = option;
}

void Player::loadPlaylist(const std::string& playlistName) {
    std::string playlistPath = PLAYLIST_DIR + playlistName + ".txt";
    std::ifstream file(playlistPath);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                playlistToPlay.push_back(line);
            }
        }
        file.close();
        curr = &playlistToPlay[0];  // Set the first song as the current
    } else {
        std::cerr << "Failed to open playlist: " << playlistPath << std::endl;
    }
}

void Player::loadInDir() {
    playlistToPlay = inDir;
    curr = &playlistToPlay[0];
}

bool Player::isCurrValid() const {
    return (curr && curr >= &playlistToPlay[0] && curr <= &playlistToPlay.back());
}