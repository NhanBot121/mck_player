#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>

class Metadata {
public:
    // Constructors
    Metadata(const std::string& fileName);
    Metadata();
    
    // Destructor
    ~Metadata();

    // Public member functions
    void viewMetadata() const;
    void editMetadata();

    TagLib::String get_title();
    int get_duration();
    TagLib::String get_artist();
    TagLib::String get_album();
    TagLib::String get_genre();

private:
    std::string filePath;
    TagLib::FileRef fileRef;  // FileRef for managing the audio file
    TagLib::Tag* tag;         // For handling metadata like title, artist, etc.
    TagLib::AudioProperties* properties;  // For handling audio properties (bitrate, length, etc.)

    // Private helper functions
    bool isAudioFile() const;
    bool isVideoFile() const;
    void loadAudioFileMetadata();
    void viewAudioMetadata() const;
    //void viewVideoMetadata() const;
};

#endif // METADATA_HPP