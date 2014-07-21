/**
 *
 *  _   _                                  _     _            
 * | |_| |__   ___    _ __ ___   __ _  ___| |__ (_)_ __   ___ 
 * | __| '_ \ / _ \  | '_ ` _ \ / _` |/ __| '_ \| | '_ \ / _ \
 * | |_| | | |  __/  | | | | | | (_| | (__| | | | | | | |  __/
 *  \__|_| |_|\___|  |_| |_| |_|\__,_|\___|_| |_|_|_| |_|\___|
 *                                                         
 */


#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "network.h"

int main (void)
{



	auto params = machine::Network::Parameters();

	params
		.inputs(4)
		.outputs(2)
		.biasTerm(true)
		.rate(0.001)
		.activation( machine::hyperbolic_tan )
		.initialization( machine::random )
		.propogation( machine::dotprod )
		.training( machine::backPropogation );

	auto net = machine::Network( params );

	// net.save( "testsave.bin" );

	for (auto it = net.begin(); it != net.end(); ++it)
	{
		std::cout << (*it)->size() << std::endl;
	}

	// std::vector<double> input = { 1.0, 2.0, 3.0, 4.0 };
	// std::vector<double> expected = { 1.0, 2.0 };

	// net.toggleTrainingMode();

	// net.train( input, expected );







	// auto classifier = machine::Classifier<std::string>( net );


	// std::vector<double> result = net.feedForward( input );

	// std::cout << result[0] << " " << result[1] << std::endl;

	// machine::Machine();
	// machine::stats::HiddenMarkovModel<std::string>();




	return 0;
}

