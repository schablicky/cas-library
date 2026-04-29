CXX = g++
CXXFLAGS = -Wall -std=c++17

TARGET = cas_app

.PHONY: all
all: main.o cas_lib.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o cas_lib.o


main.o: main.cpp cas_lib.h
	$(CXX) $(CXXFLAGS) -c main.cpp


cas_lib.o: cas_lib.cpp cas_lib.h
	$(CXX) $(CXXFLAGS) -c cas_lib.cpp


.PHONY: run
run: all
	./cas_app


.PHONY: clean
clean:
	rm -f *.o $(TARGET)