#include "audioobject.hpp"

// constructor
AudioObject::AudioObject(const SampleInfo& info, IAudioData* data) :
    m_audioPos(0),
    m_audioLength(data->GetAudioLength()),
    m_sampleInfo(info),
    m_audioData(data) {}

bool AudioObject::GenerateSamples(float* stream, size_t streamLength) {
    m_audioPos = m_audioData->GenerateSamples(
                                stream, 
                                streamLength, 
                                m_audioPos, 
                                m_sampleInfo
                              ); // wrapper for lower level IAudioData::GenerateSamples
    // get returned bool val
    if (m_audioPos == (size_t)-1) { //  reached EOF
        m_audioPos = 0; // reset to valid position
        return false;
    }
    return true;
}

void AudioObject::SetPos(double pos) {
    if (pos < 0.0) pos = 0.0;
    else if (pos > 1.0) pos = 1.0;
    m_audioPos = PosToAbsPos(pos);
}

size_t AudioObject::PosToAbsPos(double pos) {
    return (size_t)(pos * m_audioLength);
}