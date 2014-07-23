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
 * Convert a string to an integer
 * --- 
 * this is useful because a switch is a little faster than a bunch 
 * of repeated ifs, especially if some of it can be evaluated at compile time.
 * But C++ switches can only take integers as inputs, so we must convert the string to an int
 *
 * from http://stackoverflow.com/questions/16388510/evaluate-a-string-with-a-switch-in-c
 */
static constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h+1)*33) ^ str[h];
}

/**
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 			 						Mex Parameters
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Construct a Network parameters object from a mex structure array
 *
 */
class MexParameters : public machine::Network::Parameters
{
public:
	MexParameters( const mxArray* params )
	{

		if ( mxIsStruct( params ) )
		{
			int nfields = mxGetNumberOfFields( params );

			for (int i = 0; i < nfields; ++i)
			{
				// retrieve the fieldname and value
				const char *fieldname = mxGetFieldNameByNumber(params, i);
				auto value = mxGetFieldByNumber(params, 0, i);

				// Marshal mex pointers to native C++ types
				auto m = mex::Marshal(value);

				// TODO, find a better way to do this than an incredibly ugly switch statement
				switch (str2int(fieldname))
				{
					case str2int("inputs") :
						this->inputs((unsigned int)m);
						break;
					case str2int("outputs") :
						this->outputs((unsigned int)m);
						break;
					case str2int("hiddenLayers") :
						this->hiddenLayers((unsigned int)m);
						break;
					case str2int("hiddenSize") :
						this->hiddenSize((unsigned int)m);
						break;
					case str2int("biasTerm") :
						this->hiddenSize((bool)m);
						break;
					case str2int("rate") :
						this->rate((double)m);
						break;
					case str2int("activation") :
					case str2int("act") :
					{
						if( mxIsChar(value) )
						{
							// find the correct activation function
							switch ( str2int((char*)m) )
							{
								case str2int("sigmoid") :
									this->activation(machine::sigmoid);
									break;
								case str2int("softplus") :
									this->activation(machine::softplus);
									break;
								case str2int("tanh") :
								case str2int("hyperbolic_tan") :
									this->activation(machine::hyperbolic_tan);
									break;
							}

						}
						break;
					}
					case str2int("initialization") :
					case str2int("init") :
					{
						if( mxIsChar(value) )
						{
							// find the correct initialization function
							switch ( str2int((char*)m) )
							{
								case str2int("random") :
									this->initialization(machine::random);
									break;
							}
						}
						break;
					}
					case str2int("propogation") :
					case str2int("prop") :
					{
						if( mxIsChar(value) )
						{	
							// find the correct propogation function
							switch ( str2int((char*)m) )
							{
								case str2int("dotprod") :
								case str2int("dot") :
									this->propogation(machine::dotprod);
									break;
							}
						}
						break;
					}
					case str2int("training") :
					case str2int("train") :
					{
						if( mxIsChar(value) )
						{	
							// find the correct propogation function
							switch ( str2int((char*)m) )
							{
								case str2int("backPropogation") :
									this->training(machine::backPropogation);
									break;
							}
						}
						break;
					}
				}
			}
		}
	}

	~MexParameters(){}
};

void mexFunction ( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // check outputs
	if (nlhs != 1)
		mexErrMsgTxt("One output expected.");

	// if parameters are passed, build a network with those parameters
	// otherwise, build a network with the default parameters
	auto params = MexParameters( prhs[0] );
	auto net = new machine::Network(params);

	plhs[0] = (mxArray*)mex::Handle<machine::Network>(net);
}