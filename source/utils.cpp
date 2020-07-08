#include<iostream>
#include<complex>
#include "../headers/utils.h"
using namespace std;

typedef complex<double> cd;



// using function:
// mean((double *)arr, m, n) where arr is m x n 2D array
double mean(double* arr, int m, int n) 
{ 
    // Calculating sum 
    int sum = 0; 
    for (int  i = 0; i < m; i++)  
        for (int j = 0; j < n; j++) 
            sum += *((arr+i*n) + j); 
      
    // Returning mean 
    return sum / (m * n); 
}

double mean_square(double* arr, int m, int n) 
{ 
    // Calculating sum 
    double sum = 0;
    double val = 0;
    for (int  i = 0; i < m; i++)  
        for (int j = 0; j < n; j++) {
            val = *((arr+i*n) + j); 
            sum += val*val;
        }
            
      
    // Returning mean 
    return sum / (m * n); 
}

double std_deviation(double* arr, int m,int n, double mean) 
{ 
    double ex = mean;
    double ex2 = mean_square(arr, m, n);
    double var = ex2 - ex*ex;
    return sqrt(var);
} 

void normalize(double* arr, int m, int n, double mean, double std_dev){
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double temp = *((arr+i*n) + j);
            temp = (temp - mean)/std_dev;
            *((arr+i*n) + j) = temp;
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

void complex_from_polar(double* mag, double* phase, cd* spect, int size){
    for(int i=0; i<size; i++){
        spect[i] = polar(mag[i], phase[i]);
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


