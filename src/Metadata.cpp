// extern "C" {
// #include <libavformat/avformat.h>
// #include <libavutil/log.h>
// }

#include "Metadata.hpp"
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <vector>

// Constructor to initialize the file path and load metadata if it's an audio file
Metadata::Metadata(const std::string& fileName) 
    : filePath(fileName), fileRef(fileName.c_str()), tag(nullptr), properties(nullptr) {
    if (isAudioFile()) {
        loadAudioFileMetadata();
    }
}

// Default constructor
Metadata::Metadata() : filePath(""), fileRef(), tag(nullptr), properties(nullptr) {}

// Destructor
Metadata::~Metadata() {}

// Load audio file metadata (Tag and AudioProperties)
void Metadata::loadAudioFileMetadata() {
    if (!fileRef.isNull()) {
        tag = fileRef.tag();
        properties = fileRef.audioProperties();
    }
}

// Helper function to check if the file is an audio file
bool Metadata::isAudioFile() const {
    static const std::vector<std::string> audioExtensions = {".mp3", ".wav", ".flac", ".ogg", ".m4a"};
    std::string extension = std::filesystem::path(filePath).extension().string();
    return std::find(audioExtensions.begin(), audioExtensions.end(), extension) != audioExtensions.end();
}

// Helper function to check if the file is a video file
bool Metadata::isVideoFile() const {
    static const std::vector<std::string> videoExtensions = {".mp4", ".mkv", ".avi", ".mov", ".flv"};
    std::string extension = std::filesystem::path(filePath).extension().string();
    return std::find(videoExtensions.begin(), videoExtensions.end(), extension) != videoExtensions.end();
}

// Display metadata of the file based on its type
void Metadata::viewMetadata() const {
    if (isAudioFile()) {
        viewAudioMetadata();
    } 
    // else if (isVideoFile()) {
    //     viewVideoMetadata();
    // } 
    else {
        std::cerr << "Unsupported file type or no metadata available for file: " << filePath << std::endl;
    }
}

// View metadata for audio files using TagLib
void Metadata::viewAudioMetadata() const {
    if (tag) {
        std::cout << "Audio Metadata:" << std::endl;
        std::cout << "Title:      " << tag->title()  << std::endl;
        std::cout << "Artist:     " << tag->artist() << std::endl;
        std::cout << "Album:      " << tag->album()  << std::endl;
        std::cout << "Year:       " << tag->year()   << std::endl;
        std::cout << "Track:      " << tag->track()  << std::endl;
        std::cout << "Genre:      " << tag->genre()  << std::endl;
    } else {
        std::cerr << "Failed to read audio metadata for file: " << filePath << std::endl;
    }
}

// View metadata for video files using FFmpeg
// void Metadata::viewVideoMetadata() const {
//     std::cout << "Video Metadata:" << std::endl;
//     std::cout << "File Name:  " << filePath << std::endl;

//     AVFormatContext* formatContext = nullptr;

//     // Open the video file
//     if (avformat_open_input(&formatContext, filePath.c_str(), nullptr, nullptr) != 0) {
//         std::cerr << "Could not open file: " << filePath << std::endl;
//         return;
//     }

//     // Retrieve stream information
//     if (avformat_find_stream_info(formatContext, nullptr) < 0) {
//         std::cerr << "Could not find stream information." << std::endl;
//         avformat_close_input(&formatContext);
//         return;
//     }

//     // Print general information
//     std::cout << "Duration: " << formatContext->duration / AV_TIME_BASE << " seconds" << std::endl;

//     // Loop through streams to get codec and bitrate information
//     for (unsigned int i = 0; i < formatContext->nb_streams; ++i) {
//         AVStream* stream = formatContext->streams[i];
//         AVCodecParameters* codecParams = stream->codecpar;

//         std::cout << "Stream #" << i << ": " << std::endl;
//         std::cout << " - Codec: " << avcodec_get_name(codecParams->codec_id) << std::endl;
//         std::cout << " - Bitrate: " << codecParams->bit_rate / 1000 << " kbps" << std::endl; // in kbps
//     }

//     // Clean up
//     avformat_close_input(&formatContext);
// }

// Edit metadata for audio files
void Metadata::editMetadata() {
    if (filePath.empty()) {
        std::cerr << "No file path specified." << std::endl;
        return;
    }

    if (!isAudioFile()) {
        std::cerr << "Editing is only supported for audio files." << std::endl;
        return;
    }

    if (!tag) {
        std::cerr << "No metadata available for editing." << std::endl;
        return;
    }

    do {
        std::string key;
        std::cout << "Enter key to edit (title, artist, album, year, track, genre): ";
        std::cin >> key;

        std::transform(key.begin(), key.end(), key.begin(), ::tolower);

        if (key == "title") {
            std::string value;
            std::cout << "Enter new value for title: ";
            std::cin.ignore();
            std::getline(std::cin, value);
            tag->setTitle(value);
        } 
        else if (key == "artist") {
            std::string value;
            std::cout << "Enter new value for artist: ";
            std::cin.ignore();
            std::getline(std::cin, value);
            tag->setArtist(value);
        } 
        else if (key == "album") {
            std::string value;
            std::cout << "Enter new value for album: ";
            std::cin.ignore();
            std::getline(std::cin, value);
            tag->setAlbum(value);
        } 
        else if (key == "year") {
            unsigned int year;
            std::cout << "Enter new value for year: ";
            std::cin >> year;
            tag->setYear(year);
        } 
        else if (key == "track") {
            unsigned int track;
            std::cout << "Enter new value for track: ";
            std::cin >> track;
            tag->setTrack(track);
        } 
        else if (key == "genre") {
            std::string value;
            std::cout << "Enter new value for genre: ";
            std::cin.ignore();
            std::getline(std::cin, value);
            tag->setGenre(value);
        } 
        else {
            std::cerr << "Unsupported key. Supported keys are: title, artist, album, year, track, genre." << std::endl;
        }

        fileRef.save();

        std::cout << "Do you want to edit another key? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'n' || choice == 'N') {
            break;
        }
    } while (true);

    std::cout << "Metadata update completed." << std::endl;
}

TagLib::String Metadata::get_title()
{
    return tag->title();
}

int Metadata::get_duration()
{
    return properties -> length();
}

TagLib::String Metadata::get_artist() {
    return tag -> artist();
}

TagLib::String Metadata::get_album() {
    return tag -> album();
}

TagLib::String Metadata::get_genre() {
    return tag -> genre();
}