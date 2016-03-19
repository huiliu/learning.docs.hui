
#CXXFLAGS = -g -std=c++11
CXXFLAGS = -std=c++11
LIBS = inline.o main.o
TARGETS = main

all: $(TARGETS)

main: $(LIBS)
	g++ $(CXXFLAGS) $^ -o $@

inline.o: inline.cpp inline.h
	g++ $(CXXFLAGS) inline.cpp -c -o $@

clean:
	rm -f $(LIBS) $(TARGETS)
