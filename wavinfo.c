#include "wav.h"

int main (int argc, char **argv) {
    
    FILE *file_input = stdin;
    wav_header header;
    int option;

    while ((option = getopt (argc, argv, "i:")) != -1) {
        switch (option) {
          case 'i':      
            file_input = fopen(optarg, "r");
            break;
         default:
            fprintf (stderr, "Usage: %s -i arq1\n", argv[0]);
	          exit(-1);
        }
    }
    
    fread(&header, sizeof(wav_header), 1, file_input); 

    int bytes_per_sample = (header.fmt.Bits_per_sample/8);
    long int samples_per_channel = (header.data.SubChunk2Size/bytes_per_sample)/header.fmt.Nchannels;
    printf("riff tag        (4 bytes): %.4s\n", header.RIFF.ChunkID); 
    printf("riff size       (4 bytes): %d\n", header.RIFF.ChunkSize);
    printf("wave tag        (4 bytes): %.4s\n", header.RIFF.Format);
    printf("form tag        (4 bytes): %.4s\n", header.fmt.SubChunk1ID);
    printf("fmt_size        (4 bytes): %d\n", header.fmt.SubChunk1Size);
    printf("audio_format    (2 bytes): %d\n", header.fmt.Audio_format);
    printf("num_channels    (2 bytes): %d\n", header.fmt.Nchannels);
    printf("sample_rate     (4 bytes): %d\n", header.fmt.Sample_rate);        
    printf("byte_rate       (4 bytes): %d\n", header.fmt.Byte_rate);        
    printf("block_align     (2 bytes): %d\n", header.fmt.Block_align);
    printf("bits_per_sample (2 bytes): %d\n", header.fmt.Bits_per_sample);
    printf("data tag        (4 bytes): %.4s\n", header.data.SubChunk2ID);
    printf("data size       (4 bytes): %d\n", header.data.SubChunk2Size);        
    printf("bytes per sample         : %d\n", bytes_per_sample);
    printf("samples per channel      : %ld\n", samples_per_channel);
            
    return 1;
}