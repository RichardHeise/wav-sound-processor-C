// Author: Richard Fernando Heise Ferreira
// GRR20191053

#include "wav.h"

void readAudioData(wav_t *wav_pointer, FILE *input) {

    if (fread (&wav_pointer->header, 1, sizeof(wav_header), input) < sizeof(wav_header)) {
        fprintf(stderr, "Couldn't read .wav header file.\n");
        exit(EXIT_READ);
    };

    wav_pointer->audio = malloc (sizeof (int16_t) * wav_pointer->header.data.SubChunk2Size / sizeof(int16_t));

    if (wav_pointer->audio == NULL) {
        fprintf(stderr, "Couldn't allocate memory.\n");
        exit(EXIT_MEM);
    }

    if (fread(wav_pointer->audio, sizeof(int16_t), wav_pointer->header.data.SubChunk2Size, input) 
        < sizeof(wav_pointer->header.data.SubChunk2Size))
    {
        fprintf(stderr, "Couldn't read .wav audio file.\n");
        exit(EXIT_READ);
    };

    fclose(input);
}

//------------------------------------------------------------------//

void writeAudioData(wav_t *wav_pointer, FILE *output) {

    // Writes header first for debugging reasons.
    // This way is simpler to detect where errors may happen. 
    fwrite(&wav_pointer->header, sizeof (wav_pointer->header), 1, output);
    fwrite(wav_pointer->audio, sizeof (int16_t), wav_pointer->header.data.SubChunk2Size / sizeof(int16_t), output);

    free(wav_pointer->audio);
}

//==================================================================//

void changeVol(wav_t *wav_pointer, float lvl) {
    int i;
    int test; // Tests clipping

    for (i = 0; i < wav_pointer->header.data.SubChunk2Size / sizeof(int16_t); i++) {

        // SOlves clipping errors.
        test = wav_pointer->audio[i] * lvl;
        if (test > MAX_VOL) {
	        test = MAX_VOL;
	    } else if (test < -MAX_VOL) {
	      test = -MAX_VOL;
	    }
        wav_pointer->audio[i] = test;
    }
}

//====================================================================//

int16_t max(int16_t *array, long int size) {
    long int i;
    int16_t max;

    max = abs(array[0]);
    for (i = 1; i < size; i++) {
        if (max < abs(array[i])) {
	        max = array[i];
	    }
    }
    return max;
}

//===================================================================//

void normalizer(wav_t *wav_pointer) {
    int16_t peak;
    int16_t diff;
    float level;

    peak = max (wav_pointer->audio, wav_pointer->header.data.SubChunk2Size / sizeof(int16_t));
    diff = (MAX_VOL - peak);
    level = (((diff * 100) / MAX_VOL) / 100.0) + 1;
    fprintf (stderr, "Normalizing with value %f\n", level);
    changeVol (wav_pointer, level);
}

//==================================================================//

void reverser(wav_t *wav_pointer, FILE *output) {
    int i;

    fwrite (&wav_pointer->header, sizeof (wav_pointer->header), 1, output); 

    /* THIS ONLY WORKS ON STEREO */
    // This logic is easier to understand if the reader goes through one loop
    // with simple arrays.
    // First we start off from the sample's array penultimate postion, which is
    // the left channel.

    // Then we write that position, we go to the last position now, which is
    // the right channel.

    // Then we write the last position, we put our iterable i back to the penultimate
    // position and we decrement 2 from i. That way we can reverse write the sample array
    // without messing the channels up. Keep in mind there are always an even number of samples
    // in an stereo array since it has two channels. 
    for (i = (wav_pointer->header.data.SubChunk2Size / sizeof(int16_t)) - 2; i >= 0; i -= 2) {

        fwrite (&wav_pointer->audio[i], sizeof (int16_t), 1, output);
        ++i;
        fwrite (&wav_pointer->audio[i--], sizeof (int16_t), 1, output);
    }

    free(wav_pointer->audio);
}

//=================================================================//

void echo(wav_t *wav_pointer, int time, float lvl) {
    int i;
    // Relative delay calculate by a simples proportion law
    int delay = wav_pointer->header.fmt.Nchannels * wav_pointer->header.fmt.Sample_rate *
                time / 1000;

    int test;   // testes clipping 
    for (i = delay + 1; i < wav_pointer->header.data.SubChunk2Size / sizeof(int16_t); i++) {

        test = wav_pointer->audio[i] + (lvl * wav_pointer->audio[i - delay]) ;
        if (test > MAX_VOL) {
	        test = MAX_VOL;
	    } else if (test < -MAX_VOL) {
	      test = -MAX_VOL;
	    }
        wav_pointer->audio[i] = test;
    }
}

//================================================================//

void wider (wav_t *wav_pointer, float k) {
    int i;
    int16_t diff; // diff used in stereo wide

    // Basicly it's the implementation of the wide formula
    // Sample array begins with left channel.
    for (i = 0; i < wav_pointer->header.data.SubChunk2Size / sizeof(int16_t); i++) {
        diff = wav_pointer->audio[i + 1] - wav_pointer->audio[i];
        wav_pointer->audio[i] -= k * diff;
        ++i;
        wav_pointer->audio[i] += k * diff;
    }
}

//===============================================================//

void mixAudio(int16_t *inputA, uint32_t size, int16_t *outputA) {
    uint32_t i;
    int test; // tests clipping

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

//==============================================================//

void mixWavs (wav_t *wavs, int size, wav_t *wav_out) {
    int i;
    int max = wavs[0].header.data.SubChunk2Size;    

    // First we find the biggest sample array possible to be allocated
    // We also need the biggest sample array header.
    wav_out->header = wavs[0].header;
    for (i = 1; i < size; i++) {
        if (wavs[i].header.data.SubChunk2Size > max) {
            max = wavs[i].header.data.SubChunk2Size;
          	wav_out->header = wavs[i].header;
        }
    }

    // Allocate max/bytes per samples
    wav_out->audio = malloc (sizeof (int16_t) * max / sizeof(int16_t));
    if (wav_out->audio == NULL) {
        fprintf(stderr, "Couldn't allocate memory.\n");
        exit(EXIT_MEM);
    }   
    for (i = 0; i < size; i++) {
        mixAudio (wavs[i].audio, wavs[i].header.data.SubChunk2Size / sizeof(int16_t),wav_out->audio);
        free(wavs[i].audio);
    }
}

//============================================================//

void concatWavs(wav_t *wavs, int size, wav_t *wav_out) {
    int i;
    uint32_t currentPos = 0;

    wav_out->header = wavs[0].header;

    // Copy the header, but we need to do some changes
    // Since we are adding .wavs we need to add the chunk sizes.
    for (i = 1; i < size; i++) {
        wav_out->header.data.SubChunk2Size += wavs[i].header.data.SubChunk2Size;
        wav_out->header.RIFF.ChunkSize += wavs[i].header.RIFF.ChunkSize;
    }

    // Allocate memory to all samples.
    wav_out->audio = malloc (sizeof (int16_t) * wav_out->header.data.SubChunk2Size / sizeof(int16_t));
    if (wav_out->audio == NULL) {
        fprintf(stderr, "Couldn't allocate memory.\n");
        exit(EXIT_MEM);
    }

    for (i = 0; i < size; i++) {
        // We need to tell our CopyAudio() where to begin the concat process on the output wav.
        copyAudio(wavs[i].audio, wavs[i].header.data.SubChunk2Size / sizeof(int16_t), wav_out->audio, &currentPos);
        free(wavs[i].audio);
        currentPos += 1;
    }
}

//===============================================================//

void copyAudio(int16_t *inputA, uint32_t size, int16_t *outputA, uint32_t *beg) {
    uint32_t i;

    // We simply copy one array into other starting the output array 
    // on beg position
    for (i = 0; i < size; i++) {
        outputA[(*beg) + i] = inputA[i];
    }
    (*beg) += i;
}

//=============================================================//

void readWavs(wav_t *wavs, int size, char **inputs, int indexs[]) {
    int i;
    FILE *input;

    // Go through every file and try to open and read it.
    for (i = 0; i < size; i++) {
        input = fopen (inputs[indexs[i]], "r");
        if (!input) {
            fprintf (stderr, "Couldn't open file %s\n", inputs[indexs[i]]);
            exit(EXIT_FILE);
    	}   
        readAudioData(&wavs[i], input);
    }
}
