utils.o: source/utils.cpp headers/utils.h headers/constants.h
	g++ -c -o utils.o source/utils.cpp

dsp.o: source/dsp.cpp headers/dsp.h headers/utils.h headers/constants.h
	g++ -c -o dsp.o source/dsp.cpp

main.o: main.cpp headers/dsp.h headers/utils.h headers/constants.h
	g++ -c -o main.o main.cpp -Itensorflow

main: main.o dsp.o utils.o
	g++ -o main main.o dsp.o utils.o -Llibraries -ltensorflow-lite -ldl -pthread

tflite_demo: tflite_demo.cpp
	g++ tflite_demo.cpp -Itensorflow -Llibraries -ltensorflow-lite -ldl -pthread -o tflite_demo
