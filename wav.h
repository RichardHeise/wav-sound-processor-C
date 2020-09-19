// Author: Richard Fernando Heise Ferreira
// GRR20191053

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>
#include <ctype.h>

// Max volume used to avoid clipping
#define MAX_VOL     29409

// Errors defines
#define EXIT_USAGE -1
#define EXIT_FLAG  -2
#define EXIT_FILE  -3
#define EXIT_MEM   -4
#define EXIT_READ  -5

/* Abstract types */

// RIFF struct
typedef struct s_riff
{
  char ChunkID[4];
  uint32_t ChunkSize;
  char Format[4];
} riff_t;

// fmt struct
typedef struct s_fmt
{
  char SubChunk1ID[4];
  uint32_t SubChunk1Size;
  uint16_t Audio_format;
  uint16_t Nchannels;
  uint32_t Sample_rate;
  uint32_t Byte_rate;
  uint16_t Block_align;
  uint16_t Bits_per_sample;
} fmt_t;

// data struct
typedef struct s_data
{
  char SubChunk2ID[4];
  uint32_t SubChunk2Size;
} data_t;

// header struct 
typedef struct s_header
{
  riff_t RIFF;
  fmt_t fmt;
  data_t data;
} wav_header;

// Wav struct
typedef struct s_wav
{
  wav_header header;
  int16_t *audio;
} wav_t;

/**
 * Function to write audio data. 
 * Receives a s_wav struct by reference (&s_wav).
 * Receives a file pointer as the output file.
 */
void writeAudioData (wav_t * wav_pointer, FILE * output);


/**
 * Function to read audio data. 
 * Receives a s_wav struct by reference (&s_wav).
 * Receives a file pointer as the input file.
 */
void readAudioData (wav_t * wav_pointer, FILE * input);

/**
 * Function to change audio volume. 
 * Receives a s_wav struct by reference (&s_wav).
 * Receives a float volume level value.
 */
void changeVol (wav_t * wav_pointer, float lvl);

/**
 * Function to normalize audio volume. 
 * Receives a s_wav struct by reference (&s_wav).
 */
void normalizer (wav_t * wav_pointer);

/**
 * Function to find max value in an int16_t array.
 * Receives an array of int16_t.
 * Receives a long int size of an array.
 * Returns the max value as an int16_t.
 */
int16_t max (int16_t * array, long int size);

/**
 * Function to reverse an audio file.
 * Receives a s_wav struct by reference (&s_wav).
 * Receives a FILE pointer to be used as the output.
 */
void reverser (wav_t * wav_pointer, FILE * output);

/**
 * Function to produce echo in an audio file.
 * Receives a s_wav struct by reference (&s_Wav).
 * Receives a int value in miliseconds.
 * Receives a float level of the echo value.
 */
void echo (wav_t * wav_pointer, int delay, float lvl);

/**
 * Function to wide an stereo audio.
 * Receives a s_wav struct by reference (&s_wav).
 * Receives a float value to use as wide value.
 */
void wider (wav_t * wav_pointer, float k);

/**
 * Function to read wavs from an array.
 * Receives a s_wav pointer. 
 * Receives a int size of the array of wavs.
 * Receives a char array pointer to be used as file names which 
   will be opened. This can be argv.
 * Receives an int index static array which has all the indexs
   of the file names. 
 */
void readWavs (wav_t * wavs, int size, char **inputs, int indexs[]);

/**
 * Function to mix wavs from an array;
 * Receives a s_wav pointer as the input wav array.
 * Receives a int size of the array of wavs.
 * Receives a s_wav pointer as the output s_wav struct.
 */
void mixWavs (wav_t * wavs, int size, wav_t * wav_out);

/**
 * Function to copy an audio array into another audio array.
 * Receives a int16_t pointer input array.
 * Receives a uinst32_t size of input array.
 * Receives a int16_t pointer output array.
 * Receives the beginning of the output array as a uint32_t pointer.
 */
void copyAudio (int16_t * inputA, uint32_t size, int16_t * outputA, uint32_t * beg);

/**
 * Function to concat wav files into a single file.
 * Receives a s_wav pointer as the input wav array.
 * Receives a int size of the s_wav array.
 * Receives a s_wav pointer as output wav.
 */
void concatWavs (wav_t * wavs, int size, wav_t * wav_out);
