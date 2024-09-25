#ifndef AUDIO_OBJECT_INCLUDED_HPP
#define AUDIO_OBJECT_INCLUDED_HPP

#include "iaudiodata.hpp"
#include "sampleinfo.hpp"
class AudioObject {
    public:
        AudioObject(const SampleInfo& info, IAudioData* data); // constructor
        bool GenerateSamples(float* stream, size_t steamLength);
        void SetPos(double pos); // [0, 1] // independent of audio length
    private:
        size_t      m_audioPos;
        size_t      m_audioLength;

        SampleInfo  m_sampleInfo;
        IAudioData* m_audioData;

        size_t PosToAbsPos(double pos);
};

#endif