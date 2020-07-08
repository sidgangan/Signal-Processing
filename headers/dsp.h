#include<complex.h>
using namespace std;

typedef complex<double> cd;

/*
Creates hamming window of given size
a => hamming window array
size => size of hamming window
*/
void hamming(double* a, int size);

/*
Creates sumsquare envelope for Inverse FFT
window_sum => sumsquare array
n => size of sumsquare array
n_frames => number of overlapping frames
win => hamming window
n_fft => size of FFT window
hop => sliding value
*/
void window_sumsquare(double window_sum[], int n, int n_frames, double win[], int n_fft, int hop);

/*
Real Fast Fourier Transform
Private Function called by stft
*/
void RFFT(cd* temp_spect, int audio_size, cd* spectrum);

/*
Inverse RFFT
spectrum => complex spectrum
spect_size => size of spectrum
audio => restored audio
*/
void IRFFT(cd* spectrum, int spect_size, double* audio);

/*
FFT after applying window to given audio
audio => input audio
spectrum => FFT of audio
hamming_window => window array
*/
void stft(double* audio, int audio_size, cd* spectrum, double* hamming_window);