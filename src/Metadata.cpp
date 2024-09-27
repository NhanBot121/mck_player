extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/log.h>
}

#include "Metadata.hpp"
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mp4file.h>
#include <taglib/mp4tag.h>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <vector>

// Helper function to check if the file is an audio file
bool Metadata::isAudioFile(const std::string& fileName) {
    static const std::vector<std::string> audioExtensions = {".mp3", ".wav", ".flac", ".ogg", ".m4a"};
    std::string extension = std::filesystem::path(fileName).extension().string();
    return std::find(audioExtensions.begin(), audioExtensions.end(), extension) != audioExtensions.end();
}

// Helper function to check if the file is a video file
bool Metadata::isVideoFile(const std::string& fileName) {
    static const std::vector<std::string> videoExtensions = {".mp4", ".mkv", ".avi", ".mov", ".flv"};
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

void Metadata::viewVideoMetadata(const std::string& fileName) {
    std::cout << "Video Metadata:" << std::endl;
    std::cout << "File Name:  " << fileName << std::endl;

    AVFormatContext* formatContext = nullptr;

    // Open the video file
    if (avformat_open_input(&formatContext, fileName.c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Could not open file: " << fileName << std::endl;
        return;
    }

    // Retrieve stream information
    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        std::cerr << "Could not find stream information." << std::endl;
        avformat_close_input(&formatContext);
        return;
    }

    // Print general information
    std::cout << "Duration: " << formatContext->duration / AV_TIME_BASE << " seconds" << std::endl;

    // Loop through streams to get codec and bitrate information
    for (unsigned int i = 0; i < formatContext->nb_streams; ++i) {
        AVStream* stream = formatContext->streams[i];
        AVCodecParameters* codecParams = stream->codecpar;

        std::cout << "Stream #" << i << ": " << std::endl;
        std::cout << " - Codec: " << avcodec_get_name(codecParams->codec_id) << std::endl;
        std::cout << " - Bitrate: " << codecParams->bit_rate / 1000 << " kbps" << std::endl; // in kbps
    }

    // Clean up
    avformat_close_input(&formatContext);
}

// Edit metadata for audio files
void Metadata::editMetadata(const std::string &fileName) {
    // Check if the file exists
    if (!std::filesystem::exists(fileName)) {
        std::cout << "Media file doesn't exist." << std::endl;
        return;
    }

    // Check if it is an audio file
    if (!isAudioFile(fileName)) {
        std::cout << "This function only supports editing metadata for audio files." << std::endl;
        return;
    }

    TagLib::FileRef file(fileName.c_str());
    if (file.isNull() || !file.tag()) {
        std::cerr << "Failed to load the file or the file doesn't contain metadata: " << fileName << std::endl;
        return;
    }

    TagLib::Tag *tag = file.tag();

    do {
        std::string key;
        std::cout << "Enter key to edit (title, artist, album, year, track, genre): ";
        std::cin >> key;

        // Convert key to lowercase for comparison
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);

        // Supported keys
        if (key == "title") {
            std::string value;
            std::cout << "Enter new value for " << key << ": ";
            std::cin.ignore(); // Ignore newline from previous input
            std::getline(std::cin, value);
            tag->setTitle(value);
            std::cout << "Title updated to: " << value << std::endl;
        } 
        else if (key == "artist") {
            std::string value;
            std::cout << "Enter new value for " << key << ": ";
            std::cin.ignore();
            std::getline(std::cin, value);
            tag->setArtist(value);
            std::cout << "Artist updated to: " << value << std::endl;
        } 
        else if (key == "album") {
            std::string value;
            std::cout << "Enter new value for " << key << ": ";
            std::cin.ignore();
            std::getline(std::cin, value);
            tag->setAlbum(value);
            std::cout << "Album updated to: " << value << std::endl;
        } 
        else if (key == "year") {
            unsigned int year;
            std::cout << "Enter new value for " << key << ": ";
            std::cin >> year;
            tag->setYear(year);
            std::cout << "Year updated to: " << year << std::endl;
        } 
        else if (key == "track") {
            unsigned int track;
            std::cout << "Enter new value for " << key << ": ";
            std::cin >> track;
            tag->setTrack(track);
            std::cout << "Track updated to: " << track << std::endl;
        } 
        else if (key == "genre") {
            std::string value;
            std::cout << "Enter new value for " << key << ": ";
            std::cin.ignore();
            std::getline(std::cin, value);
            tag->setGenre(value);
            std::cout << "Genre updated to: " << value << std::endl;
        } 
        else {
            std::cerr << "Unsupported key. Supported keys are: title, artist, album, year, track, genre." << std::endl;
        }

        // Save the modified metadata
        file.save();

        std::cout << "Do you want to edit another key? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'n' || choice == 'N') {
            break;
        }
    } while (true);

    std::cout << "Metadata update completed." << std::endl;
}
