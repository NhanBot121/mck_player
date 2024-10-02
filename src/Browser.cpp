#include "Browser.hpp"
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>

bool isMediaFile(const std::filesystem::path& path) {
    static const std::vector<std::string> mediaExtensions = {".mp3", ".mp4", ".wav", ".flac", ".ogg", ".m4a", "webm"};
    return std::find(mediaExtensions.begin(), mediaExtensions.end(), path.extension().string()) != mediaExtensions.end();
}

void Browser::listDirectory()
{
    std::cout << "Current woring directory: " << std::filesystem::current_path().string() << std::endl;

    if (inDirMedia.empty()) {
        std::cout << "No media files found is this directory and its sub-directory." << std::endl;
        return;
    }

    // Pagination logic
    const size_t filesPerPage = 25;
    size_t totalFiles = inDirMedia.size();
    size_t totalPages = (totalFiles + filesPerPage - 1) / filesPerPage;
    size_t currentPage = 0;

    while (true) {
        // Display current page
        std::cout << "\nPage" << (currentPage + 1) << " of " << totalPages << std::endl;
        size_t start = currentPage * filesPerPage;
        size_t end = std::min(start + filesPerPage, totalFiles);

        for (size_t i = start; i < end; ++i) {
            std::cout << i + 1 << ". " << inDirMedia[i].stem().string()  << std::endl;
        }

        // Pagination navigation
        std::cout << "\nNavigation options: [n]ext, [p]revious, [q]uit\n";
        std::cout << "Enter your choice: ";
        char choice;
        std::cin >> choice;

        if (choice == 'n' && currentPage + 1 < totalPages) {
            currentPage++;
        } else if (choice == 'p' && currentPage > 0) {
            currentPage--;
        } else if (choice == 'q') {
            std::cout << "Leaving media files explorer..." << std::endl;
            break;
        } else {
            std::cout << "Invalid choice! Just enter 'n', 'p', 'q'." << std::endl;
        }
    }
}

void Browser::readMediaToVector(std::filesystem::path cwd)
{
    try {
    // Collect all media files recursively
        for (const auto& entry : std::filesystem::recursive_directory_iterator(cwd)) {
            if (entry.is_regular_file() && isMediaFile(entry.path())) {
                // store the paths
                inDirMedia.push_back(entry.path());
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
