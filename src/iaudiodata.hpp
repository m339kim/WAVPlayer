#ifndef I_AUDIO_DATA_INCLUDED_HPP
#define I_AUDIO_DATA_INCLUDED_HPP

#include "sampleinfo.hpp"
#include <cstddef>

/* interface */

class IAudioData {
    public: 
    // virutal functions are redefined(overriden) by a derived class
    // size_t: special unsigned int type 
        virtual ~IAudioData() {} // destructor called when overriden
        virtual size_t GenerateSamples(
            float* stream, 
            size_t streamLength,
            size_t pos, 
            const SampleInfo& info
        ) = 0;
        virtual size_t GetAudioLength() = 0;
    private:
};

#endif