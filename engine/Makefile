# build:
# 	g++ -Wall -std=c++11 ./src/*.cpp -o engine -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -L/opt/homebrew/lib -lSDL2

# Compiler settings for C and C++
CXX=g++

# Compiler flags for C and C++
CXXFLAGS=-Wall -std=c++14 -g -D_THREAD_SAFE

# Directories for SDL2 include and libraries
SDL2_INCLUDE=-I/opt/homebrew/include/SDL2
SDL2_LIB=-L/opt/homebrew/lib -lSDL2

# Include directory for vector.h (relative path to the ../vector/ folder)
VECTOR_INCLUDE=-I../vector

GRAPHICS_INCLUDE=-I../graphics

# The build target executable
TARGET=engine

# Source files
CXXSRC=$(wildcard ./src/*.cpp) ../vector/vector2.cpp ../graphics/graphics.cpp ../graphics/shape.cpp ../graphics/rectangle.cpp ../graphics/circle.cpp  ../graphics/polygon.cpp
# CXXSRC=$(wildcard ./src/*.cpp) ../vector/vector2.cpp ../graphics/drawing.cpp ../graphics/shape.cpp ../graphics/circle.cpp ../graphics/polygon.cpp ../graphics/rectangle.cpp 

# Object files
CXXOBJS=$(CXXSRC:.cpp=.o)

# Default target: build the executable
$(TARGET): $(CXXOBJS)
	$(CXX) $(CXXFLAGS) $(CXXOBJS) $(SDL2_INCLUDE) $(SDL2_LIB) -o $(TARGET)

# Pattern rule for compiling C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SDL2_INCLUDE) $(VECTOR_INCLUDE) $(GRAPHICS_INCLUDE) -c $< -o $@

# Clean target: remove object files and the executable
clean:
	rm -f $(COBJS) $(CXXOBJS) $(TARGET)

# Run the executable
run: $(TARGET)
	./$(TARGET)
