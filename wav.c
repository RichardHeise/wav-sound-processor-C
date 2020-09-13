#include "wav.h"

/**
 * Function to write audio data. 
 * Receives a s_wav struct by reference (&s_wav).
 * Receives a file pointer as the output file.
 */
void writeAudioData(wav_t *wav_pointer, FILE *output) {
    fwrite(&wav_pointer->header, sizeof(wav_pointer->header), 1, output);
    fwrite(wav_pointer->audio,sizeof(int16_t),wav_pointer->header.data.SubChunk2Size/2, output);
}

/**
 * Function to read audio data. 
 * Receives a s_wav struct by reference (&s_wav).
 * Receives a file pointer as the input file.
 */
void readAudioData(wav_t *wav_pointer,FILE *input) {

    fread(&wav_pointer->header, sizeof(wav_pointer->header), 1, input);
    wav_pointer->audio = malloc(sizeof(int16_t) * wav_pointer->header.data.SubChunk2Size/2);
    fread(wav_pointer->audio, 2, wav_pointer->header.data.SubChunk2Size, input);
}

/**
 * Function to change audio volume. 
 * Receives a s_wav struct by reference (&s_wav).
 * Receives a float volume level value.
 */
void changeVol(wav_t *wav_pointer, float lvl) {
    int i;
    for(i=0; i < wav_pointer->header.data.SubChunk2Size/2; i++) {
        wav_pointer->audio[i] = wav_pointer->audio[i] * lvl;
        if (wav_pointer->audio[i] > 32767) {
            wav_pointer->audio[i] = 32767;
        } else if (wav_pointer->audio[i] < -32767) {
            wav_pointer->audio[i] = -32767;
        }
    }
}