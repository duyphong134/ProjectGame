#ifndef MUSIC_H

#include <SDL_mixer.h>

class Music{
private:
    Mix_Music* gMusic = nullptr;
public:
    void initMixer();

    //music
    Mix_Music *loadMusic(const char* path);
    void playMusic(Mix_Music *gMusic);

    // sound
    Mix_Chunk* loadSound(const char* path);
    void playSound(Mix_Chunk* gChunk);

    void changeMusicState();


};

#endif // MUSIC_H
