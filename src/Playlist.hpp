#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <string>
#include <vector>

class Playlist {

public:
    static void listPlaylists();
    static void viewPlaylist(const std::string& playlistName);
    static void createPlaylist(const std::string& playlistName);
    static void updatePlaylist(const std::string& playlistName);
    static void deletePlaylist(const std::string& playlistName);

    static std::vector<std::string> readPlaylist(const std::string &playlistName);
};

#endif

