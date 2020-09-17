#include "wav.h"

static void manageArgs(int argc, char **argv, FILE **output, int indexs[],int *size) {
    int i;
    for (i = 1; i < argc; i++) {
      if (argv[i][0] == '-' && argv[i][1] == 'o') {
        if (!argv[i+1]) {
          fprintf(stderr, "Usage: %s arqs -o output\n", argv[0]);
          exit(-3);
        }
        (*output) = fopen(argv[i+1], "a");
        i++;
      } else {
        indexs[(*size)++] = i;
      }
    }
}
 
void readWavs(wav_t *wavs, int size, char **inputs, int indexs[]) {
    int i;
    FILE *input;
    
    for (i=0; i < size; i++) {
        input = fopen(inputs[indexs[i]], "r");
        if (!input) {
          fprintf(stderr, "Couldn't open file %s\n", inputs[indexs[i]]);
          exit(-2);
        }
        readAudioData(&wavs[i], input);
    }
} 

void copyAudio(int16_t *inputA, uint32_t size, int16_t *outputA, uint32_t *beg) {
    uint32_t i;

    for (i = 0; i < size; i++) {
        outputA[(*beg)+i] = inputA[i];
    }
    (*beg) += i;
}

void concatWavs(wav_t *wavs, int size, wav_t *wav_out) {
    int i;
    uint32_t currentPos = 0;
    
    wav_out->header = wavs[0].header;
    for (i = 1; i < size; i++) {
        wav_out->header.data.SubChunk2Size += wavs[i].header.data.SubChunk2Size;
        wav_out->header.RIFF.ChunkSize += wavs[i].header.RIFF.ChunkSize;
    }
    wav_out->audio = malloc(sizeof(int16_t) * wav_out->header.data.SubChunk2Size/2);

    for (i = 0; i < size; i++) {
        copyAudio(wavs[i].audio, wavs[i].header.data.SubChunk2Size/2, wav_out->audio, &currentPos);
        currentPos += 1;
    }
} 

int main (int argc, char **argv) {

    FILE *file_output = stdout;
    wav_t *input_wavs;
    wav_t output_wav;
    int files_indexs[argc-1]; // Declared with a temp size
    int arg_number = 0;      // Relative "true" size

    manageArgs(argc, argv, &file_output, files_indexs, &arg_number);

    input_wavs = malloc(sizeof(wav_t) * arg_number);

    readWavs(input_wavs, arg_number, argv, files_indexs);

    concatWavs(input_wavs, arg_number, &output_wav);

    writeAudioData(&output_wav, file_output);


    return 1; 
}