all: clean build run

run: 
	./main

build: main.cpp
	g++ -std=c++11 -pthread -o main main.cpp
 
clean:
	rm main.exe