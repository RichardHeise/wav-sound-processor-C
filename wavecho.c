// Author: Richard Fernando Heise Ferreira
// GRR20191053

#include "wav.h"

static void manageArgs (int argc, char **argv, FILE ** input, FILE ** output, float *lvl, int *t) {

    int option;
    while ((option = getopt (argc, argv, "i:o:l:t:")) != -1) {
        float parsed_arg;
        switch (option) {

			case 'i':
			    // Tries to open the file name after "-i" flag in read mode
			    (*input) = fopen(optarg, "r");

				// If it fails to open file there's an error
			    if ((*input) == NULL) {
			        fprintf (stderr, "Couldn't open file %s.\n", optarg);
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
		  	    if (parsed_arg <= 1.0 && parsed_arg >= 0.0) {
		            (*lvl) = parsed_arg;
		            break;
		        }

		  	    fprintf (stderr, "Level flag must have a value bigger than 0.0 and lesser than 1.0\n");
		  	    exit (EXIT_FLAG);

			case 't':
				// Parse the arg after "-t" to float
		  	    parsed_arg = atof(optarg);
		  	    if (parsed_arg >= 0.0) {
		            (*t) = parsed_arg;
		            break;
		        }

		  	    fprintf (stderr, "Time flag must have a value bigger than 0.0\n");
		  	    exit (EXIT_FLAG);
				
			default:
		  	    fprintf (stderr, "Usage: %s -t delay -l level -i input -o output\n", argv[0]);
		  	    exit (EXIT_USAGE);
		}
	}
}

int main (int argc, char **argv) {

    FILE *file_output = stdout;
    FILE *file_input = stdin;
    float level = 0.5; // Default values
    int time = 1000;   // Default values
    wav_t wav;

    manageArgs (argc, argv, &file_input, &file_output, &level, &time);
    readAudioData (&wav, file_input);
    echo (&wav, time, level);
    writeAudioData (&wav, file_output);

    return 1;
}
