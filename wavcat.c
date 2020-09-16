#include "wav.h"

static void manageArgs(int argc, char **argv, FILE **output) {
  
  int option;
  while ((option = getopt (argc, argv, "o:")) != -1) {
      switch (option) {
        case 'o':      
          (*output) = fopen(optarg, "a");
          break;
       default:
          fprintf (stderr, "Usage: %s arqs -o output\n", argv[0]);
	        exit(-1);
      }
  }
}

void readWavs (wav_t *wavs, int size, FILE *input) {
    int i;
    for (i=0; i < size; i++) {
        readAudioData(&wavs[i], input);
    }
} 

int main (int argc, char **argv) {

    FILE *file_input = fopen(argv[1], "r");
    FILE *file_output = stdout;
    wav_t *input_wavs;
    wav_t output_wav;

    input_wavs = malloc(sizeof(wav_t) * (argc-1));

    readWavs(input_wavs, argc-1, file_input);

    fwrite(&input_wavs[0], sizeof(wav_header), 1, stdout);


    return 1;
}