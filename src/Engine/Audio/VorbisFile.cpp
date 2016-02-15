#include "VorbisFile.hpp"
#include "../Util/Log.hpp"
#include <stb_vorbis.c>

using namespace Audio;

VorbisFile::VorbisFile(const char *filename) {
    int channels;
    mDataSize = stb_vorbis_decode_filename(filename, &channels, &mSampleRate, reinterpret_cast<short**>(&mData));
    
    if (mDataSize == -1)
        Log() << "Couldn't load OGG Vorbis file: " << filename << "\n";
    
    // We get size in samples, but we need it in bytes.
    mDataSize *= channels * sizeof(short);
    
    if (channels > 1)
        mFormat = AL_FORMAT_STEREO16;
    else
        mFormat = AL_FORMAT_MONO16;
}

VorbisFile::~VorbisFile() {
    delete[] mData;
}

const char* VorbisFile::Data() const {
    return mData;
}

ALsizei VorbisFile::Size() const {
    return mDataSize;
}

ALenum VorbisFile::Format() const {
    return mFormat;
}

ALsizei VorbisFile::SampleRate() const {
    return mSampleRate;
}
