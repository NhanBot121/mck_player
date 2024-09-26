#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <string>

class Playlist {

public:
    static void listPlaylists();
    static void viewPlaylist(std::string pname);
    static void createPlaylist(std::string pname);
    static void updatePlaylist(std::string pname);
    static void deletePlaylist(std::string pname);
};

#endif