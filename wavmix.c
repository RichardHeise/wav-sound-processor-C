// Author: Richard Fernando Heise Ferreira
// GRR20191053

#include "wav.h"

static void manageArgs (int argc, char **argv, FILE ** output, int indexs[], int *size) {
  
  int i;
  for (i = 1; i < argc; i++) {
    // Finds "-o" in argv and skips duplicated "-o"
    if (argv[i][0] == '-' && argv[i][1] == 'o' && (*output) == stdout) {
	    i++;

      // If next arg after "-o" doesn't exist there's an error
	    if (!argv[i]) {
	      fprintf (stderr, "Usage: %s arqs -o output\n", argv[0]);
	      exit (EXIT_USAGE);
	    }

      // Creates a argv[i] file with append mode
	    (*output) = fopen(argv[i], "a");
	  } else {
      // If it's an file name the index is stored in an index array
	    indexs[(*size)++] = i;
    }
  }
}

int main(int argc, char **argv) {

  FILE *file_output = stdout;
  wav_t *input_wavs;
  wav_t output_wav;
  int files_indexs[argc - 1];	// Declared with a temp size
  int arg_number = 0;		// Relative "true" size

  manageArgs (argc, argv, &file_output, files_indexs, &arg_number);

  input_wavs = malloc (sizeof (wav_t) * arg_number);
  if (! input_wavs) {
    fprintf(stderr, "Couldn't allocate memory\n");
    exit(EXIT_MEM);
  }

  readWavs (input_wavs, arg_number, argv, files_indexs);
  mixWavs (input_wavs, arg_number, &output_wav);
  writeAudioData (&output_wav, file_output);


  return 1;
}
