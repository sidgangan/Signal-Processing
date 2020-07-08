utils.o: source/utils.cpp headers/utils.h
	g++ -c -o utils.o source/utils.cpp

dsp.o: source/dsp.cpp headers/dsp.h headers/utils.h
	g++ -c -o dsp.o source/dsp.cpp

main.o: main.cpp headers/dsp.h headers/utils.h
	g++ -c -o main.o main.cpp

main: main.o dsp.o utils.o
	g++ -o main main.o dsp.o utils.o

tflite_demo: tflite_demo.cpp
	g++ tflite_demo.cpp -Itensorflow -Llibraries -ltensorflow-lite -lrt -ldl -pthread -o tflite_demo
