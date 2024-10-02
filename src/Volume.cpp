#include "Volume.hpp"
#include <iostream>

Volume::Volume() : currentVolume(64) {  // Default volume to 50% (64/128)
    Mix_Volume(-1, currentVolume);  // Set the initial volume for all channels
    Mix_VolumeMusic(currentVolume); // Set the initial volume for music
}

Volume::~Volume() {
    // No additional clean-up needed for volume control
}

void Volume::setVolume(int volume) {
    if (volume < 0 || volume > 128) {
        std::cerr << "Volume should be between 0 and 128." << std::endl;
        return;
    }
    currentVolume = volume;
    Mix_Volume(-1, currentVolume);  // Set volume for all channels
    Mix_VolumeMusic(currentVolume); // Set volume for music
}

int Volume::getVolume() const {
    // Get the actual volume from SDL for the first channel (or a specific channel)
    int volume = Mix_Volume(-1, -1);  // Use -1 to query the current volume of all channels
    return volume;
}

void Volume::upVolume() {
    int newVolume = getVolume() + 5;
    if (newVolume > 128) newVolume = 128;
    setVolume(newVolume);
}

void Volume::downVolume() {
    int newVolume = getVolume() - 5;
    if (newVolume < 0) newVolume = 0;
    setVolume(newVolume);
}

void Volume::printVolume() {
    int actualVolume = getVolume();  // Get the actual volume from SDL
    std::cout << "Volume = " << (actualVolume * 100 / 128) << "%" << std::endl;
}
