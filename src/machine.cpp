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

	auto net = new machine::Network<double>( 2, 5 );

	// net->setActivation( machine::hyperbolic_tan );

	// net->setInitialization( machine::random );

	// net->init();

	// net->reset();



	return 0;
}

