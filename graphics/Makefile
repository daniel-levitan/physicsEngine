# the compiler: gcc for C program, define as g++ for C++
CC=g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
# CFLAGS  = -g -Wall
CFLAGS= -std=c++11

# the build target executable:
TARGET = shapeTest

# SRCS = shapeTest.cpp ../vector/vector2.cpp
SRCS = shapeTest.cpp shape.cpp circle.cpp polygon.cpp ../vector/vector2.cpp

DEPS = shape.cpp circle.cpp polygon.cpp

OBJS = $(SRCS:.cpp=.o)

INCLUDES = -I../vector

# Default target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Pattern rule for .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(TARGET)

