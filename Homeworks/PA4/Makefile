CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = robot_sim
SRCS = main.cpp Robot.cpp World.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)