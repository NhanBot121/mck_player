#include "Volume.hpp"
#include <iostream>

#define MIXER_NAME "Master"  // Name of the mixer control

// Constructor: Initialize the mixer
Volume::Volume() {
    initMixer();
}

// Destructor: Clean up resources
Volume::~Volume() {
    closeMixer();
}

// Initializes the ALSA mixer
void Volume::initMixer() {
    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, "default");
    snd_mixer_selem_register(handle, nullptr, nullptr);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_name(sid, MIXER_NAME);

    elem = snd_mixer_find_selem(handle, sid);
    snd_mixer_selem_get_playback_volume_range(elem, &volumeMin, &volumeMax);
}

// Closes the ALSA mixer
void Volume::closeMixer() {
    snd_mixer_close(handle);
}

// Gets the current volume level (0 - 100%)
int Volume::getVolume() const {
    long volumeValue;
    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volumeValue);

    // Convert the actual volume value to percentage
    int volumePercent = static_cast<int>((volumeValue * 100) / volumeMax);
    return volumePercent;
}

// Sets the volume to a specific level (0 - 100%)
void Volume::setVolume(int volume) {
    if (volume < 0 || volume > 100) {
        std::cerr << "Volume should be between 0 and 100." << std::endl;
        return;
    }

    long volumeValue = (volume * volumeMax) / 100;
    snd_mixer_selem_set_playback_volume_all(elem, volumeValue);
}

void Volume::printVolume()
{
    std::cout << "Volume = " << getVolume() << "%" << std::endl;
}

// Increase the volume by a step (e.g., 5%)
void Volume::upVolume() {
    int currentVolume = getVolume();
    int newVolume = currentVolume + 5;
    if (newVolume > 100) newVolume = 100;  // Cap at 100%
    setVolume(newVolume);
}

// Decrease the volume by a step (e.g., 5%)
void Volume::downVolume() {
    int currentVolume = getVolume();
    int newVolume = currentVolume - 5;
    if (newVolume < 0) newVolume = 0;  // Cap at 0%
    setVolume(newVolume);
}
