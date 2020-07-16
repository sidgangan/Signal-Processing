#include<complex.h>

#if !defined(DENOISING_CONSTANTS)
#define DENOISING_CONSTANTS 1

const double PI = acos(-1);
const int audio_size = 256;
const int n_fft = 256;
const int hop = 64;
const int n_frames = 4;
const int sumsquare_size = n_fft + hop*(n_frames - 1);
const int spectrum_size = audio_size/2 +1;
const int num_segments = 8;
const int out_num_segments = 4;

#endif