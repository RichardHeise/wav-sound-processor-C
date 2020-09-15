#include "wav.h"

static void printHeader(wav_header *header_p) {
    int bytes_per_sample = (header_p->header.fmt.Bits_per_sample/8);
    long int samples_per_channel = (header_p->header.data.SubChunk2Size/bytes_per_sample)/header_p->header.fmt.Nchannels;
    printf("riff tag        (4 bytes): %.4s\n", header_p->header.RIFF.ChunkID); 
    printf("riff size       (4 bytes): %d\n", header_p->header.RIFF.ChunkSize);
    printf("wave tag        (4 bytes): %.4s\n", header_p->header.RIFF.Format);
    printf("form tag        (4 bytes): %.4s\n", header_p->header.fmt.SubChunk1ID);
    printf("fmt_size        (4 bytes): %d\n", header_p->header.fmt.SubChunk1Size);
    printf("audio_format    (2 bytes): %d\n", header_p->header.fmt.Audio_format);
    printf("num_channels    (2 bytes): %d\n", header_p->header.fmt.Nchannels);
    printf("sample_rate     (4 bytes): %d\n", header_p->header.fmt.Sample_rate);        
    printf("byte_rate       (4 bytes): %d\n", header_p->header.fmt.Byte_rate);        
    printf("block_align     (2 bytes): %d\n", header_p->header.fmt.Block_align);
    printf("bits_per_sample (2 bytes): %d\n", header_p->header.fmt.Bits_per_sample);
    printf("data tag        (4 bytes): %.4s\n", header_p->header.data.SubChunk2ID);
    printf("data size       (4 bytes): %d\n", header_p->header.data.SubChunk2Size);        
    printf("bytes per sample         : %d\n", bytes_per_sample);
    printf("samples per channel      : %ld\n", samples_per_channel);
}

static void manageArgs(int argc, char **argv, FILE **input) {
  
  int option;
  while ((option = getopt (argc, argv, "i:")) != -1) {
      switch (option) {
        case 'i':      
          (*input) = fopen(optarg, "r");
          break;
       default:
          fprintf (stderr, "Usage: %s -i arq1\n", argv[0]);
	        exit(-1);
      }
  }
}

int main (int argc, char **argv) {
    
    FILE *file_input = stdin;
    wav_header header;
    
    fread(&header, sizeof(wav_header), 1, file_input); 
    printHeader();
            
    return 1;
}