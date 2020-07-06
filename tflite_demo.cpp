#include <stdio.h>
#include <string>
#include "../tensorflow/tensorflow/lite/interpreter.h"
#include "../tensorflow/tensorflow/lite/kernels/register.h"
#include "../tensorflow/tensorflow/lite/model.h"
#include "../tensorflow/tensorflow/lite/tools/gen_op_registration.h"

int main(){

    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile("test2.tflite");

    if(!model){
        printf("Failed to mmap model\n");
        exit(0);
    }

    tflite::ops::builtin::BuiltinOpResolver resolver;
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::InterpreterBuilder(*model.get(), resolver)(&interpreter);

    // Resize input tensors, if desired.
    interpreter->ResizeInputTensor(0,{1,129,8,1});
    interpreter->AllocateTensors();

    float* input = interpreter->typed_input_tensor<float>(0);
    float my_input[1][129][8][1];
    float my_output[1][129][1][1];
    // Dummy input for testing
    for(int i=0;i<129;i++){
        for(int j=0;j<8;j++){
            my_input[0][i][j][0] = 0.5;
        }
    }

    memcpy(input,my_input,sizeof(my_input));


    interpreter->Invoke();

    float* output = interpreter->typed_output_tensor<float>(0);

    memcpy(my_output,output,sizeof(my_output));
    printf("Result is:\n");
    for(int i=0;i<129;i++){
        for(int j=0;j<1;j++){
            printf("%f\n",my_output[0][i][j][0]);
        }
    }

    return 0;
}
