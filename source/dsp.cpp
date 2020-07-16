
#include<complex>
#include "../headers/utils.h"
#include "../headers/dsp.h"
using namespace std;


void hamming(double* a, int size){
	double factor = 2*PI/(size);
	
	for(int i=0;i<size;i++){
		a[i] = 0.54 - 0.46*cos(factor*i);
	}
}

void window_sumsquare(double window_sum[], int n, int n_frames, double win[], int n_fft, int hop){
    // win => hamming window of size n_fft
    // window_sum => sumsquare array of size n_fft + hop*(n_frames - 1) , should be initiallized to zero
    // n => n_fft + hop*(n_frames - 1)
    
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

void RFFT(cd* temp_spect, int audio_size, cd* spectrum){

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

void stft(double* audio, int audio_size, cd* spectrum, double* hamming_window){

    cd temp_spect[audio_size];
    for(int i=0;i<audio_size;i++){
        temp_spect[i] = audio[i]*hamming_window[i];
    }

    RFFT(temp_spect, audio_size, spectrum);

}

void istft(cd spectrum[spectrum_size][out_num_segments],double audio[sumsquare_size],double win[], double win_sumsquare[]){
    cd temp_spect[out_num_segments][spectrum_size];
    double temp_audio[out_num_segments][audio_size];

    // transpose (129,4) -> (4,129)
    for(int i=0; i<out_num_segments; i++){
        for(int j=0; j<spectrum_size; j++){
            temp_spect[i][j] = spectrum[j][i];
        }
    }

    // calculate IRFFT of out_num_segment spectrums
    for(int i=0 ; i<out_num_segments ; i++){
        IRFFT(temp_spect[i],spectrum_size,temp_audio[i]);
    }

    // apply window
    for(int i=0; i<out_num_segments; i++){
        for(int j=0; j<audio_size; j++){
            temp_audio[i][j]*=win[j];
        }
    }

    // overlap add
    for(int i=0; i<out_num_segments; i++){
        int sample = i*hop;
        for(int j=0; j<audio_size; i++){
            audio[sample + j] += temp_audio[i][j];
        }
    }

    // apply window sumsquare envelop
    for(int i=0; i<sumsquare_size; i++){
        audio[i] /= win_sumsquare[i];
    }

}