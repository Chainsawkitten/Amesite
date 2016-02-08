#include "SoundSource.hpp"
#include <AL/alc.h>
#include "SoundSystem.hpp"
#include "SoundBuffer.hpp"

using namespace Audio;

Sound::Sound(SoundBuffer* buffer) {
    mPosition = glm::vec3(0.f, 0.f, 0.f);
    mPitch = 1.f;
    mGain = 1.f;
    mLoop = AL_FALSE;
    
    alGenSources((ALuint)1, &mSource);
    
    alSourcef(mSource, AL_PITCH, mPitch);
    alSourcef(mSource, AL_GAIN, mGain);
    alSource3f(mSource, AL_POSITION, mPosition.x, mPosition.y, mPosition.z);
    alSource3f(mSource, AL_VELOCITY, 0.f, 0.f, 0.f);
    alSourcei(mSource, AL_LOOPING, mLoop);
    
    alSourcei(mSource, AL_BUFFER, mBuffer->Buffer());
    SoundSystem::CheckError("Couldn't set sound source buffer.");
}

Sound::~Sound() {
    alDeleteSources(1, &mSource);
}

const glm::vec3& Sound::Position() const {
    return mPosition;
}

void Sound::SetPosition(const glm::vec3& position) {
    mPosition = position;
    alSource3f(mSource, AL_POSITION, mPosition.x, mPosition.y, mPosition.z);
}

ALfloat Sound::Pitch() const {
    return mPitch;
}

void Sound::SetPitch(ALfloat pitch) {
    mPitch = pitch;
    alSourcef(mSource, AL_PITCH, pitch);
}

ALfloat Sound::Gain() const {
    return mGain;
}

void Sound::SetGain(ALfloat gain) {
    mGain = gain;
    alSourcef(mSource, AL_GAIN, gain);
}

ALboolean Sound::Looping() const {
    return mLoop;
}

void Sound::SetLooping(ALboolean loop) {
    mLoop = loop;
    alSourcei(mSource, AL_LOOPING, loop);
}

void Sound::Play() {
    alSourcePlay(mSource);
    SoundSystem::CheckError("Couldn't play sound.");
}

void Sound::Pause() {
    alSourcePause(mSource);
    SoundSystem::CheckError("Couldn't pause sound.");
}

void Sound::Stop() {
    alSourceStop(mSource);
    SoundSystem::CheckError("Couldn't pause sound.");
}

ALint Sound::State() const {
    ALint state;
    alGetSourcei(mSource, AL_SOURCE_STATE, &state);
    return state;
}
