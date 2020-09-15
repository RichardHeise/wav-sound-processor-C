#include "wav.h"


static void manageArgs(int argc, char **argv, FILE **input, FILE **output) {

    int option;
    while ((option = getopt (argc, argv, "i:o:")) != -1) {
        switch (option) {

            case 'i':      
                (*input) = fopen(optarg, "r");
                if ((*input) == NULL) {
                    fprintf(stderr, "Couldn't open file %s", optarg);
                    exit(-1);
                }
                break;

            case 'o':
                (*output) = fopen(optarg, "a");
                break;

            default:
                fprintf (stderr, "Usage: %s -i input -o output\n", argv[0]);
	            exit(-2);
        }
    }
}

int main (int argc, char **argv) {
    
    FILE *file_input = stdin;
    FILE *file_output = stdout;
    wav_t wav;

    manageArgs(argc, argv, &file_input, &file_output);
    readAudioData(&wav, file_input);
    normalizer(&wav);
    writeAudioData(&wav, file_output);

    return 1;    
}