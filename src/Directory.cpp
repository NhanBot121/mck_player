#include "Directory.hpp"
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>

bool isMediaFile(const std::filesystem::path& path) {
    static const std::vector<std::string> mediaExtensions = {".mp3", ".mp4", ".wav", ".flac", ".ogg", ".m4a", "webm"};
    return std::find(mediaExtensions.begin(), mediaExtensions.end(), path.extension().string()) != mediaExtensions.end();
}

void Directory::listDirectory()
{
    std::string directoryName = "./";
    std::vector<std::filesystem::path> mediaFiles;

    try {
        // Collect all media files recursively
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryName)) {
            if (entry.is_regular_file() && isMediaFile(entry.path())) {
                // store the paths
                mediaFiles.push_back(entry.path());
            }
        }

        if (mediaFiles.empty()) {
            std::cout << "No media files found is the directory and its subdirectory." << std::endl;
            return;
        }

        // Pagination logic
        const size_t filesPerPage = 25;
        size_t totalFiles = mediaFiles.size();
        size_t totalPages = (totalFiles + filesPerPage - 1) / filesPerPage;
        size_t currentPage = 0;

        while (true) {
            // Display current page
            std::cout << "\nPage" << (currentPage + 1) << " of " << totalPages << std::endl;
            size_t start = currentPage * filesPerPage;
            size_t end = std::min(start + filesPerPage, totalFiles);

            for (size_t i = start; i < end; ++i) {
                std::cout << i + 1 << ". " << mediaFiles[i].string() << std::endl;
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
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}