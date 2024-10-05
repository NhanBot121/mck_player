#include "Player.hpp"

#include <filesystem>
#include <iostream>
#include <chrono>
#include <mutex>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>


std::mutex terminalMutex;  // Protects terminal access between threads

Player::Player() : curr(nullptr), is_playing(false), is_displaying(false), stopFlag(false), is_auto_next(false){
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
    }

    volume.setVolume(64);
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

            // playback info
            curr_played_time = 0;

            curr_metadata = Metadata(*curr);

            curr_title = curr_metadata.get_title();
            curr_duration = curr_metadata.get_duration();

            while (Mix_PlayingMusic() && !stopFlag && is_playing) {
                //SDL_Delay(100);  // Small delay to prevent busy-waiting
                std::this_thread::sleep_for(std::chrono::seconds(1)); // wait for 1 second
                if (!is_displaying && !Mix_PausedMusic()) {  // counting in background when not displaying
                    ++curr_played_time;
                }
            }
            is_playing = false;
            if (is_auto_next) {
                next();
            }
        }
        Mix_FreeMusic(music);
    }
}


void Player::displayPlayBackInfo() {
    if (is_playing || Mix_PausedMusic()) {
        is_displaying = true;

        // Set terminal to non-blocking mode
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);  // Save current terminal settings
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // Apply new terminal settings

        // Set input mode to non-blocking
        int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        // Display playback info in a loop
        while (is_displaying && curr_played_time <= curr_duration) {  // Exit when is_displaying is false or playback finishes
            // Clear the screen to refresh the display
            std::cout << "\033[2J\033[1;1H";  // ANSI escape codes to clear the screen and move the cursor

            // Display metadata in a structured way
            std::cout << "Title: " << curr_title << "\n";
            std::cout << "Artist: " << curr_metadata.get_artist() << "\n";
            std::cout << "Album: " << curr_metadata.get_album() << "\n";
            std::cout << "Genre: " << curr_metadata.get_genre() << "\n";
            std::cout << "Playback Time: " << curr_played_time << " / " << curr_duration << " seconds\n";
            volume.printVolume();
            std::string auto_ = (is_auto_next ? "on" : "off");
            std::cout << "Auto next: " << auto_ << std::endl;; 
            std::cout << "| [p]ause | [r]esume | [q]uit |\n"; 
            //std::cout << "| [->] next | [<-] prev | [+] volume up | [-] volume down |\n";
            std::cout.flush();  // Ensure the line updates correctly

            // Check if a key was pressed
            char c;
            if (read(STDIN_FILENO, &c, 1) > 0) {
                switch (c) {
                    case 'p':
                        std::cout << "\nPausing playback...\n";
                        pause();
                        break;
                    case 'r':
                        std::cout << "\nResuming playback...\n";
                        resume();
                        break;
                    case 27: {  // Escape sequence for arrow keys
                        char seq[2];
                        if (read(STDIN_FILENO, &seq[0], 1) > 0 && read(STDIN_FILENO, &seq[1], 1) > 0) {
                            if (seq[0] == '[') {
                                if (seq[1] == 'C') {  // Right arrow key
                                    std::cout << "\nSkipping to the next track...\n";
                                    next();
                                }
                                else if (seq[1] == 'D') {  // Left arrow key
                                    std::cout << "\nGoing to the previous track...\n";
                                    prev();
                                }
                            }
                        }
                        break;
                    }
                    case '=': {
                        volume.upVolume();
                        break;
                    }
                    case '-': {
                        volume.downVolume();
                        break;
                    }
                    case 'q':
                        std::cout << "\nExiting playback...\n";
                        is_displaying = false;  // Signal loop to stop
                        break;
                    default:
                        std::cout << "\nUnknown command.\n";
                        break;
                }
            }

            // Simulate playback progress
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (!Mix_PausedMusic()) {
                ++curr_played_time;  // Update the playback time if not paused
            }
        }

        // Clear the line after playback finishes
        std::cout << std::endl;

        // Restore terminal settings
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Restore old terminal settings
        fcntl(STDIN_FILENO, F_SETFL, oldf);  // Restore old input mode
    } 
    else {
        std::cout << "No track playing." << std::endl;
        return;
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
        volume.printVolume();
    }
}

void Player::play_track(int i) {
    curr = &playlistToPlay[i];
    if (isCurrValid()) {
        startAudioThread(*curr);
    } else {
        std::cout << "Invalid action. \n";
    }
}

void Player::stopAudioThread() {
    stopFlag = true;
    is_playing = false;
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
        //is_playing = true;
        std::cout << "Audio resumed." << std::endl;
    } else {
        std::cout << "No paused audio to resume." << std::endl;
    }
}


bool Player::isPlaying()
{
    return is_playing;
}

Metadata Player::getMetadata()
{
    return curr_metadata;
}

Volume Player::getVolume()
{
    return volume;
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
    playlistToPlay = {};
    for (const auto& path : inDir) {
        playlistToPlay.push_back(path.string());
    }
    curr = &playlistToPlay[0];
}

bool Player::isCurrValid() const {
    return (curr && curr >= &playlistToPlay[0] && curr <= &playlistToPlay.back());
}