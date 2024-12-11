# the compiler: gcc for C program, define as g++ for C++
CC=g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
# CFLAGS  = -g -Wall
CFLAGS= -std=c++11

# the build target executable:
TARGET = vectorTest

SRCS = vectorTest.cpp vector2.cpp

DEPS = vector2.cpp

OBJS = $(SRCS:.cpp=.o)
# %.o: %.c $(DEPS)
# 	$(CC) -c -o $@ $< $(CFLAGS)

# all: $(TARGET)

# $(TARGET): $(TARGET).cpp
# 	$(CC) $(CFLAGS) -o $(TARGET) $(DEPS) $(TARGET).cpp

# clean:
# 	$(RM) $(TARGET)

# Default target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Pattern rule for .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(TARGET)

