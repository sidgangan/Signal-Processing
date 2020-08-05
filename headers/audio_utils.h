#include "constants.h"

// WAVE PCM soundfile format (you can find more in https://ccrma.stanford.edu/courses/422/projects/WaveFormat/ )
typedef struct header_file
{
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;			// sample_rate denotes the sampling rate.
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    int subchunk2_size;			// subchunk2_size denotes the total size in bytes of samples.
} header;

typedef struct header_file* header_p;
/************* UTILS ************/


bool get_chunk(short int audio[], int audio_len, double audio_chunk[],int* itr, int* chunk_size){
    if(*itr<audio_len){

        int i=0;
        while(i<n_fft && (*itr + i) < audio_len){
            audio_chunk[i] = (double)(audio[i+ *itr])/32768.0;
            i++;
        }
        *chunk_size = i;
        *itr = *itr + i;
        return true;
    }
    return false;
}