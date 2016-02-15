#include "SoundBuffer.hpp"
#include "../System/SoundSystem.hpp"
#include "SoundFile.hpp"

using namespace Audio;

SoundBuffer::SoundBuffer(SoundFile* soundFile) {
    // Create audio buffer.
    alGetError();
    alGenBuffers((ALuint)1, &mBuffer);
    System::SoundSystem::CheckError("Couldn't create buffers.");
    
    // Set the buffer data.
    alBufferData(mBuffer, soundFile->Format(), soundFile->Data(), soundFile->Size(), soundFile->SampleRate());
    System::SoundSystem::CheckError("Couldn't set buffer data.");
}

SoundBuffer::~SoundBuffer() {
    alDeleteBuffers(1, &mBuffer);
}

ALuint SoundBuffer::Buffer() const {
    return mBuffer;
}
