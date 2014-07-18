cxx = g++
cxxflags = -std=c++11
src = machine.cpp
deps = network-classes.cpp network-functions.cpp
# target = machine

all: machine

deps: $(deps)
	$(cxx) $(cxxflags) $(deps) -o $(subst .cpp,.o,$(deps))

machine:	
	$(cxx) $(cxxflags) $(deps) $(src)