CXX := g++
CXXFLAGS := -Wall -Werror

BIN := main

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))

all: $(BIN)

$(BIN) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(OBJS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(BIN)