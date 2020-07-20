#include<complex.h>
#include "constants.h"
using namespace std;

typedef complex<double> cd;

// Squaring a collection / array
template<class T>
void square(T* a, int size){
    for(int i=0; i<size; i++){
        a[i] *= a[i];
    }
}

// Copy one array to another
template<class T>
void copy_array(T* src, T* dest, int size){
    for(int i=0; i<size; i++){
        dest[i] = src[i];
    }
}

/*
Calculates mean of elements in given 2D array
Usage: mean((double *)arr, m, n) 
where arr is m x n 2D array
*/
double mean(double arr[spectrum_size][num_segments], int m, int n);

/*
Calculates mean of squares of elements in given 2D array
Usage: mean_square((double *)arr, m, n) 
where arr is m x n 2D array
*/
double mean_square(double arr[spectrum_size][num_segments], int m, int n);

/*
Calculates std deviation of elements in given 2D array
Usage: std_deviation((double *)arr, m, n, mean) 
where arr is m x n 2D array
*/
double std_deviation(double arr[spectrum_size][num_segments], int m,int n, double mean);

/*
Normalize values around given mean and std deviation
Usage: normalize((double *)arr, m, n, mean, std_dev) 
where arr is m x n 2D array
*/
void normalize(double arr[spectrum_size][num_segments], double norm[spectrum_size][num_segments], int m, int n, double mean, double std_dev);

/*
Calculates magnitude of each complex number in spectrum array
spectrum => 1D array of complex numbers
mag => output array storing the magnitudes
size => length of arrays
*/
void magnitude(cd* spectrum, double* mag, int size);

/*
Calculates angle of each complex number in spectrum array
spectrum => 1D array of complex numbers
mag => output array storing the magnitudes
size => length of arrays
*/
void angle(cd* spectrum, double* ang, int size);

/*
Converts complex number from polar coordinates
mag => magnitude array
phase => angle array
spect => output array of complex numbers
size => size of arrays
*/
void complex_from_polar(double mag[spectrum_size][out_num_segments], double phase[spectrum_size][out_num_segments], cd spect[spectrum_size][out_num_segments], int m, int n);

/*
Calculates FFT as well as Inverse FFT of array
a => input array
size => size
invert => True/FFT , False/IFFT
*/
void fft_util(cd* a, int size, bool invert);


/*
return false if no audio chunk returned i.e size is zero
return 256 size always
return less than 256 for ending part
*/
bool get_audio_chunk(double* audio, int* size);

/*
return true for successfull write
*/
bool write_audio_chunk(double* audio, int size);

/*
Appends spectrum at the end of model input
Also pops the beginning column and appends at last
pos => column position to append
*/
template<class T>
void append_to_model_input(T model_input[spectrum_size][num_segments], T spectrum[], int spect_size, int pos){
    
    if(pos < num_segments){
        for(int i=0 ; i< spect_size ; i++){
            model_input[i][pos] = spectrum[i];
        }

    }else{

        // shift all columns to left
        for(int i=0 ; i< spect_size ; i++){
            for(int j=0 ; j < pos-1 ; i++){
                model_input[i][j] = model_input[i][j+1];
            }
        }

        // add spectrum in last column
        for(int i=0 ; i< spect_size ; i++){
            model_input[i][pos-1] = spectrum[i];
        }

    }
}

/*
Appends output at the end of model output
Also pops the beginning column and appends at last
pos => column position to append
*/
template<class T>
void append_to_model_output(T model_output[spectrum_size][out_num_segments],T output[], int spect_size, int pos){

    if(pos < out_num_segments){
        for(int i=0 ; i< spect_size ; i++){
            model_output[i][pos] = output[i];
        }

    }else{

        // shift all columns to left
        for(int i=0 ; i< spect_size ; i++){
            for(int j=0 ; j < pos-1 ; i++){
                model_output[i][j] = model_output[i][j+1];
            }
        }

        // add output in last column
        for(int i=0 ; i< spect_size ; i++){
            model_output[i][pos-1] = output[i];
        }

    }
}

/*
De-Normalize values around given mean and std deviation
Usage: normalize((double *)arr, m, n, mean, std_dev) 
where arr is m x n 2D array
*/
void denormalize(double arr[spectrum_size][out_num_segments], double denorm[spectrum_size][out_num_segments], int m, int n, double mean, double std_dev);

/*
Get phase of recent 4(required num) spectrums
*/
void get_latest_phase(double in_phase[spectrum_size][num_segments], double out_phase[num_segments][out_num_segments]);
