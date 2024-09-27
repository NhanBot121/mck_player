#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <string>
#include <vector>

class Playlist {
    std::vector<std::string> all_playlists;
public:
    static void listPlaylists();
    static void viewPlaylist(const std::string& playlistName);
    static void createPlaylist(const std::string& playlistName);
    static void updatePlaylist(const std::string& playlistName);
    static void deletePlaylist(const std::string& playlistName);
};

#endif