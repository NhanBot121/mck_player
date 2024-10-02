#ifndef BROWSER_HPP
#define BROWSER_HPP

#include <vector>
#include <filesystem>

class Browser {
public:
    std::vector<std::filesystem::path> inDirMedia;
    std::string cwd; 
    // show all files in that dir and its sub-dir
    // pagination = 25
    Browser() : cwd{std::filesystem::current_path().string()} {
        readMediaToVector(cwd);
    }
    void listDirectory();
    void readMediaToVector(std::filesystem::path);
};

#endif