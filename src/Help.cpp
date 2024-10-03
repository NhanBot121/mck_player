#include "Help.hpp"
#include <iostream>

void Help::allHelp()
{
    std::cout << "Available commands:\n";
    
    // General commands
    std::cout << "  help                 : Show this help menu\n";
    std::cout << "  q                 : Exit the application\n";
    std::cout << "  ls                   : List media files in the current directory\n\n";
    
    // Playlist commands
    Help::playlistHelp();

    // Metadata commands
    Help::metadataHelp();

    // Player commands
    Help::playerHelp();

    // Volume commands
    Help::volumeHelp();

    std::cout << "\nType 'help <command>' for detailed information on a specific command.\n";
}

void Help::playlistHelp()
{
    // Playlist commands
    std::cout << "Playlist commands:\n";
    std::cout << "  playlist -l          : List all playlists\n";
    std::cout << "  playlist -v <name>   : View contents of the playlist\n";
    std::cout << "  playlist -c <name>   : Create a new playlist\n";
    std::cout << "  playlist -u <name>   : Update a playlist (add/remove media)\n";
    std::cout << "  playlist -d <name>   : Delete a playlist\n\n";
}

void Help::metadataHelp()
{
    // Metadata commands
    std::cout << "Metadata commands:\n";
    std::cout << "  metadata -v <file>   : View metadata of a media file\n";
    std::cout << "  metadata -e <file>   : Edit metadata of a media file\n\n";
}

void Help::playerHelp()
{
    // Player commands
    std::cout << "Player commands:\n";
    std::cout << "  player -pl <name>    : Load and play a playlist\n";
    std::cout << "  player -cwd          : Load and play media from current directory\n";
    std::cout << "  player -p            : Pause playback\n";
    std::cout << "  player -r            : Resume playback\n";
    std::cout << "  player --display     : Display current playback information\n";
    std::cout << "  player --next        : Play the next media file\n";
    std::cout << "  player --prev        : Play the previous media file\n";
    std::cout << "  player --stop        : Stop playback\n";
    std::cout << "  player --auto-on     : Enable auto-play of the next media file\n";
    std::cout << "  player --auto-off    : Disable auto-play of the next media file\n";
    std::cout << "  player --view-tag    : View metadata of the current media file\n";
    std::cout << "  player --edit-tag    : Edit metadata of the current media file\n\n";
}

void Help::volumeHelp()
{
    std::cout << "Volume commands:\n";
    std::cout << "  volume               : Display current volume level\n";
    std::cout << "  volume ++            : Increase volume\n";
    std::cout << "  volume --            : Decrease volume\n";
    std::cout << "  volume --set <level> : Set volume to a specific level\n";
}
