# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Wextra -Wpedantic

# Target executable
TARGET = MediaApp

# Source files
SRCS = Media.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Build rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the application
run: $(TARGET)
	./$(TARGET)