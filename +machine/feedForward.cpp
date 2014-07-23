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
#include "../src/network.h"
#include "mex.h"
#include "mexutils.h"

/**
 * in Matlab, this function takes as parameters:
 * 		:param params - a pointer to the C++ Network class
 *
 * in C++, this function takes as parameters:
 * 		:param nlhs - Number of output (left-side) arguments (the size of the plhs array)
 * 		:param plhs - Array of output arguments.
 * 		:param nrhs - Number of input (right-side) arguments (or the size of the prhs array)
 * 		:param prhs - Array of input arguments.
 *
 */
void mexFunction ( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // check outputs
	// if (nlhs != 1)
	// 	mexErrMsgTxt("One output expected.");

	// cast the mxArray* back to a Network*
	if ( nrhs > 0 ) 
	{
		auto handle = mex::Handle<machine::Network>(prhs[0]);
		auto net = (machine::Network*)handle;
	}
}