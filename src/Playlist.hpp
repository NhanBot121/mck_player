#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP

#include <string>

class Playlist {
public:
    void listPlaylists();
    void createPlaylist(std::string pname);
    void updatePlaylist(std::string pname);
    void deletePlaylist(std::string pname);
};

#endif