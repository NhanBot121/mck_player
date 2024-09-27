#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>

class Metadata {
public:
    static void viewMetadata(const std::string& fileName);
    static void editMetadata(const std::string& filename);

private:
    static bool isAudioFile(const std::string& fileName);
    static bool isVideoFile(const std::string& fileName);
    static void viewAudioMetadata(const std::string& fileName);
    static void viewVideoMetadata(const std::string& fileName);
};

#endif // METADATA_HPP
