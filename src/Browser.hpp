#ifndef BROWSER_HPP
#define BROWSER_HPP

#include <vector>
#include <filesystem>

class Browser {
    std::vector<std::filesystem::path> inDirMedia;
    std::filesystem::path cwd; 
public:
    // show all files in that dir and its sub-dir
    // pagination = 25
    Browser() {
        std::filesystem::path cwd = std::filesystem::current_path();
        readMediaToVector(cwd);
    }
    void listDirectory();
    void readMediaToVector(std::filesystem::path);
};

#endif