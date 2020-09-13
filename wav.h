#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>
#include <ctype.h>

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

