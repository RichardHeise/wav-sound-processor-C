// Author: Richard Fernando Heise Ferreira
// GRR20191053

#include "wav.h"
#include <unistd.h>

static void manageArgs(int argc, char **argv, FILE ** input, FILE ** output, float *lvl) {

    int option;
    while ((option = getopt (argc, argv, "i:o:l:")) != -1) {
        float parsed_arg;
        switch (option) {

			case 'i':
                // Tries to open optarg in read mode
	            (*input) = fopen(optarg, "r");

                // If file couldn't be open there's an error
	            if ((*input) == NULL) {
	                fprintf (stderr, "Couldn't open file %s\n", optarg);
	                exit (EXIT_FILE);
	            }
	            break;

			case 'o':
				// Creates a file in append mode 
			    (*output) = fopen(optarg, "a");
			    break;

			case 'l':
				// Parse the arg after "-l" to float
		  	    parsed_arg = atof(optarg);
		  	    if (parsed_arg <= 10.0 && parsed_arg >= 0.0) {
		            (*lvl) = parsed_arg;
		            break;
		        }

		  	    fprintf (stderr, "Level flag must have a value bigger than 0.0 and lesser than 10.0\n");
		  	    exit (EXIT_FLAG);

			default:
			    fprintf (stderr, "Usage: %s -l num -i input -o output\n", argv[0]);
			    exit (EXIT_USAGE);
		}
    }
}

int main(int argc, char **argv) {

    FILE *file_input = stdin;
    FILE *file_output = stdout;
    float level = 1.0; // Default
    wav_t wav;

    manageArgs (argc, argv, &file_input, &file_output, &level);
    readAudioData (&wav, file_input);
    changeVol (&wav, level);
    writeAudioData (&wav, file_output);


    return 1;
}
