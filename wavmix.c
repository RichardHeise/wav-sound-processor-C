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

void mixAudio(int16_t *inputA, uint32_t size, int16_t *outputA) {
    uint32_t i;
    int32_t test = 0;
    for (i = 0; i < size; i++) {
        test = outputA[i] + inputA[i];
        if (test > MAX_VOL) {
            test = MAX_VOL;
        } else if (test < -MAX_VOL) {
            test = -MAX_VOL;
        }

        outputA[i] = test;
    }

}

void mixWavs(wav_t *wavs, int size, wav_t *wav_out) {
    int i;
    int max = wavs[0].header.data.SubChunk2Size;

    wav_out->header = wavs[0].header;
    for (i = 1; i < size; i++) {
        if (wavs[i].header.data.SubChunk2Size > max) {
            max = wavs[i].header.data.SubChunk2Size;
            wav_out->header = wavs[i].header;
        }
    }
    wav_out->audio = malloc(sizeof(int16_t) * max/2);

    for (i = 0; i < size; i++) {
        mixAudio(wavs[i].audio, wavs[i].header.data.SubChunk2Size/2, wav_out->audio);
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

    mixWavs(input_wavs, arg_number, &output_wav);

    writeAudioData(&output_wav, file_output);


    return 1; 
}