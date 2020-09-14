#include "wav.h"

int main (int argc, char **argv) {
    
    FILE *file_input = stdin;
    FILE *file_output = stdout;
    float level = 1.0;
    wav_t wav;
    int option;

    while ((option = getopt (argc, argv, "i:o:l:")) != -1) {
        float parsed_arg;
        switch (option) {

            case 'i':      
                file_input = fopen(optarg, "r");
                if (file_input == NULL) {
                    fprintf(stderr, "Couldn't open file %s", optarg);
                    exit(-1);
                }
                break;

            case 'o':
                file_output = fopen(optarg, "a");
                break;

            case 'l':
                parsed_arg = atof(optarg);
                if (parsed_arg <= 10.0 && parsed_arg >= 0.0) {
                    level = parsed_arg;
                    break;
                }

                fprintf(stderr, "Level flag must have a value bigger than 0.0 and lesser than 10.0\n");
                exit(-2);

            default:
                fprintf (stderr, "Usage: %s -l num -i input -o output\n", argv[0]);
	            exit(-3);
        }
    }

    readAudioData(&wav, file_input);
    changeVol(&wav, level);
    writeAudioData(&wav, file_output);


    return 1;
}