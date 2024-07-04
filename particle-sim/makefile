CXX = g++
CXXFLAGS = -std=c++11 -I /opt/homebrew/Cellar/sfml/2.6.1/include
LDFLAGS = -L /opt/homebrew/Cellar/sfml/2.6.1/lib
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system

SRCS = main.cpp particle.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = a.out

all : $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@
	rm -f $^

clean:
	rm -f $(OBJS) $(EXEC)
