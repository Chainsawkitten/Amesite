#include "SoundSystem.hpp"

#include "../Audio/Listener.hpp"
#include "../Util/Log.hpp"
#include <AL/al.h>

using namespace Audio;
using namespace System;

SoundSystem* SoundSystem::mInstance = nullptr;

SoundSystem::SoundSystem() {
    // Open default audio device.
    mDevice = alcOpenDevice(nullptr);
    if (!mDevice)
        Log() << "Couldn't open default audio device.\n";
    
    // Create audio context.
    mContext = alcCreateContext(mDevice, nullptr);
    if (!alcMakeContextCurrent(mContext))
        Log() << "Couldn't create audio context.\n";
    
    mListener = new Listener();
    mInstance = this;
}

SoundSystem::~SoundSystem() {
    delete mListener;
    
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}

Listener* SoundSystem::GetListener() const {
    return mListener;
}

SoundSystem* SoundSystem::GetInstance() {
    return mInstance;
}

void SoundSystem::CheckError(const char* message) {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        Log() << message << "\n";
        if (error == AL_INVALID_NAME) Log() << "Invalid name\n";
        if (error == AL_INVALID_ENUM) Log() << "Invalid enum\n";
        if (error == AL_INVALID_VALUE) Log() << "Invalid value\n";
        if (error == AL_INVALID_OPERATION) Log() << "Invalid operation\n";
        if (error == AL_OUT_OF_MEMORY) Log() << "Out of memory like!\n";
    }
}
