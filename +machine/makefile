
deps = ../src/network-fun.cpp ../src/network-obj.cpp
cpp = $(deps) constructor.cpp
target = constructor
target_ext = mexmaci64

cxx = g++
cxxflags = -O2 -Wall -std=c++11 -DMATLAB_MEX_FILE -shared

matlab_dir = /Applications/MATLAB_R2013a.app/
libs = -lmex -lmx -lmwlapack -lmwblas -leng
lib_path = -L$(matlab_dir)extern/lib/maci64/ -L$(matlab_dir)bin/maci64
inc_path = -I$(matlab_dir)extern/include/

all:
	$(cxx) $(cxxflags) $(cpp) -o $(target).$(target_ext) $(lib_path) $(inc_path) $(libs)