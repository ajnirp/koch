all: koch

koch: koch.cpp
	g++ koch.cpp -Wall -O3 -lGL -lGLU -lglut -o koch

clean:
	rm -f koch

.PHONY: all clean