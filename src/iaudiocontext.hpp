#ifndef I_AUDIO_DEVICE_INCLUDED_HPP
#define I_AUDIO_DEVICE_INCLUDED_HPP

#include "audioobject.hpp"

class IAudioContext {
    public:
        virtual ~IAudioContext() {}
        virtual void PlayAudio(AudioObject& ao) = 0;
        virtual void PauseAudio(AudioObject& ao) = 0;
        virtual void StopAudio(AudioObject& ao) = 0;
    private:
};

#endif