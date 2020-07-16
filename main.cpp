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
	
	
    // int audio_size = 256;
    // int n_fft = 256;
    // int hop = 64;
    // int n_frames = 4;
    // int size = n_fft + hop*(n_frames - 1);
    // int spectrum_size = audio_size/2 +1;
    // int num_segments = 8;


    // double audio[] = {0.5,1,3,4,0.5,1.5,3.5,4.5,4.5,3.5,1.5,0.5,4,3,1,0.5};


    cd* spectrum = (cd *)malloc(spectrum_size * sizeof(cd));
    double* restored_audio = (double *)malloc(audio_size * sizeof(double));
    double* abs_spect = (double *)malloc(spectrum_size * sizeof(double));
    double* phase = (double *)malloc(spectrum_size * sizeof(double));
    cd* spect = (cd *)malloc(spectrum_size * sizeof(cd));

    double* win = (double*)calloc(n_fft,__SIZEOF_DOUBLE__); // array for hamming window
    double* win_sumsquare = (double* )calloc(sumsquare_size, __SIZEOF_DOUBLE__);  // array for sumsquare

    // hamming(win,n_fft);
    // window_sumsquare(win_sumsquare,size,n_frames,win,n_fft,hop);


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

    // double mat[3][3] = { { 1, 2, 30 }, 
    //                   { 4, 5, 6 }, 
    //                   { 7, 8, 9 } }; 

    // double mu = mean((double *)mat,3,3);
    // double sigma = std_deviation((double *)mat,3,3,mu);
    // normalize((double *)mat,3,3,mu,sigma);

    // for (int i = 0; i < 3; i++)
    // {
    //     for (int j = 0; j < 3; j++)
    //     {
    //         cout<<mat[i][j]<<"\t";
    //     }
    //     cout<<endl;
        
    // }


    // END TO END FLOW

    double mu = 0;
    double sigma = 0;

    double input_audio_chunk[audio_size]; // consider this as temporary audio chunk, we will get new chunks in this array always
    cd model_input_spect[129][8]; // should be allocated dynamically
    double model_input_phase[129][8]; // same as  (1,129,8,1), verified in tflite code
    double model_input_mag[129][8];
    double normalized_model_input[129][8];
    
    double normalized_model_output[129]; //same as (1,129,1,1)
    double model_output_mag[129][4];
    double model_output_denorm[129][4];
    double model_output_phase[129][4];
    cd model_output_spect[129][4];
    double* restored_output_audio = (double*)calloc(sumsquare_size,__SIZEOF_DOUBLE__);
    double output_audio_chunk[64]; // this will be written to output buffer
    
    int chunk_size;
    int model_input_size = 0; // iterating over columns, 0 to 7
    int model_output_size = 0; // iterating over columns, 0 to 3

    hamming(win,n_fft); // allocating hamming window
    window_sumsquare(win_sumsquare,sumsquare_size,n_frames,win,n_fft,hop); // allocatinf sumsquare envelop array

    // start loop for denoising
    while(get_audio_chunk(input_audio_chunk, &chunk_size)){
        // zero pad if less than window length
        if(chunk_size < audio_size){
            for(int i=chunk_size; i<audio_size; i++){
                input_audio_chunk[i] = 0.0;
            }
        }

        // apply fft
        stft(input_audio_chunk,audio_size,spectrum, win);

        // calculate phase
        angle(spectrum,phase,spectrum_size);

        // calculate magnitude
        magnitude(spectrum,abs_spect,spectrum_size);

        // append spectrum to model_input_spect
        append_to_model_input(model_input_spect,spectrum,spectrum_size,model_input_size);

        // append phase to model_input_phase
        append_to_model_input(model_input_phase,phase,spectrum_size,model_input_size);

        // append abs_spect to model_input_mag
        append_to_model_input(model_input_mag,abs_spect,spectrum_size,model_input_size);

        // update model_input_size
        if(model_input_size < 8){

            ++model_input_size ;

        }else{
            // pass input to model
            
            // calculate mean and std dev and normalize inputs
            mu = mean(model_input_mag,spectrum_size,num_segments);
            sigma = std_deviation(model_input_mag,spectrum_size,num_segments,mu);
            normalize(model_input_mag,normalized_model_input,spectrum_size,num_segments,mu,sigma);

            // pass normalized model input to the Denoising mode
            // 
            // code of passing input to tflite and taking output goes here
            // 
            // process the output of model (1D array of size 129)

            append_to_model_output(model_output_mag,normalized_model_output,spectrum_size,model_output_size);

            if(model_output_size < 4){

                ++model_output_size;

            }else{
                // scale up the magnitude
                denormalize(model_output_mag,model_output_denorm,spectrum_size,out_num_segments,mu,sigma);

                // get phase of recent 4 spectrums
                get_latest_phase(model_input_phase, model_output_phase);

                // convert to spectrum form
                complex_from_polar(model_output_mag,model_output_phase,model_output_spect,spectrum_size,out_num_segments);

                // get output audio
                istft(model_output_spect,restored_output_audio,win,win_sumsquare);

                // get mid 64 audio points
                for(int i=0; i<64; i++){
                    output_audio_chunk[i] = restored_output_audio[i+192];
                }

                // write to output buffer
                write_audio_chunk(output_audio_chunk, 64);
            }


        }

        
    }


	return 0;
}
