# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#   _   _                                  _     _            
#  | |_| |__   ___    _ __ ___   __ _  ___| |__ (_)_ __   ___ 
#  | __| '_ \ / _ \  | '_ ` _ \ / _` |/ __| '_ \| | '_ \ / _ \
#  | |_| | | |  __/  | | | | | | (_| | (__| | | | | | | |  __/
#   \__|_| |_|\___|  |_| |_| |_|\__,_|\___|_| |_|_|_| |_|\___|
#                                                          
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# A machine learning experiment by @jonbrennecke / https://github.com/jonbrennecke
#
# Released under the MIT license (see the accompanying LICENSE.md)
#  

cxxfiles = constructor feedForward
target_ext = mexmaci64
build_dir = ../+build

cxx = g++
cxxflags = -O2 -Wall -std=c++11

# netlib
# compile the network files into a dynamic library
netlib_dir = ../../src/
netlib_files = network-obj network-fun
netlib_ext = dylib
netlib_target = network
netlibflags = -dynamiclib -Wl -fPIC

mexflags = -shared -DMATLAB_MEX_FILE

matlab_dir = /Applications/MATLAB_R2013a.app/
libs = -lmex -lmx -lmwlapack -lmwblas -leng
lib_path = -L$(matlab_dir)extern/lib/maci64/ -L$(matlab_dir)bin/maci64
inc_path = -I$(matlab_dir)extern/include/ -I$(netlib_dir)


all: $(cxxfiles)

netlib-obj:
	# compiling object files
	$(cxx) $(cxxflags) -c $(addprefix $(netlib_dir), $(addsuffix .cpp,$(netlib_files)))

netlib: netlib-obj
	# compileing netlib
	$(cxx) $(netlibflags) -o $(netlib_target).$(netlib_ext) $(addsuffix .o,$(netlib_files))

$(cxxfiles): netlib-obj
	# compiling mex file: $@
	$(cxx) $(cxxflags) $(mexflags) $@.cpp $(addsuffix .o,$(netlib_files)) -o $(build_dir)/$@.$(target_ext) $(lib_path) $(inc_path) $(libs)

