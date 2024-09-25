/*
 * Copyright (c) 2014, Benny Bobaganoosh
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED ANDON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <SDL2/SDL.h>

#define FILE_PATH "../res/audio/HelloFuture.wav" // audio file

struct AudioData {
	Uint8* pos; // curr pos of audio
	Uint32 length; // length of audio
};

// how does a device play audio? > has its own internal array of samples
// everytime it reaches its end, it calls MyAudioCallback

void MyAudioCallback(void* userdata, Uint8* stream, int streamLength) { // stream = audio device's internal audio buffer
	AudioData* audio = (AudioData*)userdata; // cast userdata into audiodata ptr
	
	if (audio->length == 0) return; // no audio to copy

	Uint32 length = (Uint32)streamLength; // how much we'll actually copy
	if (length > audio->length) length = audio->length;
	// now length is appropriate
	// do a memory copy
	SDL_memcpy(stream, audio->pos, length); // (destination, source, length)

	// update audio
	audio->pos += length;
	audio->length -= length;
}

int main(int argc, char** argv) {
	// Just to make sure SDL is working
	// SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_AUDIO); // optional, because SDL_INIT_EVERYTHING includes everything

	// load wav file
	SDL_AudioSpec wavSpec; // contains data of wav file // eg. frequency, audio channels (mono/stereo)
	Uint8* wavStart; // location in memory of the wav file
	Uint32 wavLength; 
	if (SDL_LoadWAV(FILE_PATH, &wavSpec, &wavStart, &wavLength) == NULL) {
		// SDL_LoadWAV: We are finding a sample array and storing it in wavStart, and
		//				creating a specification of an output device that can potnetially play the audio
		// can't load file
		std::cerr << "Error: " << FILE_PATH << " could not be loaded as an audio file" << std::endl;
		return 1; // to stop program execution
	}
	AudioData audio;
	audio.pos = wavStart;
	audio.length = wavLength;

	wavSpec.callback = MyAudioCallback; //SDL will know about function and use it to play audio
		// "when device runs out of audio samples, execute MyAudioCallback()"
	wavSpec.userdata = &audio;

	// find audio device that best matches specificaitons
	SDL_AudioDeviceID device = SDL_OpenAudioDevice(
		NULL,  
		0, 
		&wavSpec, // audio specifications
		NULL, 
		SDL_AUDIO_ALLOW_ANY_CHANGE // play even though not exact specification of wavSpec to match audio device
	); 
	if (!device) { // no audio device in system that we can access to play wav file
		std::cerr << "Error: " << SDL_GetError() << std::endl;
		return 1; // to stop program execution
	}

	SDL_PauseAudioDevice(device, 0); // 0:play, 1:pause

	// repeat until end of audio
	while(audio.length > 0) {
		SDL_Delay(100);
	}

	SDL_CloseAudioDevice(device); 
	SDL_FreeWAV(wavStart);
	SDL_Quit();
	return 0;
}
