//
// Created by nic on 7/2/26.
//


#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_log.h>

// Number of float samples per second
#define FLOAT_SAMPLES 8000
// Minimum amount of audio to keep in the sound buffer (~0.5seconds)
#define MIN_AUDIO ((FLOAT_SAMPLES * sizeof (float)) / 2)

SDL_AudioSpec audio_spec;
SDL_AudioStream *audio_stream;

bool sound_active = false;

int current_sine_sample = 0;

// SDL sound initialization
void initSound() {
    audio_spec.channels = 1;
    audio_spec.freq = 44100;
    audio_spec.format = SDL_AUDIO_F32;
    audio_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_spec, NULL, NULL);
    if (!audio_stream) {
        SDL_Log("Failed to open audio device stream: %s", SDL_GetError());
    }
}

// Pause or unpause the audio
void toggleSound(bool active) {
    // If we've toggled from off to on play audio
    if (active && !sound_active) {
        SDL_ResumeAudioStreamDevice(audio_stream);
    }
    // If we've toggled from on to off pause audio
    else if (!active && sound_active) {
       SDL_PauseAudioStreamDevice(audio_stream);
    }
    sound_active = active;
}

//Push new audio to the buffer
void updateSound() {
   if (SDL_GetAudioStreamQueued(audio_stream) < MIN_AUDIO) {
       static float samples[512];

       for (int i = 0; i < SDL_arraysize(samples); i++) {
           const int freq = 440;
           const float phase = (float) current_sine_sample * (float) freq / (float) FLOAT_SAMPLES;
           samples[i] = SDL_sinf(phase * 2 * SDL_PI_F);
           current_sine_sample++;
       }

       current_sine_sample %= FLOAT_SAMPLES;

       SDL_PutAudioStreamData(audio_stream, samples, sizeof(samples));
   }
}