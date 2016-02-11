#include "SoundSource.hpp"
#include "../Entity/Entity.hpp"

using namespace Component;

SoundSource::SoundSource(Entity* entity) : SuperComponent(entity) {
    soundBuffer = nullptr;
    
    alGenSources(1, &mSource);
    
    pitch = 1.f;
    gain = 1.f;
    loop = AL_FALSE;
    
    mShouldPlay = false;
    mShouldPause = false;
    mShouldStop = false;
}

SoundSource::~SoundSource() {
    alDeleteSources(1, &mSource);
}

void SoundSource::Play() {
    mShouldPlay = true;
}

void SoundSource::Pause() {
    mShouldPause = true;
}

void SoundSource::Stop() {
    mShouldStop = true;
}
