#include <iostream>
#include <string>
#include <sstream>
#include "Directory.hpp"
#include "Playlist.hpp"
#include "Metadata.hpp"
#include "Player.hpp"
#include "Volume.hpp"

#include <vector>

// Function to split input into arguments
std::vector<std::string> splitInput(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }
    return args;
}

int main() {
    std::string input;
    std::cout << "MCK Player. Type 'exit' to quit." << std::endl;

    // Main loop
    while (true) {
        std::cout << "> ";  // Prompt symbol
        std::getline(std::cin, input);  // Get user input

        if (input.empty()) continue;  // Ignore empty input

        // Split the input into command and arguments
        std::vector<std::string> args = splitInput(input);
        std::string command = args[0];

        if (command == "exit") {
            std::cout << "Exiting the application..." << std::endl;
            break;  // Exit the loop to terminate the application
        }
        else if (command == "ls" && args.size() == 1) {
            Directory::listDirectory();
        }
        else if (command == "playlist") {
            if (args.size() >= 2) {
                std::string action = args[1];
                if (action == "-l") {
                    Playlist::listPlaylists();
                } else if (action == "-v" && args.size() == 3) {
                    Playlist::viewPlaylist(args[2]);
                } else if (action == "-c" && args.size() == 3) {
                    Playlist::createPlaylist(args[2]);
                } else if (action == "-u" && args.size() == 3) {
                    Playlist::updatePlaylist(args[2]);
                } else if (action == "-d" && args.size() == 3) {
                    Playlist::deletePlaylist(args[2]);
                }
            }
        }
        else if (command == "metadata") {
            if (args.size() == 3) {
                std::string action = args[1];
                if (action == "-view") {
                    Metadata::viewMetadata(args[2]);
                } else if (action == "--change") {
                    Metadata::changeMetadata(args[2]);
                } else if (action == "--add") {
                    Metadata::addMetadata(args[2]);
                }
            }
        }
        else if (command == "play") {
            if (args.size() == 2) {
                Player::play(args[1]);
            } else {
                Player::play();
            }
        }
        else if (command == "pause") {
            Player::pause();
        }
        else if (command == "next") {
            Player::next();
        }
        else if (command == "prev") {
            Player::prev();
        }
        else if (command == "auto") {
            if (args.size() == 2 && (args[1] == "--on" || args[1] == "--off")) {
                Player::autoNext(args[1] == "--on"); // boolean ?
            }
        }

        else if (command == "volume") {
            if (args.size() == 2) {
                if (args[1] == "up") {
                    Volume::upVolume();
                } else if (args[1] == "down") {
                    Volume::downVolume();
                }
            }
        }
        else {
            std::cout << "Unknown command. Please try again." << std::endl;
        }
    }

    return 0;
}