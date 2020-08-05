#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include<complex>
#include "headers/constants.h"
#include "headers/utils.h"
#include "headers/audio_utils.h"
#include "headers/dsp.h"
#include <stdio.h>
#include <string>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/tools/gen_op_registration.h"
using namespace std;

typedef complex<double> cd;







//-----------------------------------------------------------------------------------------------------------------//

int main(){
    /**********************READ AUDIO FILE BEGIN*************************************/
    FILE * infile = fopen("sounds/sample.wav","rb");		// Open wave file in read mode
	FILE * outfile = fopen("sounds/output.wav","wb");		// Create output ( wave format) file in write mode
    short int audio[1000000]; // array to store audio PCM data
    int temp_size = 0;
    int* size = &temp_size; // length of audio
    
	int BUFSIZE = 1;					// BUFSIZE can be changed according to the frame size required (eg:512)
	int count = 0;						// For counting number of frames in wave file.
	short int buff16[BUFSIZE];			// short int used for 16 bit as input data format is 16 bit PCM audio
	header_p meta = (header_p)malloc(sizeof(header));	// header_p points to a header struct that contains the wave file metadata fields
	int nb;
    if (infile)
	{
		fread(meta, 1, sizeof(header), infile);
		fwrite(meta,1, sizeof(*meta), outfile);
		cout << "Size of Header file is "<<sizeof(*meta)<<" bytes" << endl;
		cout << "Sampling rate of the input wave file is "<< meta->sample_rate <<" Hz" << endl;
        cout << "Bits per Sample is "<< meta->bits_per_sample << endl;
		cout << "Number of samples in wave file are " << meta->subchunk2_size*8/meta->bits_per_sample << " samples" << endl;

        *size = meta->subchunk2_size*8/meta->bits_per_sample; // length of PCM data
        


		while (!feof(infile))
		{
			nb = fread(buff16,2,BUFSIZE,infile);		// Reading data in chunks of BUFSIZE
			//cout << nb <<endl;
			count++;					// Incrementing Number of frames

			/* Insert your processing code here*/
            audio[count-1] = buff16[0];


			// fwrite(buff16,2,nb,outfile);			// Writing read data into output file
		}
        cout << "Number of frames in the input wave file are " <<count << endl;
        cout << "Start audio PCM Data \n"<<endl;

        // print audio data
        // for(int i=0 ; i< size+1 ; i++){
        //     cout<<audio[i]<<endl;
        // }
	}

    fclose(infile);
    /**********************READ AUDIO FILE ENDS*************************************/

    /*********************TF-LITE INITIALIZATIONS BEGIN******************************/
    // load model
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile("test2.tflite");
    
    if(!model){
        printf("Failed to mmap model\n");
        exit(0);
    }

    // build interpreter
    tflite::ops::builtin::BuiltinOpResolver resolver;
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::InterpreterBuilder(*model.get(), resolver)(&interpreter);

    // Resize input tensors
    interpreter->ResizeInputTensor(0,{1,129,8,1}); // not needed

    // Allocate tensors
    interpreter->AllocateTensors();

    // pointer to input tensor of interpreter
    float* input = interpreter->typed_input_tensor<float>(0);
    //float* input = interpreter->typed_tensor<float>(1);

    // pointer to output tensor of interpreter
    float* output = interpreter->typed_output_tensor<float>(0);
    //float* input = interpreter->typed_tensor<float>(0);

    // create input output arrays of required dimensions
    float my_input[129][8]; //same as (1,129,8,1)
    float my_output[129]; //same as (1,129,1,1)


    // Dummy input for testing
    for(int i=0;i<129;i++){
        for(int j=0;j<8;j++){
            my_input[i][j] = 1.5;
        }
    }
	/*********************TF-LITE INITIALIZATIONS END******************************/

    /*********************DSP INITIALIZATIONS BEGIN******************************/
    
    cd* spectrum = (cd *)malloc(spectrum_size * sizeof(cd));
    double* abs_spect = (double *)malloc(spectrum_size * sizeof(double));
    double* phase = (double *)malloc(spectrum_size * sizeof(double));

    double* win = (double*)calloc(n_fft,__SIZEOF_DOUBLE__); // array for hamming window
    double* win_sumsquare = (double* )calloc(sumsquare_size, __SIZEOF_DOUBLE__);  // array for sumsquare


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
    short output_audio_chunk[64]; // this will be written to output buffer
    
    int chunk_size=0; // length of audio data read
    int model_input_size = 0; // iterating over columns, 0 to 7
    int model_output_size = 0; // iterating over columns, 0 to 3
    int tflite_input_size = spectrum_size*num_segments*__SIZEOF_FLOAT__;
    int tflite_output_size = spectrum_size*__SIZEOF_FLOAT__;

    hamming(win,n_fft); // allocating hamming window
    window_sumsquare(win_sumsquare,sumsquare_size,n_frames,win,n_fft,hop); // allocatinf sumsquare envelop array

    /*********************DSP INITIALIZATIONS END******************************/

    int itr=0;
    /*********************DENOISING BEGIN******************************/
    // start loop for denoising
    while(get_chunk(audio,*size,input_audio_chunk, &itr, &chunk_size)){
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
        if(model_input_size < (num_segments-1)){

            ++model_input_size ;

        }else{

            if(model_input_size == (num_segments-1)){
                model_input_size++;
            }

            // pass input to model
            
            // calculate mean and std dev and normalize inputs
            mu = mean(model_input_mag,spectrum_size,num_segments);
            sigma = std_deviation(model_input_mag,spectrum_size,num_segments,mu);
            normalize(model_input_mag,normalized_model_input,spectrum_size,num_segments,mu,sigma);

            /** pass normalized model input to the Denoising mode **/

            // copy input array to tflite input
            memcpy(input,normalized_model_input,tflite_input_size); //memcpy doesnt work coz of float and double type misatch, use loop to copy elements

            // run model
            interpreter->Invoke();

            // copy tflite output to output array
            memcpy(normalized_model_output,output,tflite_output_size); //memcpy doesnt work coz of float and double type misatch, use loop to copy elements

            /** process the output of model (1D array of size 129) **/

            append_to_model_output(model_output_mag,normalized_model_output,spectrum_size,model_output_size);

            if(model_output_size < (out_num_segments-1)){

                ++model_output_size;

            }else{

                if(model_output_size == (out_num_segments-1)){
                    model_output_size++;
                }

                // scale up the magnitude
                denormalize(model_output_mag,model_output_denorm,spectrum_size,out_num_segments,mu,sigma);

                // get phase of recent 4 spectrums
                get_latest_phase(model_input_phase, model_output_phase);

                // convert to spectrum form
                complex_from_polar(model_output_denorm,model_output_phase,model_output_spect,spectrum_size,out_num_segments);

                // initialized restored output to zero
                for(int i=0; i<sumsquare_size; i++){
                    restored_output_audio[i] = 0.0;
                }

                // get output audio
                istft(model_output_spect,restored_output_audio,win,win_sumsquare);

                // get mid 64 audio points and scale them to 2 byte integer
                for(int i=0; i<64; i++){
                    output_audio_chunk[i] = restored_output_audio[i+192]*32768;
                }

                // write to output buffer
                //write_audio_chunk(output_audio_chunk, 64); //temporary commented
                fwrite(output_audio_chunk,2,64,outfile);			// Writing read data into output file
            }


        }

        
    }

    fclose(outfile);

    /*********************DENOISING END******************************/


	return 0;
}
