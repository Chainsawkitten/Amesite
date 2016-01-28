#include "Listener.hpp"

#include <AL/al.h>
#include "SoundSystem.hpp"

using namespace Audio;

Listener::Listener() {
    mPosition = glm::vec3(0.f, 0.f, 0.f);
    mForward = glm::vec3(0.f, 0.f, -1.f);
    mUp = glm::vec3(0.f, 1.f, 0.f);
    
    mGain = 1.f;
}

const glm::vec3& Listener::GetPosition() const {
	return mPosition;
}

void Listener::SetPosition(const glm::vec3& position) {
    mPosition = position;
    alListener3f(AL_POSITION, position.x, position.y, position.z);
    SoundSystem::CheckError("Couldn't set listener position.");
}

const glm::vec3& Listener::GetForward() const {
    return mForward;
}

const glm::vec3& Listener::GetUp() const {
    return mUp;
}

void Listener::SetOrientation(const glm::vec3& forward, const glm::vec3& up) {
    mForward = forward;
    mUp = up;
    
    ALfloat listenerOri[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
    alListenerfv(AL_ORIENTATION, listenerOri);
    SoundSystem::CheckError("Couldn't set listener orientation.");
}

float Listener::GetGain() const {
    return mGain;
}

void Listener::SetGain(float gain) {
    mGain = gain;
    alListenerf(AL_GAIN, gain);
}
