// Author: Richard Fernando Heise Ferreira
// GRR20191053

#include "wav.h"
#include <unistd.h>

static void manageArgs (int argc, char **argv, FILE ** input, FILE ** output) {

    int option;
    while ((option = getopt (argc, argv, "i:o:")) != -1) {
        switch (option) {

		    case 'i':
                // Tries to open optarg in read mode
	            (*input) = fopen(optarg, "r");

                // If file couldn't be open there's an error
	            if ((*input) == NULL) {
	                fprintf (stderr, "Couldn't open file %s.\n", optarg);
	                exit (EXIT_FILE);
	            }
	            break;

		    case 'o':
				// Creates a file in append mode 
		  	    (*output) = fopen(optarg, "a");
		  	    break;

		    default:
		        fprintf (stderr, "Usage: %s -i input -o output\n", argv[0]);
		        exit (EXIT_USAGE);
		}
    }
}

int main(int argc, char **argv) {

    FILE *file_input = stdin;
    FILE *file_output = stdout;
    wav_t wav;

    manageArgs (argc, argv, &file_input, &file_output);
    readAudioData (&wav, file_input);
    normalizer (&wav);
    writeAudioData (&wav, file_output);

    return 1;
}
