#ifndef NETWORK_H
#define NETWORK_H

#include <typeinfo>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <random>

namespace machine {

	// predeclare these for later
	template <typename T> class Network;
	template <typename T> class Layer;
	class Neuron;

	// define 'act' as a shared pointer to an activation function
	typedef std::shared_ptr<std::function<double(double)> > act;

	// factory method to create type 'act' pointers from lambdas
	template <class F>
	act createActivationFunction(F f) {
	    return act( new std::function<double(double)>(f) );
	}

	/**
	 * the activation function is a mathematical function used to approximate the influence of 
	 * the extracellular field on the modulation of a neuron
	 *
	 * the ActFunction data structure implements both the function (as dxdy) and it's first
	 * derivative (as dydx)
	 *
	 */
	struct ActFunction
	{
		act dxdy;
		act dydx;
	};

	/**
	 *
	 * Let's define some commonly used activation functions
	 *
	 */

	// see http://en.wikipedia.org/wiki/Sigmoid_function
	ActFunction sigmoid { 
		createActivationFunction([]( double x ) {
			return 1 / ( 1 + exp(-x) );
		}), 
		createActivationFunction([]( double y ) {
			return y * (1 - y);
		})
	};

	// see http://en.wikipedia.org/wiki/Sigmoid_function
	ActFunction softplus {
		createActivationFunction([]( double x ) {
			return log10( 1 + exp(x) );
		}), 
		createActivationFunction([]( double y ) {
			return 1 / (1 + exp(-y));
		})
	};

	// see http://en.wikipedia.org/wiki/Hyperbolic_tangent
	ActFunction hyperbolic_tan {
		createActivationFunction([]( double x ) {
			return tanh(x);
		}), 

		// d(tanh)/dy = sech^2(y)
		createActivationFunction([]( double y ) {
			return pow( (2 * exp(-y)) / ( 1 + exp(-2 * y)), 2);
		})
	};


	/**
	 *
	 * Initialization functions
	 *
	 */

	std::default_random_engine rng;
	std::uniform_int_distribution<int> dist(0,1);
	auto r = std::bind( dist, rng );

	typedef std::shared_ptr<std::function<double()> >

	// auto random = std::shared_ptr<std::function<double()> >( new std::function<double()>(r) );





	/**
	 *
	 * Neural Network Controller
	 * ---
	 * by default, the Network class creates a multilayer feed-forward back-propogating network
	 * with one hidden layer
	 *
	 *
	 * see 	- http://en.wikipedia.org/wiki/Neural_network
	 * 		- http://en.wikipedia.org/wiki/Deep_learning#Deep_neural_networks
	 *		- http://stats.stackexchange.com/questions/181/how-to-choose-the-number-of-hidden-layers-and-nodes-in-a-feedforward-neural-netw
	 *		- http://www.faqs.org/faqs/ai-faq/neural-nets/part1/preamble.html
	 *
	 */
	template <typename T> class Network 
	{
	private:

		// sigmoid is the default activation function
		ActFunction activation = sigmoid;

	public:

		/**
		 *
		 * :param inputs - size of the input vector
		 * :param outputs - size of the output vector
		 */
		Network ( int inputs, int outputs )
		{
			std::vector<Layer<T> *> layers;

			layers.push_back( new Layer<T>( inputs, 2 ) );
		}

		/**
		 * set the activation function
		 */
		void setActivation ( ActFunction f  )
		{
			this->activation = f;
		}

		void setInitialization ( double (*init)() )
		{

		}

	};


	/**
	 *
	 * A layer of n 'neurons'
	 *
	 */
	class Neuron
	{
	public:
		Neuron ( int nWeights )
		{
			std::vector<double> weights(nWeights);

			for (auto i = weights.begin(); i != weights.end(); ++i)
			{
				// (*i) = 
			}
		}
		
	};


	/**
	 *
	 * A layer of n 'neurons'
	 *
	 */
	template <typename T> class Layer
	{
	public:

		/**
		 * :param nNeurons - dimension of the layer; eg. number of 'neurons'
		 * :param nWeights - length of the layer; eg. dimension of the weight vector of each of n 'neurons'
		 */
		Layer ( int nNeurons, int nWeights )
		{
			std::vector<Neuron *> neurons(nNeurons);

			for (auto i = neurons.begin(); i != neurons.end(); ++i)
				(*i) = new Neuron(nWeights);
		}
		
	};
}

#endif