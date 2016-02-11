#include "SoundSystem.hpp"

#include "../Audio/Listener.hpp"
#include "../Util/Log.hpp"
#include <AL/al.h>

#include "../Scene/Scene.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Physics.hpp"
#include "../Component/SoundSource.hpp"
#include "../Audio/SoundBuffer.hpp"

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

void SoundSystem::Update(Scene& scene) {
    std::vector<Component::SoundSource*> soundComponents = scene.GetAll<Component::SoundSource>();
    for (Component::SoundSource* sound : soundComponents) {
        Entity* entity = sound->entity;
        
        // Set position based on transform.
        Component::Transform* transform = entity->GetComponent<Component::Transform>();
        if (transform != nullptr) {
            glm::vec3 position = glm::vec3(transform->modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f));
            alSource3f(sound->mSource, AL_POSITION, position.x, position.y, position.z);
        }
        
        // Set velocity based on physics.
        Component::Physics* physics = entity->GetComponent<Component::Physics>();
        if (physics != nullptr)
            alSource3f(sound->mSource, AL_VELOCITY, physics->velocity.x, physics->velocity.y, physics->velocity.z);
        
        alSourcef(sound->mSource, AL_PITCH, sound->pitch);
        alSourcef(sound->mSource, AL_GAIN, sound->gain);
        alSource3f(sound->mSource, AL_VELOCITY, 0.f, 0.f, 0.f);
        alSourcei(sound->mSource, AL_LOOPING, sound->loop);
        if (sound->soundBuffer != nullptr)
            alSourcei(sound->mSource, AL_BUFFER, sound->soundBuffer->Buffer());
        
        // Play it / pause it / stop it.
        if (sound->mShouldPlay)
            alSourcePlay(sound->mSource);
        
        if (sound->mShouldPause)
            alSourcePause(sound->mSource);
        
        if (sound->mShouldStop)
            alSourceStop(sound->mSource);
    }
}
