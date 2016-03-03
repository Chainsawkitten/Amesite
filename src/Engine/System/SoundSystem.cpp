#include "SoundSystem.hpp"

#include "../Util/Log.hpp"
#include <AL/al.h>

#include "../Scene/Scene.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Physics.hpp"
#include "../Component/SoundSource.hpp"
#include "../Component/Listener.hpp"
#include "../Audio/SoundBuffer.hpp"

using namespace Audio;
using namespace System;

SoundSystem* SoundSystem::mInstance = nullptr;

// Scaling constant. Used to convert from our units to sound system units.
const float soundScale = 0.2f;

SoundSystem::SoundSystem() {
    // Open default audio device.
    mDevice = alcOpenDevice(nullptr);
    if (!mDevice)
        Log() << "Couldn't open default audio device.\n";
    
    // Create audio context.
    mContext = alcCreateContext(mDevice, nullptr);
    if (!alcMakeContextCurrent(mContext))
        Log() << "Couldn't create audio context.\n";
    
    mInstance = this;
}

SoundSystem::~SoundSystem() {
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}

void SoundSystem::SetVolume(float volume) {
    alListenerf(AL_GAIN, volume);
    mVolume = volume;
}

float SoundSystem::GetVolume() const {
    return mVolume;
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
    // Update sound sources.
    std::vector<Component::SoundSource*> soundComponents = scene.GetAll<Component::SoundSource>();
    for (Component::SoundSource* sound : soundComponents) {
        if (sound->IsKilled())
            continue;
        
        Entity* entity = sound->entity;
        
        // Pause it.
        if (sound->mShouldPause) {
            alSourcePause(sound->mSource);
            sound->mShouldPause = false;
        }
        
        // Stop it.
        if (sound->mShouldStop) {
            alSourceStop(sound->mSource);
            sound->mShouldStop = false;
        }
        
        // Set position based on transform.
        Component::Transform* transform = entity->GetComponent<Component::Transform>();
        if (transform != nullptr) {
            glm::vec3 position = soundScale * glm::vec3(transform->modelMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f));
            alSource3f(sound->mSource, AL_POSITION, position.x, position.y, position.z);
        }
        
        // Set velocity based on physics.
        Component::Physics* physics = entity->GetComponent<Component::Physics>();
        if (physics != nullptr) {
            glm::vec3 velocity = soundScale * physics->velocity;
            alSource3f(sound->mSource, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        } else {
            alSource3f(sound->mSource, AL_VELOCITY, 0.f, 0.f, 0.f);
        }
        
        // Set other properties.
        alSourcef(sound->mSource, AL_PITCH, sound->pitch);
        alSourcef(sound->mSource, AL_GAIN, sound->gain);
        alSourcei(sound->mSource, AL_LOOPING, sound->loop);
        if (sound->soundBuffer != nullptr && !sound->mSoundBufferSet) {
            alSourcei(sound->mSource, AL_BUFFER, sound->soundBuffer->Buffer());
            sound->mSoundBufferSet = true;
        }
        
        // Play it.
        if (sound->mShouldPlay) {
            alSourcePlay(sound->mSource);
            sound->mShouldPlay = false;
        }
        
        CheckError("Something went wrong updating a sound source.");
    }
    
    // Update listener.
    std::vector<Component::Listener*> listeners = scene.GetAll<Component::Listener>();
    for (Component::Listener* listener : listeners) {
        Entity* entity = listener->entity;
        Component::Transform* transform = entity->GetComponent<Component::Transform>();
        if (transform != nullptr) {
            // Set position
            glm::vec3 position = soundScale * transform->position;
            alListener3f(AL_POSITION, position.x, position.y, position.z);
            CheckError("Couldn't set listener position.");
            
            // Set orientation.
            glm::vec4 forward = glm::inverse(transform->GetWorldOrientation()) * glm::vec4(0.f, 0.f, -1.f, 1.f);
            glm::vec4 up = glm::inverse(transform->GetWorldOrientation()) * glm::vec4(0.f, 1.f, 0.f, 1.f);
            ALfloat listenerOri[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
            alListenerfv(AL_ORIENTATION, listenerOri);
            CheckError("Couldn't set listener orientation.");
            
            break;
        }
    }
}
