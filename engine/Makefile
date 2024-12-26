# build:
# 	g++ -Wall -std=c++11 ./src/*.cpp -o engine -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE \
#     -L/opt/homebrew/lib -lSDL2

# Compiler settings for C and C++
CXX=g++

# Compiler flags for C and C++
# This is including debug flags, I need to take out the -g option afterwards
CXXFLAGS=-Wall -std=c++14 -g -D_THREAD_SAFE

# Directories for SDL2 include and libraries
SDL2_INCLUDE=-I/opt/homebrew/include/SDL2 -I/opt/homebrew/include/SDL2_ttf
SDL2_LIB=-L/opt/homebrew/lib -lSDL2 -lSDL2_ttf

# Include directory for vector.h (relative path to the ../vector/ folder)
VECTOR_INCLUDE=-I../vector

GRAPHICS_INCLUDE=-I../graphics

INPUT_MANAGER_INCLUDE=-I../input_manager

COLLISION_INCLUDE=-I../collision

UTILS_INCLUDE=-I../utils

# The build target executable
TARGET=engine

# Source files
CXXSRC=$(wildcard ./src/*.cpp) \
       ../vector/vector2.cpp \
       ../graphics/drawing.cpp \
       ../graphics/graphics.cpp \
       ../graphics/text.cpp \
       ../input_manager/input_manager.cpp \
       ../graphics/shape.cpp \
       ../graphics/rectangle.cpp \
       ../graphics/circle.cpp \
       ../graphics/polygon.cpp \
       ../graphics/polygonFactory.cpp \
       ../graphics/line.cpp \
       ../collision/collision.cpp \
       ../collision/manifold.cpp \
       ../collision/supportPoint.cpp \
       ../utils/utils.cpp

# Object files
CXXOBJS=$(CXXSRC:.cpp=.o)

# Default target: build the executable
$(TARGET): $(CXXOBJS)
	$(CXX) $(CXXFLAGS) $(CXXOBJS) $(SDL2_INCLUDE) $(SDL2_LIB) -o $(TARGET)

# Pattern rule for compiling C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SDL2_INCLUDE) $(VECTOR_INCLUDE) $(DRAWING_INCLUDE)\
	$(GRAPHICS_INCLUDE) $(INPUT_MANAGER_INCLUDE) $(COLLISION_INCLUDE) \
       $(UTILS_INCLUDE) -c $< -o $@

# Clean target: remove object files and the executable
clean:
	rm -f $(COBJS) $(CXXOBJS) $(TARGET)

# Run the executable
run: $(TARGET)
	./$(TARGET)
