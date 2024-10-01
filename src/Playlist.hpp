#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <string>
#include <vector>
#include <filesystem>

class Playlist {
    const std::filesystem::path PLAYLISTS_PATH = "/home/nhnbot21/ndrd/c_cpp/cdr/mck_player/src/playlists/";
    const std::filesystem::path playlistPath;
    std::vector<std::filesystem::path> playlist; // vector of playlist files, each playlist contain the path to the media file.
public:

    Playlist() {
        readPlaylistLists(PLAYLISTS_PATH);
    }

    void readPlaylistLists(const std::filesystem::path& playlistDir);

    void addToPlaylist(const std::filesystem::path &playlistPath, const std::filesystem::path &mediaPath);

    void listPlaylists();
    void viewPlaylist(const std::string& playlistName);
    void createPlaylist(const std::string& playlistName);
    void updatePlaylist(const std::string& playlistName);
    void deletePlaylist(const std::string& playlistName);
};

#endif