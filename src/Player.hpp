#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <filesystem>
#include "Browser.hpp"

#include <taglib/fileref.h>
#include <taglib/tag.h>

// Constant paths
const std::string PLAYLIST_DIR = "/home/nhnbot21/ndrd/c_cpp/cdr/mck_player/src/playlists/";
const std::string CW_DIR = std::filesystem::current_path();

// Singleton Player class
class Player {
public:
    // Static method to access Singleton instance
    static Player& getInstance() {
        static Player instance;
        return instance;
    }

    // Delete copy constructor and assignment operator to enforce Singleton pattern
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    // Audio control methods
    void play();        // Play the current track
    void pause();       // Pause the current track
    void resume();      // Resume the paused track
    void start_counter();
    void stop();        // Stop playback
    void next();        // Play the next track
    void prev();        // Play the previous track
    void auto_next(bool is_on);  // Enable/disable auto-next functionality

    // Playlist and directory methods
    void loadPlaylist(const std::string& playlistName);  // Load a playlist from file
    void loadInDir();  // Load media files from the directory

    // Display and playback info
    void display();    // Display playback info
    void playbackInfo();  // Display detailed info about current playback

    void displayPlayBackInfo();

    // Utility methods
    bool isPlaying();  // Check if music is currently playing

private:
    // Private constructor to enforce Singleton
    Player();
    ~Player();  // Private destructor to clean up resources

    // Audio-related private functions
    void playAudio(const std::string& fileName);  // Play audio from file
    void playbackInfo(const std::string &fileName);
    void stopAudioThread(); // Stop the audio playback thread
    void stopInfoThread();  // Stop the info display thread

    void startAudioThread(const std::string& fileName);  // Start audio playback in a new thread
    void startInfoThread();  // Start info display in a new thread

    // Helper methods
    bool isCurrValid() const;  // Check if the current song is valid

    // Data members related to playlist
    std::vector<std::string> playlistToPlay;  // List of songs to play
    std::string* curr;  // Pointer to the currently playing song

    // Threads for audio playback and info display
    std::thread audioThread;
    std::thread infoThread;

    // Flags for controlling playback and thread safety
    std::atomic<bool> is_playing{false};  // Is a song currently playing?
    std::atomic<bool> is_displaying{false};  // Is playback info being displayed?

    std::atomic<bool> stopFlag{false};  // Stop flag for controlling audio and info threads
    std::atomic<bool> is_auto_next{true};  // Is auto-next enabled?

    // Browser instance for directory handling
    Browser browser;  // Browser object to browse media files
    std::vector<std::string> inDir = browser.inDirMedia;  // List of media files in the directory

    int curr_duration;
    int curr_played_time;
    TagLib::String curr_title;
};

#endif // PLAYER_HPP
