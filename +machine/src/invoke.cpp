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
 *		:param expectedV - expected 
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

	// check args
	if ( nrhs < 1 )
		mexErrMsgTxt("Arguments required. See documentation for a list of parameters."); 
	if ( nrhs < 2 )
		mexErrMsgTxt("Second argument should not be empty. Expected a method to invoke.");

	// the first parameter should be a handle to the Network instance
	// so, cast the mxArray* back to a Network*
	auto handle = mex::Handle<machine::Network>(prhs[0]);
	machine::Network* net = handle;

	// the second parameter should be the name of a method to invoke
	char* method = mxArrayToString(prhs[1]);

	if(!strcmp("size", method)) {
    	plhs[0] = mxCreateDoubleScalar(net->size());
	    return;
	}

	if(!strcmp("rate", method)) {
    	plhs[0] = mxCreateDoubleScalar(net->rate());
	    return;
	}

	// if(!strcmp("size", method)) {
 //    	plhs[0] = mxCreateDoubleScalar(net->size());
	//     return;
	// }

    mxFree(method);

	// if ( nrhs > 1 ) 
	// {
	// 	// cast the mxArray* back to a Network*
	// 	// auto handle = mex::Handle<machine::Network>(prhs[0]);
	// 	// machine::Network* net = handle;

	// 	// // cast the second and third arguments into vectors
	// 	// std::vector<double> inputV = mex::mex2vector<double>(prhs[1]);
	// 	// std::vector<double> expectedV = mex::mex2vector<double>(prhs[2]);

	// 	// std::vector<double> output = net->train( inputV, expectedV );

	// 	// plhs[0] = mex::vector2mex<double>(output);
	// }

}