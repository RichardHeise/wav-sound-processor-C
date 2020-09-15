#include "wav.h"

void readAudioData(wav_t *wav_pointer,FILE *input) {

    fread(&wav_pointer->header, sizeof(wav_pointer->header), 1, input);
    wav_pointer->audio = malloc(sizeof(int16_t) * wav_pointer->header.data.SubChunk2Size/2);
    fread(wav_pointer->audio, 2, wav_pointer->header.data.SubChunk2Size, input);
}

void writeAudioData(wav_t *wav_pointer, FILE *output) {
    fwrite(&wav_pointer->header, sizeof(wav_pointer->header), 1, output);
    fwrite(wav_pointer->audio,sizeof(int16_t),wav_pointer->header.data.SubChunk2Size/2, output);
}

//==================================================================//

void changeVol(wav_t *wav_pointer, float lvl) {
    int i;
    long int test;
    for(i=0; i < wav_pointer->header.data.SubChunk2Size/2; i++) {
        test = wav_pointer->audio[i] * lvl;
        if (test > MAX_VOL) {
            test = MAX_VOL;
        } else if (test < -MAX_VOL) {
            test = -MAX_VOL;
        }
        wav_pointer->audio[i] = test;
    }
}

//====================================================================//

int16_t max(int16_t *array, long int size) {
    long int i;
    int16_t max;
    max = abs(array[0]);
    for (i=1; i < size; i++) {
        if(max < abs(array[i])) {
            max = array[i];
        }
    }
    return max;
}

//===================================================================//

void normalizer(wav_t *wav_pointer) {
    int16_t peak;
    int16_t diff;
    float level;
    peak = max(wav_pointer->audio, wav_pointer->header.data.SubChunk2Size/2);
    diff = (MAX_VOL-peak);
    level = (((diff*100)/MAX_VOL)/100.0) + 1;
    fprintf(stderr, "Normalizing with value %f\n", level);
    changeVol(wav_pointer, level);
}

//==================================================================//

void reverser(wav_t *wav_pointer, FILE *output) {
    int i;

    fwrite(&wav_pointer->header, sizeof(wav_pointer->header), 1, output);

    for (i=(wav_pointer->header.data.SubChunk2Size/2)-2; i > 0; i -= 2) {
        fwrite(&wav_pointer->audio[i], sizeof(int16_t), 1, output);
        ++i;
        fwrite(&wav_pointer->audio[i--], sizeof(int16_t), 1, output);
    }
}

//=================================================================//

void echo(wav_t *wav_pointer, int time, float lvl) {
    int i;
    int delay = wav_pointer->header.fmt.Nchannels * wav_pointer->header.fmt.Sample_rate * time/1000;
    fprintf(stderr, "delay: %d", delay);    
    for (i=delay+1; i < wav_pointer->header.data.SubChunk2Size/2; i++) {
        wav_pointer->audio[i] += (lvl * wav_pointer->audio[i - delay]);
    }
}
