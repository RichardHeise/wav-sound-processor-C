// Author: Richard Fernando Heise Ferreira
// GRR20191053

#include "wav.h"
#include <unistd.h>

static void printHeader (wav_header * header_p) {
    int bytes_per_sample = (header_p->fmt.Bits_per_sample / 8);
    int samples_per_channel =
    (header_p->data.SubChunk2Size / bytes_per_sample) /
    header_p->fmt.Nchannels;

    printf ("riff tag        (4 bytes): %.4s\n", header_p->RIFF.ChunkID);
    printf ("riff size       (4 bytes): %d\n", header_p->RIFF.ChunkSize);
    printf ("wave tag        (4 bytes): %.4s\n", header_p->RIFF.Format);
    printf ("form tag        (4 bytes): %.4s\n", header_p->fmt.SubChunk1ID);
    printf ("fmt_size        (4 bytes): %d\n", header_p->fmt.SubChunk1Size);
    printf ("audio_format    (2 bytes): %d\n", header_p->fmt.Audio_format);
    printf ("num_channels    (2 bytes): %d\n", header_p->fmt.Nchannels);
    printf ("sample_rate     (4 bytes): %d\n", header_p->fmt.Sample_rate);
    printf ("byte_rate       (4 bytes): %d\n", header_p->fmt.Byte_rate);
    printf ("block_align     (2 bytes): %d\n", header_p->fmt.Block_align);
    printf ("bits_per_sample (2 bytes): %d\n", header_p->fmt.Bits_per_sample);
    printf ("data tag        (4 bytes): %.4s\n", header_p->data.SubChunk2ID);
    printf ("data size       (4 bytes): %d\n", header_p->data.SubChunk2Size);
    printf ("bytes per sample         : %d\n", bytes_per_sample);
    printf ("samples per channel      : %d\n", samples_per_channel);
}

static void manageArgs (int argc, char **argv, FILE ** input) {

    int option;
    while ((option = getopt (argc, argv, "i:")) != -1) {

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

	    default:
	        fprintf (stderr, "Usage: %s -i arq1\n", argv[0]);
	        exit (EXIT_USAGE);
	    }
    }
}

int main(int argc, char **argv) {

    FILE *file_input = stdin;
    wav_header header;

    manageArgs (argc, argv, &file_input);
    if (fread (&header, 1, sizeof(wav_header), file_input) < sizeof(wav_header)) {
        fprintf(stderr, "Couldn't read .wav file.\n");
        exit(EXIT_READ);
    }
    printHeader(&header);
    fclose(file_input);

    return 1;
}
