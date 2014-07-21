
#include <typeinfo>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>

#include "network.h"

namespace machine {

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 			Activation functions
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */

	// see http://en.wikipedia.org/wiki/Sigmoid_function
	ActFunction sigmoid { 
		activationFunctionFactory([]( double x ) {
			return 1 / ( 1 + exp(-x) );
		}), 
		activationFunctionFactory([]( double y ) {
			return y * (1 - y);
		})
	};

	// see http://en.wikipedia.org/wiki/Sigmoid_function
	ActFunction softplus {
		activationFunctionFactory([]( double x ) {
			return log10( 1 + exp(x) );
		}), 
		activationFunctionFactory([]( double y ) {
			return 1 / (1 + exp(-y));
		})
	};

	// see http://en.wikipedia.org/wiki/Hyperbolic_tangent
	ActFunction hyperbolic_tan {
		activationFunctionFactory([]( double x ) {
			return tanh(x);
		}), 
		// d(tanh)/dy = sech^2(y)
		activationFunctionFactory([]( double y ) {
			return pow( (2 * exp(-y)) / ( 1 + exp(-2 * y)), 2);
		})
	};

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 		Initialization functions
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */

	// the default init function generates a random number in the distribution [0,1)
	std::random_device rd;
	std::default_random_engine rng( rd() );
	std::uniform_real_distribution<double> dist(0,1);
	auto random = initFunctionFactory( std::bind( dist, rng ) );

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 			Propogation functions
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */

	// return the dot product of two vectors
	// assumes that bot vectors are of the same size
	double __dotprod ( std::vector<double> a, std::vector<double> b )
	{
		double c = 0;
		auto ita = a.begin();
		auto itb = b.begin();

		for (; ita != a.end(); ++ita, ++itb )
			c += (*ita) * (*itb);

		return c;
	}

	auto dotprod = propFunctionFactory( __dotprod );

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 			Training functions
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */

	/**
	 * 
	 * ~~~~~ Back Propogation ~~~~~
	 * 
	 * Train the network by 'back propogation'
	 * see http://en.wikipedia.org/wiki/Backpropagation
	 */
	void _backPropogation ( std::vector<double> input, std::vector<double> expected, Network& net )
	{
		// get the result of feeding the input into the network
		std::vector<double> output = net.feedForward(input);

		ActFunction actf = net.activate();
		double rate = net.rate();

		// ~~~~~ loop backwards over the layers ~~~~~
		// 
		// as we descend though the layers, the difference between the output and the expected 
		// result is propogated through each layer; while the change in weights (deltas) is computed 
		// on for each layer.
		for(auto layer = net.rbegin(); layer != net.rend(); ++layer)
		{
			// input and output for each layer
			std::vector<double> layer_input = (*layer)->getInput();
			std::vector<double> layer_output = (*layer)->getOutput();

			// iterate over the neurons in a vector
			auto out = layer_output.begin();
			for (auto neuron = (*layer)->begin(); neuron != (*layer)->end(); ++neuron, ++out)
			{

				// the output layer is handled a bit differently, as it can be compared directly with the 
				// expected answer
				auto ex = expected.begin();
				auto in = layer_input.begin();
				for (auto weight = (*neuron)->begin(); weight != (*neuron)->end(); ++weight, ++in, ++ex )
				{
					// calculate the deltas of the weights
					double delta = rate * ((*ex) - (*in)) * actf.dydx((*out)) * (*in);
					(*weight) -= delta; 

				}
			}

			// propogate the expected value down the chain by 
			// recalculating the layer's output with the new weights
			expected = (*layer)->feedForward( layer_input );
		}
	}

	auto backPropogation = trainingFunctionFactory( _backPropogation );
}