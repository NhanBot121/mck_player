#include "Playlist.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>

const std::string PLAYLIST_DIR = "/home/mandane_21/wrk/mck_player/src/playlists/";

// List all playlists
void Playlist::listPlaylists() {
    for (const auto& entry : std::filesystem::directory_iterator(PLAYLIST_DIR)) {
        std::cout << entry.path().stem().string() << std::endl;
    }
}

// View the current playlist, printing only the filenames
void Playlist::viewPlaylist(const std::string& playlistName) {
    std::string playlistPath = PLAYLIST_DIR + playlistName + ".txt";
    std::ifstream file(playlistPath); // Open the playlist
    if (!file.is_open()) {
        std::cerr << "Unable to open the playlist: " << playlistName << std::endl;
        return;
    }

    std::string mediaPath;
    int idx = 0;
    while (std::getline(file, mediaPath)) {
        std::cout << ++idx << ". " << std::filesystem::path(mediaPath).stem() << std::endl; // Print only the filename
    }

    file.close();
}

// Create an empty playlist
void Playlist::createPlaylist(const std::string& playlistName) {
    std::string playlistPath = PLAYLIST_DIR + playlistName + ".txt";
    
    // Check if the playlist already exists
    if (!std::filesystem::exists(playlistPath)) {
        std::ofstream file(playlistPath); // Try to create the file
        
        if (file.is_open()) { // Check if the file was created successfully
            std::cout << "Playlist '" << playlistName << "' created." << std::endl;
            file.close(); // Close the file after creating
        } else {
            std::cerr << "Failed to create playlist '" << playlistName << "'." << std::endl;
        }
    } else {
        std::cerr << "Playlist '" << playlistName << "' already exists." << std::endl;
    }
}

// Read media files in a playlist to a vector provided its path
std::vector<std::string> Playlist::readPlaylist(const std::string& playlistName) {
    std::string playlistPath = PLAYLIST_DIR + playlistName + ".txt";
    std::vector<std::string> lines;
    std::ifstream file(playlistPath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    }
    return lines;
}

// Update the playlist (add/remove media)
void Playlist::updatePlaylist(const std::string& playlistName) {
    std::string playlistPath = PLAYLIST_DIR + playlistName + ".txt";
    if (!std::filesystem::exists(playlistPath)) {
        std::cout << "Playlist '" << playlistName << "' does not exist." << std::endl;
    } else {
        while (true) {
            std::cout << "\nUpdate actions: [a]dd, [r]emove, [q]uit\n";
            std::cout << "Enter your choice: ";
            char choice;
            std::cin >> choice;

            if (choice == 'a') {
                std::string mediaPath;
                std::cout << "\nEnter the media file to add to the playlist: ";
                std::cin.ignore();
                getline(std::cin, mediaPath);
                // Add logic to append mediaPath to the playlist
                std::ofstream outFile(playlistPath, std::ios::app);
                if (outFile.is_open()) {
                    outFile << mediaPath << std::endl;
                    outFile.close();
                }
            } else if (choice == 'r') {
                std::string mediaPath;
                std::cout << "\nEnter media file to remove from the playlist: ";
                std::cin.ignore();
                getline(std::cin, mediaPath);
                // Add logic to remove mediaPath from the playlist
                std::ifstream file(playlistPath);
                std::vector<std::string> lines;
                std::string line;

                while (std::getline(file, line)) {
                    if (line != mediaPath) {
                        lines.push_back(line); 
                    }
                }

                file.close();
                std::ofstream outFile(playlistPath);
                for (const auto& updatedLine : lines) {
                    outFile << updatedLine << std::endl;
                }
            } else if (choice == 'q') {
                std::cout << "\nExit updating playlist..." << std::endl;
                break;
            }
        }
    }
}

// Delete the playlist
void Playlist::deletePlaylist(const std::string& playlistName) {
    std::string fullPath = PLAYLIST_DIR + playlistName + ".txt";

    // Check if the playlist file exists
    if (!std::filesystem::exists(fullPath)) {
        std::cout << "Playlist '" << playlistName << "' does not exist." << std::endl;
        return;
    }

    // Try to remove the file
    try {
        if (std::filesystem::remove(fullPath)) {
            std::cout << "Playlist '" << playlistName << "' has been deleted successfully." << std::endl;
        } else {
            std::cerr << "Failed to delete the playlist '" << playlistName << "'." << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error deleting playlist '" << playlistName << "': " << e.what() << std::endl;
    }
}
