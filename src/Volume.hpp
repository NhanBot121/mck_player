#ifndef VOLUME_HPP
#define VOLUME_HPP


#include <SDL2/SDL_mixer.h>
#include <iostream>

class Volume {
public:
    Volume();
    ~Volume();

    void setVolume(int volume);

    int getVolume() const;


    void upVolume();
    void downVolume();
    void printVolume();

private:
    int currentVolume;
};

#endif
