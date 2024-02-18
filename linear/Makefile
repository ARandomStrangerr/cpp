# Makefile for compiling test.cpp and linear.cpp with g++

CXX = g++
CXXFLAGS = -std=c++11

# Source files
SRCS = test.cpp linear.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = myprogram

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
