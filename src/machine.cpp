/**
 *
 *  _   _                                  _     _            
 * | |_| |__   ___    _ __ ___   __ _  ___| |__ (_)_ __   ___ 
 * | __| '_ \ / _ \  | '_ ` _ \ / _` |/ __| '_ \| | '_ \ / _ \
 * | |_| | | |  __/  | | | | | | (_| | (__| | | | | | | |  __/
 *  \__|_| |_|\___|  |_| |_| |_|\__,_|\___|_| |_|_|_| |_|\___|
 *                                                         
 */



#include "network.h"
#include <map>
#include <string>


int main (void)
{

	auto params = machine::Network::Parameters();

	params
		.inputs(10)
		.outputs(15)
		.biasTerm(true)
		.rate(0.001)
		.activation( machine::hyperbolic_tan )
		.initialization( machine::random )
		.propogation( machine::dotprod );

	auto net = new machine::Network( params );

	// machine::nets::Classifier<std::string>( net );
	// machine::stats::HiddenMarkovModel<std::string>();




	return 0;
}

