#include<iostream>
#include<complex>
#include "../headers/utils.h"
using namespace std;

typedef complex<double> cd;



// using function:
// mean((double *)arr, m, n) where arr is m x n 2D array
double mean(double arr[spectrum_size][num_segments], int m, int n) 
{ 
    // Calculating sum 
    double sum = 0.0; 
    for (int  i = 0; i < m; i++)  
        for (int j = 0; j < n; j++) 
            sum += arr[i][j]; 
      
    // Returning mean 
    return sum / (m * n); 
}

double mean_square(double arr[spectrum_size][num_segments], int m, int n) 
{ 
    // Calculating sum 
    double sum = 0;
    double val = 0;
    for (int  i = 0; i < m; i++)  
        for (int j = 0; j < n; j++) {
            val = arr[i][j]; 
            sum += val*val;
        }
            
      
    // Returning mean 
    return sum / (m * n); 
}

double std_deviation(double arr[spectrum_size][num_segments], int m,int n, double mean) 
{ 
    double ex = mean;
    double ex2 = mean_square(arr, m, n);
    double var = ex2 - ex*ex;
    return sqrt(var);
} 

void normalize(double arr[spectrum_size][num_segments], double norm[spectrum_size][num_segments], int m, int n, double mean, double std_dev){
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            norm[i][j] = (arr[i][j] - mean)/std_dev;
        }
        
    }
    
}

void magnitude(cd* spectrum, double* mag, int size){
    for(int i=0; i<size; i++){
        mag[i] = abs(spectrum[i]);
    }
}

void angle(cd* spectrum, double* ang, int size){
    for(int i=0; i<size; i++){
        ang[i] = arg(spectrum[i]);
    }
}

void complex_from_polar(double mag[spectrum_size][out_num_segments], double phase[spectrum_size][out_num_segments], cd spect[spectrum_size][out_num_segments], int m, int n){
    for(int i=0; i<m; i++){
        for(int j=0; j<n; j++){
            spect[i][j] = polar(mag[i][j], phase[i][j]);
        }
        
    }
}

void fft_util(cd* a, int size, bool invert) {
    int n = size;

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i+j], v = a[i+j+len/2] * w;
                a[i+j] = u + v;
                a[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (int i = 0; i < n; i ++)
            a[i] /= n;
    }
}

bool get_audio_chunk(double* audio, int* size){
    // reading code goes heree
    return true;
}

bool write_audio_chunk(double* audio, int size){
    // writing code goes here
    return true;
}

void denormalize(double arr[spectrum_size][out_num_segments], double denorm[spectrum_size][out_num_segments], int m, int n, double mean, double std_dev){
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            denorm[i][j] = arr[i][j]*std_dev + mean;
        }
        
    }
}

void get_latest_phase(double in_phase[spectrum_size][num_segments], double out_phase[num_segments][out_num_segments]){
    
    int shift = num_segments - out_num_segments;
    for(int j=0; j<out_num_segments; j++){
        for(int i=0; i<spectrum_size; i++){
            out_phase[i][j] = in_phase[i][j + shift];
        }
    }
}

