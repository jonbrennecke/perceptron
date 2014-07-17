cxx = g++
cxxflags = -std=c++11
src = machine.cpp
deps = network.cpp
# target = machine

all: machine

deps: $(deps)
	$(cxx) $(cxxflags) $(deps) -o $(subst .cpp,.o,$(deps))

machine:	
	$(cxx) $(cxxflags) $(deps) $(src)