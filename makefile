CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
LDFLAGS = -pthread

SRCS = main.cpp scheduler.cpp log.cpp process.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = schedule

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
