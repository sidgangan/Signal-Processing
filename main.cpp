#include<iostream>
#include<complex>
#include "headers/utils.h"
#include "headers/dsp.h"
using namespace std;

typedef complex<double> cd;
/************* UTILS ************/

template<class T>
void print_array(T* a, int start, int len, int stride, bool column=false){
    for(int i=start; i< (start+len); i += stride){

        if(column)
            cout << a[i] << endl;
        else
        {
            cout << a[i] << "\t";
        }
        
    }
    cout<<endl;
}





//-----------------------------------------------------------------------------------------------------------------//

int main(){
	
	
    int audio_size = 16;
    int n_fft = 16;
    int hop = 2;
    int n_frames = 1;
    int size = n_fft + hop*(n_frames - 1);
    int spectrum_size = audio_size/2 +1;

    double audio[] = {0.5,1,3,4,0.5,1.5,3.5,4.5,4.5,3.5,1.5,0.5,4,3,1,0.5};
    cd* spectrum = (cd *)malloc(spectrum_size * sizeof(cd));
    double* restored_audio = (double *)malloc(audio_size * sizeof(double));
    double* abs_spect = (double *)malloc(spectrum_size * sizeof(double));
    double* phase = (double *)malloc(spectrum_size * sizeof(double));
    cd* spect = (cd *)malloc(spectrum_size * sizeof(cd));

    double* win = (double*)calloc(n_fft,__SIZEOF_DOUBLE__); // array for hamming window
    double* x = (double* )calloc(size, __SIZEOF_DOUBLE__);  // array for sumsquare

    // hamming(win,n_fft);
    // window_sumsquare(x,size,n_frames,win,n_fft,hop);


    // cout<<"****************AUDIO****************"<<endl;
    // print_array(audio,0,audio_size,1);

    // cout<<"****************FFT******************"<<endl;
    // stft(audio, audio_size, spectrum, win);
    // print_array(spectrum,0,spectrum_size,1,true);

    // cout<<"****************ABS******************"<<endl;
    // magnitude(spectrum,abs_spect,spectrum_size);
    // print_array(abs_spect,0,spectrum_size,1,true);

    // cout<<"****************ANGLE****************"<<endl;
    // angle(spectrum,phase,spectrum_size);
    // print_array(phase,0,spectrum_size,1,true);

    // cout<<"****************COMPLEX FROM POLAR***"<<endl;
    // complex_from_polar(abs_spect, phase, spect, spectrum_size);
    // print_array(spect,0,spectrum_size,1,true);

    // cout<<"****************INVERSE**************"<< endl;
    // IRFFT(spectrum,spectrum_size,restored_audio);
    // print_array(restored_audio,0,audio_size,1,false);

    double mat[3][3] = { { 1, 2, 30 }, 
                      { 4, 5, 6 }, 
                      { 7, 8, 9 } }; 

    double mu = mean((double *)mat,3,3);
    double sigma = std_deviation((double *)mat,3,3,mu);
    normalize((double *)mat,3,3,mu,sigma);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout<<mat[i][j]<<"\t";
        }
        cout<<endl;
        
    }

	return 0;
}
