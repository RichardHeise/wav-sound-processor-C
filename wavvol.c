#include "wav.h"

void readAudioData(wav_t *wav_pointer,FILE *src) {

    fread(&wav_pointer->header, sizeof(wav_pointer->header), 1, src);
    wav_pointer->audio = malloc(sizeof(int16_t) * wav_pointer->header.data.SubChunk2Size/2);
    fread(wav_pointer->audio, 2, wav_pointer->header.data.SubChunk2Size, src);
}

void changeVol(wav_t *wav_pointer, float lvl) {
    int i;
    for(i=0; i < wav_pointer->header.data.SubChunk2Size/2; i++) {
        wav_pointer->audio[i] = wav_pointer->audio[i] * lvl;
        if (wav_pointer->audio[i] > 32767) {
            wav_pointer->audio[i] = 32767;
        } else if (wav_pointer->audio[i] < -32767) {
            wav_pointer->audio[i] = -32767;
        }
    }
}
/*
void writeAudioData(wav_t *wav_pointer) {
    fwrite(wav_pointer->header, sizeof(wav_pointer->header), 1, stdout);
    fwrite(wav_pointer->audio,2,wav_pointer->header.data.SubChunk2Size,stdout);
}
*/
int main (int argc, char **argv) {
    
    FILE *file_input = stdin;
    float level = 1.0;
    wav_t wav;
    int option;

    while ((option = getopt (argc, argv, "i:o:l:")) != -1) {
        float parsed_arg;
        switch (option) {
            case 'i':      
                file_input = fopen(optarg, "r");
                break;
            case 'o':
                break;
            case 'l':
                parsed_arg = atof(optarg);
                if (parsed_arg <= 10.0 && parsed_arg >= 0.0) {
                    level = parsed_arg;
                } else {
                    fprintf(stderr, "Level flag must have a value bigger than 0.0 and lesser than 10.0\n");
                    exit(-2);
                }
                break;
            default:
                fprintf (stderr, "Usage: %s -l num -i input -o output\n", argv[0]);
	            exit(-1);
        }
    }

    readAudioData(&wav, file_input);
    changeVol(&wav, level);
    // writeAudioData(&wav);

    //fwrite(&wav.header, 44, 1, stdout);
    //fwrite(&wav.audio,2,wav.header.data.SubChunk2Size,stdout);


    return 1;
}