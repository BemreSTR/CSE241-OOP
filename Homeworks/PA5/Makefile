CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = main

SRC = main.cpp
HEADERS = databaseorganizer.h entry.h field.h

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

clean:
	rm -f $(TARGET) *.o output.txt

.PHONY: all clean