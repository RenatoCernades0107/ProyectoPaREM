# Makefile for compiling parem.cpp on macOS with OpenMP

# Compiler and flags
CXX := clang++
CXXFLAGS := -Xpreprocessor -fopenmp -std=c++17 -g -fopenmp-version=51 -Wno-deprecated-declarations

# Python virtual environment paths
VENV_PATH := /Users/joaquin/Desktop/paralelas/ProyectoPaREM/venv
PYTHON_INCLUDE := $(VENV_PATH)/include/python3.10
PYTHON_LIB := $(VENV_PATH)/lib/python3.10

# Path to the Python framework on macOS
PYTHON_FRAMEWORK_INCLUDE := /Library/Frameworks/Python.framework/Headers

# Fix for NumPy include path
NUMPY_INCLUDE := $(VENV_PATH)/lib/python3.10/site-packages/numpy/_core/include

# Linker flags
LDFLAGS := -lomp -L/opt/homebrew/opt/libomp/lib \
           -L$(VENV_PATH)/lib/python3.10 -lpython3.10

# Include directories
INCLUDES := -I/opt/homebrew/opt/libomp/include \
            -I$(PYTHON_INCLUDE) \
            -I$(NUMPY_INCLUDE) \
            -I$(PYTHON_FRAMEWORK_INCLUDE)

# Target executable
TARGET := main

# Source file
SRC := parem.cpp

# Default number of OpenMP threads
OMP_THREADS ?= 4

# macOS compilation target
macos:
	# Ensure the virtual environment is activated before compiling
	if [ -z "$$VIRTUAL_ENV" ]; then echo "Please activate your virtual environment first"; exit 1; fi
	OMP_NUM_THREADS=$(OMP_THREADS) $(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(INCLUDES)

# Clean target to remove generated files
clean:
	rm -f $(TARGET)
