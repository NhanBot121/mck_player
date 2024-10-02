#include <iostream>
#include <string>
#include <sstream>
#include "Browser.hpp"
#include "Playlist.hpp"
#include "Metadata.hpp"
#include "Player.hpp"
#include "Volume.hpp"

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
std::atomic<bool> stopDisplay(false); // Flag to stop display thread

// Function to display real-time info of currently playing track
// void displayRealTimeInfo(Player& player) {
//     while (!stopDisplay) {
//         if (player.isPlaying()) {
//             mtx.lock(); // lock to prevent interface conflict with input thread
//             player.displayPlaybackInfo();
//             mtx.unlock();
//         }
//         std::this_thread::sleep_for(std::chrono::seconds(1)); // update every second
//     }
// }

int main() 
{
    std::string input;
    std::cout << "MCK Player. Type 'exit' to quit." << std::endl;

    Browser browser;
    Player& player = Player::getInstance();

    // Start the real-time display thread
    //std::thread displayThread(displayRealTimeInfo, std::ref(player));

    // Main loop for handling user input
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
        }
        else if (command == "metadata") {
            if (args.size() == 3) {
                std::string action = args[1];
                if (action == "-v") {
                    mtx.lock();
                    Metadata::viewMetadata(args[2]);
                    mtx.unlock();
                } else if (action == "-e") {
                    mtx.lock();
                    Metadata::editMetadata(args[2]);
                    mtx.unlock();
                } 
            }
        }
        else if (command == "player") {
            if (args.size() >= 2) {
                std::string action = args[1];
                if (action == "-pl" && args.size() >= 3) {
                    mtx.lock();
                    player.loadPlaylist(args[2]);
                    player.play();
                    mtx.unlock();
                }
                else if (action == "-cwd") {
                    mtx.lock();
                    player.loadInDir();
                    player.play();
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
                else if (action == "--pause") {
                    mtx.lock();
                    player.pause();
                    mtx.unlock();
                }
                else if (action == "--resume") {
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
            }
        }
        else if (command == "volume") {
            if (args.size() == 2) {
                if (args[1] == "up") {
                    mtx.lock();
                    Volume::upVolume();
                    mtx.unlock();
                } else if (args[1] == "down") {
                    mtx.lock();
                    Volume::downVolume();
                    mtx.unlock();
                }
            }
        }
        else {
            mtx.lock();
            std::cout << "Unknown command. Please try again." << std::endl;
            mtx.unlock();
        }
    }

    // stopDisplay = true;
    // if (displayThread.joinable()) {
    //     displayThread.join();
    // }

    return 0;
}
