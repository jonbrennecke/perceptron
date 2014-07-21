cxx = g++
cxxflags = -std=c++11 -Wall
src = machine.cpp
deps = network-obj.cpp network-fun.cpp
# target = machine

all: machine

deps: $(deps)
	$(cxx) $(cxxflags) $(deps) -o $(subst .cpp,.o,$(deps))

machine:	
	$(cxx) $(cxxflags) $(deps) $(src)