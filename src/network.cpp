#include <typeinfo>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>

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
	std::default_random_engine rng;
	std::uniform_int_distribution<int> dist(0,1);
	auto random = initFunctionFactory( std::bind( dist, rng ) );

	/**
	* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	* 			Propogation functions
	* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/

	// return the dot product of two vectors
	double __dotprod ( std::vector<double> a, std::vector<double> b )
	{
		// if ( a.size() != b.size() )
			// throw new Exception()

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
	 * 				Parameters
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	Network::Parameters::Parameters() 
		: __inputs(3), __outputs(5), __hiddenLayers(1), __hiddenSize(0), actf(sigmoid), initf(random), __biasTerm(true), __rate(0.001) {}
			
	Network::Parameters& Network::Parameters::inputs ( int n )
	{
		this->__inputs = n;
		return *this;
	}

	Network::Parameters& Network::Parameters::outputs ( int n )
	{
		this->__outputs = n;
		return *this;
	}

	Network::Parameters& Network::Parameters::hiddenLayers ( int n )
	{
		this->__hiddenLayers = n;
		return *this;
	}

	Network::Parameters& Network::Parameters::hiddenSize ( int n )
	{
		this->__hiddenSize = n;
		return *this;
	}

	Network::Parameters& Network::Parameters::rate ( double n )
	{
		this->__rate = n;
		return *this;
	}

	Network::Parameters& Network::Parameters::biasTerm ( bool b )
	{
		this->__biasTerm = b;
		return *this;
	}

	Network::Parameters& Network::Parameters::activation ( ActFunction actf )
	{
		this->actf = actf;
		return *this;
	}

	Network::Parameters& Network::Parameters::initialization ( init_handle initf )
	{
		this->initf = initf;
		return *this;
	}

	Network::Parameters& Network::Parameters::propogation ( prop_handle propf )
	{
		this->propf = propf;
		return *this;
	}

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 					Network
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	Network::Network ( const Network::Parameters& params ) : params(params)
	{
		// initialize the layers
		std::vector<Layer*> layers( this->params.__hiddenLayers + 2 );

		// for all but the input layer, the size of the weight vector is equal 
		// to the number of neurons in the previous layer
		for (auto it = layers.begin(); it != layers.end(); ++it)
		{
			int nNeurons, nWeights;

			if ( *(it-1) == nullptr ) // input layer
			{
				nNeurons = this->params.__inputs;
				nWeights = this->params.__inputs;
			} 
			else if ( it+1 == layers.end() ) // output layer
			{
				nNeurons = this->params.__outputs;
				nWeights = (*(it-1))->nNeurons;
			}
			else // hidden layer
			{
				// if the hiddenSize hasn't been set by the user, it should be set automatically.
				// by default, the hidden size is equal to the floor of the mean of the number
				// of inputs and outputs
				if ( this->params.__hiddenSize )
					nNeurons = this->params.__hiddenSize;
				else
					nNeurons = (unsigned int)(( this->params.__inputs + this->params.__outputs) * 0.5);

				nWeights = (*(it-1))->nNeurons;
			}

			// create the layer
			*it = new Layer( nNeurons, nWeights, *this, it - layers.begin() );
		}
	
	}

	double Network::init ()
	{
		return (*this->params.initf)();
	}

	// ActFunction* Network::activation ()
	// {
	// 	return *(this->params.actf);
	// }

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 					Layer
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	Layer::Layer ( int nNeurons, int nWeights, Network &parent, int index ) 
		: nNeurons(nNeurons), nWeights(nWeights), parent(parent), index(index)
	{
		// this->nWeights == (prev) ? parent.nInputs() : prev.nNeurons;
		// this->nNeurons = ( prev != nullptr ) ? parent.params.__inputs : parent.hiddenSize;

		std::cout << this->nNeurons << " " << this->nWeights << std::endl;

		// for (auto i = neurons.begin(); i != neurons.end(); ++i)
		// 	(*i) = new Neuron<T>( nWeights, parent );
	}

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 					Neuron
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	Neuron::Neuron ( int nWeights, Network *parent )
	{
		this->parent = parent;

		std::vector<double> weights(nWeights);

		// for (auto i = weights.begin(); i != weights.end(); ++i)
		// 	(*i) = parent->init();
	}

}