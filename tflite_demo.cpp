#include <stdio.h>
#include <string>
#include "../tensorflow/tensorflow/lite/interpreter.h"
#include "../tensorflow/tensorflow/lite/kernels/register.h"
#include "../tensorflow/tensorflow/lite/model.h"
#include "../tensorflow/tensorflow/lite/tools/gen_op_registration.h"

int main(){

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
    interpreter->ResizeInputTensor(0,{1,129,8,1});

    // Allocate tensors
    interpreter->AllocateTensors();

    // pointer to input tensor of interpreter
    float* input = interpreter->typed_input_tensor<float>(0);

    // create input output arrays of required dimensions
    float my_input[129][8]; //same as (1,129,8,1)
    float my_output[129]; //same as (1,129,1,1)


    // Dummy input for testing
    for(int i=0;i<129;i++){
        for(int j=0;j<8;j++){
            my_input[i][j] = 1.5;
        }
    }


    // copy input array to tflite input
    memcpy(input,my_input,sizeof(my_input));

    // run model
    interpreter->Invoke();

    // pointer to output tensor of interpreter
    float* output = interpreter->typed_output_tensor<float>(0);

    // copy tflite output to output array
    memcpy(my_output,output,sizeof(my_output));


    printf("Result is:\n");
    for(int i=0;i<129;i++){
            printf("%f\n",my_output[i]);
        
    }

    return 0;
}
