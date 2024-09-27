#include "Playlist.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>

const std::string PLAYLIST_DIR = "/home/nhnbot21/ndrd/c_cpp/cdr/mck_player/src/playlists/";

// list all playlists
void Playlist::listPlaylists()
{
    for (const auto& entry : std::filesystem::directory_iterator(PLAYLIST_DIR)) {
        std::cout << entry.path().filename().string() << std::endl;
    }
}

void Playlist::viewPlaylist(std::string playlistName)
{
    std::ifstream file(playlistName); // open the playlist
    if (!file.is_open()) {
        std::cerr << "Unable to open the playlist: " << playlistName << std::endl;
        return;
    }

    std::string mediaPath;
    while (std::getline(file, mediaPath)) {
        std::cout << mediaPath << std::endl;
    }

    file.close();
}

// Create empty 
void Playlist::createPlaylist(std::string playlistName) {
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

// Read media files in a playlist to a vector
std::vector<std::string> readPlaylist(const std::string& playlistName) {
    std::vector<std::string> lines;
    std::ifstream file(playlistName);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    }
    return lines;
}

// Function to check if a path exists in a playlist file
bool isMediaInPlaylist(const std::string& playlistName, const std::string& mediaPath) {
    auto lines = readPlaylist(playlistName);
    return std::find(lines.begin(), lines.end(), mediaPath) != lines.end();
}

void addToPlaylist(const std::string& playlistName, const std::string& mediaPath) {
    std::string playlistPath = PLAYLIST_DIR + playlistName + ".txt";

    if (!isMediaInPlaylist(playlistPath, mediaPath)) {
        std::ofstream file(playlistPath, std::ios::app); // Open file in append mode
        if (file.is_open()) {
            file << mediaPath << std::endl;
            file.close();
        }
    } else {
        std::cout << "Media already exists in playlist: " << playlistName << std::endl;
    }
}

void removeFromPlaylist(const std::string& playlistName, const std::string& mediaPath) {
    std::string playlistPath = PLAYLIST_DIR + playlistName + ".txt";

    if (!isMediaInPlaylist(playlistPath, mediaPath)) {
        std::cout << "Media not in playlist '" << playlistName << "'." << std::endl;
        return;
    }

    std::ifstream file(playlistPath);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << playlistName << std::endl;
        return;    
    }

    std::vector<std::string> lines;
    std::string line;

    // Read all lines from the files into a vector
    // excluding the mediaPath to be removed
    while (std::getline(file, line)) {
        if (line != mediaPath) {
            lines.push_back(line); 
        }
    }

    file.close();

    // Write the updated lines back to the file
    std::ofstream outFile(playlistPath);
    if (!outFile.is_open()) {
        std::cerr << "Unable to open file for writing: " << playlistName << std::endl;
        return;
    }

    for (const auto& updatedLine : lines) {
        outFile << updatedLine << std::endl;
    }

    std::cout << "Media removed from playlist '" << playlistName << "'." << std::endl;
}

void Playlist::updatePlaylist(std::string playlistName)
{
    std::string playlistPath = PLAYLIST_DIR + playlistName + ".txt";
    if (!std::filesystem::exists(playlistPath)) {
        std::cout << "Playlist '" << playlistName << "' not exists.";
    } else {
        while (true) {
            std::cout << "\nUpdate actions: [a]dd, [r]emove, [q]uit\n";
            std::cout << "Enter your choice: ";
            char choice;
            std::cin >> choice;

            if (choice == 'a') {
                std::string mediaPath;
                std::cout << "\nEnter the media file to add in playlist: ";
                std::cin.ignore();
                getline(std::cin, mediaPath);
                addToPlaylist(playlistName, mediaPath);
            } else if (choice == 'r') {
                std::string mediaPath;
                std::cout << "\nEnter media file to be remove from playlist: ";
                std::cin.ignore();
                getline(std::cin, mediaPath);
                removeFromPlaylist(playlistName, mediaPath);
            } else if (choice == 'q') {
                std::cout << "\nExit updating playlist..." << std::endl;
                break;
            }
        }
    }
}

void Playlist::deletePlaylist(std::string playlistName) {
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
