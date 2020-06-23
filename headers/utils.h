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

template<class T>
void copy_array(T* src, T* dest, int size){
    for(int i=0; i<size; i++){
        dest[i] = src[i];
    }
}

double mean(double* arr, int m, int n);
double mean_square(double* arr, int m, int n);
double std_deviation(double* arr, int m,int n, double mean);
void magnitude(cd* spectrum, double* mag, int size);
void angle(cd* spectrum, double* ang, int size);
void complex_from_polar(double* mag, double* phase, cd* spect, int size);
void fft_util(cd* a, int size, bool invert);


