#pragma once

#include "SoundFile.hpp"

namespace Audio {
    /// WAVE implementation of SoundFile.
    /**
     * Loads .wave-files.
     */
    class WaveFile : public SoundFile {
        public:
            /// Loads the specified wave-file.
            /**
             * @param filename Filename (relative or absolute) to wave-file.
             */
            WaveFile(const char* filename);
            
            /// Destructor
            ~WaveFile();
            
            /// Get raw audio data.
            /**
             * @return Raw audio data
             */
            const char* Data() const;
            
            /// Get data size.
            /**
             * @return The length of the raw audio data.
             */
            ALsizei Size() const;
            
            /// Get AL format.
            /**
             * 32-bit sound is not supported in OpenAL.
             * @return One of AL_FORMAT_MONO8, AL_FORMAT_MONO16, AL_FORMAT_STEREO8 or AL_FORMAT_STEREO16
             */
            ALenum Format() const;
            
            /// Get sample rate.
            /**
             * @return The sound file's sample rate (Hz)
             */
            ALsizei SampleRate() const;
            
        private:
            char* mData;
            unsigned long mDataSize;
            unsigned long mSampleRate;
            ALenum mFormat;
            
            static inline ALenum ToALFormat(short channels, short bitsPerSample);
    };
}
