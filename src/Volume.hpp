#ifndef VOLUME_HPP
#define VOLUME_HPP

#include <alsa/asoundlib.h>

class Volume {
private:
    snd_mixer_t* handle;                // Handle for the ALSA mixer
    snd_mixer_elem_t* elem;             // ALSA mixer element
    snd_mixer_selem_id_t* sid;          // Mixer simple element ID
    long volumeMin, volumeMax;          // Minimum and maximum volume range

    void initMixer();                   // Initializes ALSA mixer
    void closeMixer();                  // Closes ALSA mixer

public:
    Volume();                           // Constructor to initialize the mixer
    ~Volume();                          // Destructor to clean up

    void upVolume();                    // Increase volume by a defined step
    void downVolume();                  // Decrease volume by a defined step
    int getVolume() const;              // Get the current volume as a percentage
    void setVolume(int volume);         // Set the volume to a specific percentage
    void printVolume();
};

#endif
