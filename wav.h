#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>
#include <ctype.h>

#define MAX_VOL 35000

typedef struct s_riff {
    char ChunkID[4];
    uint32_t ChunkSize;
    char Format[4];
} riff_t;

typedef struct s_fmt {
    char SubChunk1ID[4];
    uint32_t SubChunk1Size;
    uint16_t Audio_format;
    uint16_t Nchannels;
    uint32_t Sample_rate;
    uint32_t Byte_rate;
    uint16_t Block_align;
    uint16_t Bits_per_sample; 
} fmt_t;

typedef struct s_data {
    char SubChunk2ID[4];
    uint32_t SubChunk2Size;
} data_t;

typedef struct s_header {
    riff_t RIFF;
    fmt_t fmt;
    data_t data;
} wav_header;

typedef struct s_wav {
    wav_header header;
    int16_t *audio;
} wav_t;

/**
 * Function to write audio data. 
 * Receives a s_wav struct by reference (&s_wav).
 * Receives a file pointer as the output file.
 */
void writeAudioData(wav_t *wav_pointer, FILE *output);


/**
 * Function to read audio data. 
 * Receives a s_wav struct by reference (&s_wav).
 * Receives a file pointer as the input file.
 */
void readAudioData(wav_t *wav_pointer,FILE *input);

/**
 * Function to change audio volume. 
 * Receives a s_wav struct by reference (&s_wav).
 * Receives a float volume level value.
 */
void changeVol(wav_t *wav_pointer, float lvl);

/**
 * Function to normalize audio volume. 
 * Receives a s_wav struct by reference (&s_wav).
 */
void normalizer(wav_t *wav_pointer);

/**
 * Function to find max value in an int16_t array.
 * Receives an array of int16_t.
 * Receives a long int size of an array.
 */
int16_t max(int16_t *array, long int size);

void reverser(wav_t *wav_pointer, FILE *output);

void echo(wav_t *wav_pointer, int delay, float lvl);

void wider(wav_t *wav_pointer, float k);