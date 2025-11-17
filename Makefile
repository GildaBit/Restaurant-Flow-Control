# Name: Gilad Bitton
# RedID: 130621085
# Makefile for Restaurant Flow Control

CXX = g++
CXXFLAGS = -Wall -Wextra -g -pthread
OBJS = main.o log.o
DEPS = log.h seating.h
TARGET = dineseating

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)