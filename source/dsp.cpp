
#include<complex>
#include "../headers/utils.h"
using namespace std;


template<class T>
void hamming(T* a, int size){
	double factor = 2*PI/(size);
	
	for(int i=0;i<size;i++){
		a[i] = 0.54 - 0.46*cos(factor*i);
	}
}

void window_sumsquare(double window_sum[], int n, int n_frames, double win[], int n_fft, int hop){
    // win => hamming window of size n_fft
    // window_sum => sumsquare array of size n_fft + hop*(n_frames - 1) , should be initiallized to zero
    // size => n_fft + hop*(n_frames - 1)
    
    // square the win array elements
    square(win, n_fft);
    
    // fill sumsquare array
    for(int i=0; i<n_frames; i++){
        int sample = i*hop;
        int win_sum_start = sample;
        int win_sum_end = min(n, sample+n_fft);
        int win_start = 0;
        int win_end = max(0,min(n_fft, n - sample));

        int k = win_sum_start;
        int j = win_start;

        while(k<win_sum_end && j<win_end){
            window_sum[k] += win[j];
            k++;
            j++;
        }

    }
}

void RFFT(double* audio, int audio_size, cd* spectrum){
    cd temp_spect[audio_size];
    
    for(int i=0; i<audio_size;i++){
        temp_spect[i] = audio[i];
    }

    fft_util(temp_spect,audio_size,false);

    int spectrum_size = audio_size/2 + 1;

    copy_array(temp_spect, spectrum, spectrum_size);

    for(int i=1;i<spectrum_size;i++){
        spectrum[i] = conj(spectrum[i]);
    }

}


void IRFFT(cd* spectrum, int spect_size, double* audio){
    int audio_size = (spect_size - 1)*2;
    cd temp_audio[audio_size];
    temp_audio[0] = spectrum[0];

    for(int i=1; i< spect_size; i++){
        temp_audio[i] = conj(spectrum[i]);
    }

    for(int i= spect_size; i< audio_size; i++){
        temp_audio[i] = spectrum[audio_size - i];
    }

    fft_util(temp_audio,audio_size,true);

    for(int i=0; i<audio_size;i++){
        audio[i] = real(temp_audio[i]);
    }
}
