# Makefile for compiling parem.cpp on macOS with OpenMP

# Compiler and flags
CXX := clang++
CXXFLAGS := -Xpreprocessor -fopenmp -std=c++11 -g
LDFLAGS := -lomp -L/opt/homebrew/opt/libomp/lib
INCLUDES := -I/opt/homebrew/opt/libomp/include

# Target executable
TARGET := main

# Source file
SRC := parem.cpp

# Default number of OpenMP threads
OMP_THREADS ?= 4

# macOS compilation target
macos:
	OMP_NUM_THREADS=$(OMP_THREADS) $(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(INCLUDES)

# Clean target to remove generated files
clean:
	rm -f $(TARGET)
