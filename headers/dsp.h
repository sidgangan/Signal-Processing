#include<complex.h>
using namespace std;

typedef complex<double> cd;

template<class T>
void hamming(T* a, int size);

void window_sumsquare(double window_sum[], int n, int n_frames, double win[], int n_fft, int hop);

void RFFT(double* audio, int audio_size, cd* spectrum);
void IRFFT(cd* spectrum, int spect_size, double* audio);
