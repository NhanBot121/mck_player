#ifndef BROWSER_HPP
#define BROWSER_HPP

#include <vector>
#include <filesystem>

const std::string CW_DIR = std::filesystem::current_path();


class Browser {
public:
    std::vector<std::filesystem::path> inDirMedia;
    // show all files in that dir and its sub-dir
    // pagination = 25
    Browser() {
        readMediaToVector(CW_DIR);
    }
    void listDirectory();
    void readMediaToVector(std::filesystem::path);
};

#endif