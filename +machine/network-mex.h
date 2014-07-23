#ifndef NETWORK_MEX_H
#define NETWORK_MEX_H

#include "../src/network.h"
#include "mex.h"
#include "mexutils.h"
#include <string>

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

namespace machine {

/**
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 			 						Mex Parameters
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Construct a Network parameters object from a mex structure array
 *
 */
class MexParameters : public Network::Parameters
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
				auto value = mxGetField(params, i, fieldname);

				// Marshal lets us cast mex pointers to native C++ types
				auto m = mex::Marshal(value);

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
						break;
					case str2int("initialization") :
					case str2int("init") :
						break;
					case str2int("propogation") :
					case str2int("prop") :
						break;
					case str2int("training") :
					case str2int("train") :
						break;
				}
			}
		}
	}

	~MexParameters(){}
	
};

}

#endif

