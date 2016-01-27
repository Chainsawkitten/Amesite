#include "WaveFile.hpp"
#include <fstream>
#include <cstring>
#include "../Util/Log.hpp"

using namespace Audio;

WaveFile::WaveFile(const char* filename) {
	std::ifstream waveFile;
	waveFile.open(filename, std::ios::binary);
	if (!waveFile.is_open())
        Log() << "Couldn't open wave file " << filename << " for reading.\n";

	// Read RIFF.
	char id[4];
	waveFile.read(id, 4);
	if (!strcmp(id, "RIFF"))
		Log() << "File is not a RIFF file.\n";

	// Read file size (excluding RIFF).
	unsigned long size;
	waveFile.read(reinterpret_cast<char*>(&size), sizeof(size));

	// Read WAVE.
	waveFile.read(id, 4);
	if (!strcmp(id, "WAVE"))
		Log() << "File is not a wave file.\n";

	// Read format.
	unsigned long formatLength, avgBytesPerSec;
	short formatTag, channels, blockAlign, bitsPerSample;

	waveFile.read(id, 4);
	waveFile.read(reinterpret_cast<char*>(&formatLength), sizeof(formatLength));
	waveFile.read(reinterpret_cast<char*>(&formatTag), sizeof(formatTag));
	waveFile.read(reinterpret_cast<char*>(&channels), sizeof(channels));
	waveFile.read(reinterpret_cast<char*>(&mSampleRate), sizeof(mSampleRate));
	waveFile.read(reinterpret_cast<char*>(&avgBytesPerSec), sizeof(avgBytesPerSec));
	waveFile.read(reinterpret_cast<char*>(&blockAlign), sizeof(blockAlign));
	waveFile.read(reinterpret_cast<char*>(&bitsPerSample), sizeof(bitsPerSample));

	// Read data.
	waveFile.read(id, 4);
	waveFile.read(reinterpret_cast<char*>(&mDataSize), sizeof(mDataSize));
	mData = new char[mDataSize];
	waveFile.read(mData, mDataSize);

	waveFile.close();

	mFormat = ToALFormat(channels, bitsPerSample);
}

WaveFile::~WaveFile() {
	delete[] mData;
}

const char* WaveFile::Data() const {
	return mData;
}

ALsizei WaveFile::Size() const {
	return mDataSize;
}

ALenum WaveFile::Format() const {
	return mFormat;
}

ALsizei WaveFile::SampleRate() const {
	return mSampleRate;
}

ALenum WaveFile::ToALFormat(short channels, short bitsPerSample) {
	bool stereo = (channels > 1);

	switch (bitsPerSample) {
	case 16:
		if (stereo)
			return AL_FORMAT_STEREO16;
		else
			return AL_FORMAT_MONO16;
	case 8:
		if (stereo)
			return AL_FORMAT_STEREO8;
		else
			return AL_FORMAT_MONO8;
	default:
		return -1;
	}
}
