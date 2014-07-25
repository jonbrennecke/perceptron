/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   _   _                                  _     _            
 *  | |_| |__   ___    _ __ ___   __ _  ___| |__ (_)_ __   ___ 
 *  | __| '_ \ / _ \  | '_ ` _ \ / _` |/ __| '_ \| | '_ \ / _ \
 *  | |_| | | |  __/  | | | | | | (_| | (__| | | | | | | |  __/
 *   \__|_| |_|\___|  |_| |_| |_|\__,_|\___|_| |_|_|_| |_|\___|
 *                                                          
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * A machine learning experiment by @jonbrennecke / https://github.com/jonbrennecke
 *
 * Released under the MIT license (see the accompanying LICENSE.md)
 */  

#include <string>
#include "network.h"
#include "mex.h"
#include "mexutils.h"

/**
 * in Matlab, this function takes the parameters:
 * 		:param handle - a pointer to a C++ Network class
 *		:param inputV - input vector
 * and returns
 *		:return - the ouput vector of the network
 *
 * in C++, this function takes the parameters:
 * 		:param nlhs - Number of output (left-side) arguments (the size of the plhs array)
 * 		:param plhs - Array of output arguments.
 * 		:param nrhs - Number of input (right-side) arguments (or the size of the prhs array)
 * 		:param prhs - Array of input arguments.
 *
 */
void mexFunction ( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if ( nrhs > 1 ) 
	{
		// cast the mxArray* back to a Network*
		auto handle = mex::Handle<machine::Network>(prhs[0]);
		machine::Network* net = handle;

		// cast the second argument into a vector
		std::vector<double> inputV = mex::mex2vector<double>(prhs[1]);

		// feed the input vector forward into the network
		std::vector<double> output = net->feedForward(inputV);

		// return the output vector
		plhs[0] = mex::vector2mex<double>(output);
	}

}