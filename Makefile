# Makefile for compiling parem.cpp with OpenMP for macOS and Linux

# Compiler and flags
CXX := g++
CXXFLAGS := -fopenmp -std=c++17 -g -Wno-deprecated-declarations -D_DEBUG

# Python virtual environment paths (Linux)
VENV_PATH_LINUX := /home/juan-diego/Desktop/UTEC/2024-2/Parallel-Computing/ProyectoPaREM/venv
PYTHON_INCLUDE_LINUX := /usr/include/python3.12
NUMPY_INCLUDE_LINUX := $(VENV_PATH_LINUX)/lib/python3.12/site-packages/numpy/_core/include

# Linker flags for Python 3.12 (Linux)
LDFLAGS_LINUX := -lpython3.12

# Include directories for Linux
INCLUDES_LINUX := -I/usr/include/python3.12 \
                  -I$(VENV_PATH_LINUX)/lib/python3.12/site-packages/numpy/_core/include

# Target executable
TARGET := main

# Source file
SRC := parem.cpp

# Debug flags (you can modify or extend these)
DEBUG_CXXFLAGS := -DDEBUG -O0 -g -Wall -Wextra
DEBUG_LDFLAGS := -g

# Default number of OpenMP threads
OMP_THREADS ?= 4

# If DEBUG is set, add debug flags to compilation and linking
ifeq ($(DEBUG), 1)
    CXXFLAGS += $(DEBUG_CXXFLAGS)
    LDFLAGS += $(DEBUG_LDFLAGS)
endif

# macOS compilation target
macos:
	# Ensure the virtual environment is activated before compiling
	if [ -z "$$VIRTUAL_ENV" ]; then echo "Please activate your virtual environment first"; exit 1; fi
	OMP_NUM_THREADS=$(OMP_THREADS) $(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(INCLUDES_LINUX)

# Linux compilation target
linux:
	# Compile on Linux with the given Python and NumPy paths
	OMP_NUM_THREADS=$(OMP_THREADS) g++-13 -fopenmp $(SRC) \
        -I/usr/include/python3.12 \
        -I/home/juan-diego/Desktop/UTEC/2024-2/Parallel-Computing/ProyectoPaREM/venv/lib/python3.12/site-packages/numpy/_core/include \
        -lpython3.12 -o main 

linur:
	# Compile on Linux with the given Python and NumPy paths
	OMP_NUM_THREADS=$(OMP_THREADS) g++-11 -fopenmp $(SRC) \
        -I/usr/include/python3.10 \
        -I/home/renato/Documents/Parallel_Computing/ProyectoPaREM/venv/lib/python3.10/site-packages/numpy/_core/include \
        -lpython3.10 -o main 

# Clean target to remove generated files
clean:
	rm -f $(TARGET)
