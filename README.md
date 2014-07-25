The Machine
===========

A machine learning experiment in C++11, with wrappers for Matlab. 

### installation
The compilation procedure is pretty rudimentary at the moment; the only supported platform is OS X, since that's what it's being developed on.

The development target will eventually use Waf so that compilation on Windows is easier. But at this stage of development I'm using GNU Make. To compile, simply run ```./make``` in the ```+machine/src``` directory.

NOTE: due to the use of ```classdefs``` and ```+folder``` directories, Octave isn't well supported. Eventually I'd like to make a branch for Octave, but that isn't likely to recieve any attention for some time.
