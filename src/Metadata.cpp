#include "Metadata.hpp"
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mp4file.h>
#include <taglib/mp4tag.h>
#include <taglib/tpropertymap.h>
#include <iostream>
#include <filesystem>

// Helper function to check if the file is an audio file
bool Metadata::isAudioFile(const std::string& fileName) {
    static const std::vector<std::string> audioExtensions = {".mp3", ".wav", ".flac", ".ogg", ".m4a"};
    std::string extension = std::filesystem::path(fileName).extension().string();
    return std::find(audioExtensions.begin(), audioExtensions.end(), extension) != audioExtensions.end();
}

// Helper function to check if the file is a video file
bool Metadata::isVideoFile(const std::string& fileName) {
    static const std::vector<std::string> audioExtensions = {".mp4", ".wav", ".flac", ".ogg", ".m4a"};
    std::string extension = std::filesystem::path(fileName).extension().string();
    return std::find(videoExtensions.begin(), videoExtensions.end(), extension) != videoExtensions.end();
}

// Display metadata of the given file
void Metadata::viewMetadata(const std::string& fileName) {
    if (isAudioFile(fileName)) {
        viewAudioMetadata(fileName);
    } else if (isVideoFile(fileName)) {
        viewVideoMetadata(fileName);
    } else {
        std::cerr << "Unsupported file type or no metadata available for file: " << fileName << std::endl;
    }
}

// View metadata for audio files using TagLib
void Metadata::viewAudioMetadata(const std::string& fileName) {
    TagLib::FileRef file(fileName.c_str());
    if (!file.isNull() && file.tag()) {
        TagLib::Tag *tag = file.tag();
        std::cout << "Audio Metadata:" << std::endl;
        std::cout << "Title:      " << tag->title()  << std::endl;
        std::cout << "Artist:     " << tag->artist() << std::endl;
        std::cout << "Album:      " << tag->album()  << std::endl;
        std::cout << "Year:       " << tag->year()   << std::endl;
        std::cout << "Track:      " << tag->track()  << std::endl;
        std::cout << "Genre:      " << tag->genre()  << std::endl;
        std::cout << "Publisher:  N/A" << std::endl; // TagLib may not support all metadata fields
    } else {
        std::cerr << "Failed to read audio metadata for file: " << fileName << std::endl;
    }
}

// View metadata for video files using FFmpeg or similar library
void Metadata::viewVideoMetadata(const std::string& fileName) {
    //
    std::cout << "Video Metadata:" << std::endl;
    std::cout << "File Name:  " << fileName << std::endl;
    std::cout << "Size: " << std::endl; // Replace with actual size
    std::cout << "Duration:  " << std::endl; // Replace with actual duration
    std::cout << "Bitrate:  " << std::endl; // Replace with actual bitrate
    std::cout << "Codec:  " << std::endl; // Replace with actual codec
}

// Placeholder functions for change and add metadata
void Metadata::editMetadata(const std::string &fileName)
{
    // check if media file exist
    if (!std::filesystem::exists(fileName)) {
        std::cout << "Media file doesn't exist." << std::endl;
    } else {
        do {
            std::string key;
            std::cout << "Enter key to edit: ";
            std::cin >> key;

            // check if key valid

            std::string value;
            std::cout << "Enter new value: ";
            std::cin >> value;

            // edit the metadata

            // ask the user if they want to continue editting

        } while () // the user want to continue editting {
            // continue editting
        }

    
            
    }
}


