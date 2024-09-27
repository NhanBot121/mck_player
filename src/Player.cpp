#include "Player.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

// just take the absolute path for now

void Player::playAudio(const std::string &fileName)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    // Load the audio file
    Mix_Music* music = Mix_LoadMUS(fileName.c_str()); // Use c_str() to convert std::string to const char*
    if (!music) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
    } else {
        // Play the music
        if (Mix_PlayMusic(music, 1) == -1) {
            std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
        } else {
            std::cout << "Playing: " << fileName << std::endl;
            SDL_Delay(5000); // Play for 5 seconds
        }
        Mix_FreeMusic(music); // Free the music after playing
    }

    // Clean up and quit SDL
    Mix_CloseAudio();
    SDL_Quit();
}

void Player::playVideo(const std::string &fileName)
{
    return;
}

void Player::playPlaylist(const std::string &playlist)
{
}

void Player::play()
{
}

void Player::pause()
{
}

void Player::prev()
{
}

void Player::next()
{
}

bool Player::autoNext(bool is_auto_on)
{
    return false;
}
