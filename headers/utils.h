#include<complex.h>
using namespace std;

typedef complex<double> cd;
const double PI = acos(-1);

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
double mean(double* arr, int m, int n);

/*
Calculates mean of squares of elements in given 2D array
Usage: mean_square((double *)arr, m, n) 
where arr is m x n 2D array
*/
double mean_square(double* arr, int m, int n);

/*
Calculates std deviation of elements in given 2D array
Usage: std_deviation((double *)arr, m, n, mean) 
where arr is m x n 2D array
*/
double std_deviation(double* arr, int m,int n, double mean);

/*
Normalize values around given mean and std deviation
Usage: normalize((double *)arr, m, n, mean, std_dev) 
where arr is m x n 2D array
*/
void normalize(double* arr, int m, int n, double mean, double std_dev);

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
void complex_from_polar(double* mag, double* phase, cd* spect, int size);

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
Appends spectrum at the end of model input
Also pops the beginning column and appends at last
pos => column position to append
*/
template<class T>
void append_to_model_input(T model_input[1][129][8][1], T spectrum[], int spect_size, int pos){
    
    if(pos < 8){
        for(int i=0 ; i< spect_size ; i++){
            model_input[0][i][pos][0] = spectrum[i];
        }

    }else{
        
        // shift all columns to left
        for(int i=0 ; i< spect_size ; i++){
            for(int j=0 ; j < pos-1 ; i++){
                model_input[0][i][j][0] = model_input[0][i][j+1][0];
            }
        }

        // add spectrum in last column
        for(int i=0 ; i< spect_size ; i++){
            model_input[0][i][pos-1][0] = spectrum[i];
        }

    }
}


