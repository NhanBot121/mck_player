#include <iostream>
#include <string>
#include <sstream>
#include "Browser.hpp"
#include "Playlist.hpp"
#include "Metadata.hpp"
#include "Player.hpp"
#include "Volume.hpp"
#include "Help.hpp"

#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

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

std::mutex mtx; // Mutex for synchronizing output

int main() 
{
    std::string input;
    std::cout << "[Multimedia-Cpp-Kernel] MCK PLAYER." << std::endl;
    std::cout << "Type 'q' to quit." << std::endl;
    std::cout << "Type 'help' for user manual" << std::endl;

    Browser browser;
    Player& player = Player::getInstance();
    //Volume volume;

    // Main loop for handling user input
    while (true) {
        std::cout << "> ";  // Prompt symbol
        std::getline(std::cin, input);  // Get user input

        if (input.empty()) continue;  // Ignore empty input

        // Split the input into command and arguments
        std::vector<std::string> args = splitInput(input);
        std::string command = args[0];

        if (command == "q") {
            std::cout << "Exiting the application..." << std::endl;
            break;  // Exit the loop to terminate the application
        }
        else if (command == "ls" && args.size() == 1) {
            mtx.lock();
            browser.listDirectory();
            mtx.unlock();
        }
        else if (command == "playlist") {
            if (args.size() >= 2) {
                std::string action = args[1];
                if (action == "-l") {
                    mtx.lock();
                    Playlist::listPlaylists();
                    mtx.unlock();
                } else if (action == "-v" && args.size() == 3) {
                    mtx.lock();
                    Playlist::viewPlaylist(args[2]);
                    mtx.unlock();
                } else if (action == "-c" && args.size() == 3) {
                    mtx.lock();
                    Playlist::createPlaylist(args[2]);
                    mtx.unlock();
                } else if (action == "-u" && args.size() == 3) {
                    mtx.lock();
                    Playlist::updatePlaylist(args[2]);
                    mtx.unlock();
                } else if (action == "-d" && args.size() == 3) {
                    mtx.lock();
                    Playlist::deletePlaylist(args[2]);
                    mtx.unlock();
                }
            }
            else {
                std::cout << "Invalid command, type 'help playlist' for reference." << std::endl;
            }
        }
        else if (command == "metadata") {
            if (args.size() == 3) {
                Metadata metadata(args[2]);
                std::string action = args[1];
                if (action == "-v") {
                    mtx.lock();
                    metadata.viewMetadata();
                    mtx.unlock();
                } else if (action == "-e") {
                    mtx.lock();
                    metadata.editMetadata();
                    mtx.unlock();
                } 
            }
            else {
                std::cout << "Invalid command, type 'help metadata' for reference." << std::endl;
            }
        }
        else if (command == "player") {
            if (args.size() >= 2) {
                std::string action = args[1];
                if (action == "-pl") {
                    mtx.lock();
                    player.loadPlaylist(args[2]);
                    Playlist::viewPlaylist(args[2]);
                    // // player -pl <playlist_name> <track_number>
                    // if (args.size() == 4) {
                    //     mtx.lock();
                    //     player.stop();
                    //     int i = std::stoi(args[3]) - 1;
                    //     player.play_track(i);    
                    //     mtx.unlock();
                    // }
                    player.play();
                    mtx.unlock();
                }
                else if (action == "-cwd") {
                    mtx.lock();
                    player.loadInDir();
                    browser.listDirectory();
                    // player --cwd <track_number>
                    // if (args.size() == 3) {
                    //     mtx.lock();
                    //     player.stop();
                    //     int i = std::stoi(args[2]) - 1;
                    //     player.play_track(i);
                    //     mtx.unlock();
                    // }
                    player.play();
                    mtx.unlock();
                }
                else if (action == "--track" && args.size() == 3) {
                    mtx.lock();
                    int i = std::stoi(args[2]) - 1;
                    player.play_track(i);
                    mtx.unlock();
                }
                else if (action == "--display") {
                    mtx.lock();
                    player.displayPlayBackInfo();
                    mtx.unlock();
                }
                else if (action == "--next") {
                    mtx.lock();
                    player.next();
                    mtx.unlock();
                }
                else if (action == "--prev") {
                    mtx.lock();
                    player.prev();
                    mtx.unlock();
                }
                else if (action == "-p") {
                    mtx.lock();
                    player.pause();
                    mtx.unlock();
                }
                else if (action == "-r") {
                    mtx.lock();
                    player.resume();
                    mtx.unlock();
                }
                else if (action == "--stop") {
                    mtx.lock();
                    player.stop();
                    mtx.unlock();
                }
                else if (action == "--auto-on") {
                    mtx.lock();
                    player.auto_next(true);
                    mtx.unlock();
                }
                else if (action == "--auto-off") {
                    mtx.lock();
                    player.auto_next(false);
                    mtx.unlock();
                }
                else if (action == "--view-tag") {
                    mtx.lock();
                    player.getMetadata().viewMetadata();
                    mtx.unlock();
                }
                else if (action == "--edit-tag") {
                    mtx.lock();
                    player.getMetadata().editMetadata();
                    mtx.unlock();
                }
                else {
                    std::cout << "Invalid command" << std::endl;
                }
            }
            else {
                std::cout << "Invalid command, type 'help player' for reference." << std::endl;
            }
        }
        else if (command == "volume") {
            if (args.size() == 1) {
                mtx.lock();
                player.getVolume().printVolume();
                mtx.unlock();
            }
            else if (args.size() >= 2) {
                if (args[1] == "++") {
                    mtx.lock();
                    player.getVolume().upVolume();
                    player.getVolume().printVolume();
                    mtx.unlock();
                } else if (args[1] == "--") {
                    mtx.lock();
                    player.getVolume().downVolume();
                    player.getVolume().printVolume();
                    mtx.unlock();
                } else if (args[1] == "--set") {
                    mtx.lock();
                    int vol = std::stoi(args[2]);
                    player.getVolume().setVolume(vol);
                    player.getVolume().printVolume();
                    mtx.unlock();
                }
            }
            else {
                std::cout << "Invalid command, type 'help volume' for reference." << std::endl;
            }
        }
        else if (command == "help") {
            if (args.size() == 1) {
                mtx.lock();
                Help::allHelp();
                mtx.unlock();
            }
            else if (args.size() == 2) {
                std::string func = args[1];
                if (func == "playlist") {
                    mtx.lock();
                    Help::playlistHelp();
                    mtx.unlock();
                }
                else if (func == "metadata") {
                    mtx.lock();
                    Help::metadataHelp();
                    mtx.unlock();
                }
                else if (func == "player") {
                    mtx.lock();
                    Help::playerHelp();
                    mtx.unlock();
                }
                else if (func == "volume") {
                    mtx.lock();
                    Help::volumeHelp();
                    mtx.unlock();
                }
            }
        }
        else {
            mtx.lock();
            std::cout << "Unknown command. Please try again. Type 'help' to know more." << std::endl;
            mtx.unlock();
        }
    }

    return 0;
}
