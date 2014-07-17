#ifndef NETWORK_H
#define NETWORK_H

#include <typeinfo>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <random>


namespace machine {

	// forward declare our classes
	class Network;
	class Layer;
	class Neuron;
	struct ActFunction;

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 			Activation functions
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * the activation function is a mathematical function used to approximate the influence of 
	 * the extracellular field on the modulation of a neuron
	 *
	 */

 	// forward declare some activation functions that will be defined in 'network.cpp'
	extern ActFunction sigmoid;
	extern ActFunction softplus;
	extern ActFunction hyperbolic_tan;

	// define 'act' as a shared pointer to an activation function
	typedef std::shared_ptr<std::function<double(double)> > act_handle;

	// factory function to create type 'act_handle' pointers
	template <class F>
	act_handle activationFunctionFactory(F f) {
	    return act_handle( new std::function<double(double)>(f) );
	}

	/**
	 * ActFunction implements both the activation function (as dxdy) and it's first
	 * derivative (as dydx)
	 */
	struct ActFunction
	{
		act_handle dxdy;
		act_handle dydx;
	};


	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 		Initialization functions
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * initialization functions are used to define the weights of the neurons
	 * the initialization function is called once per weight
	 *
	 */

	typedef std::shared_ptr<std::function<double()> > init_handle;

	// factory function to create type 'init_handle' pointers
	template <class F>
	init_handle initFunctionFactory(F f) {
	    return init_handle( new std::function<double()>(f) );
	}

	// the default initialization function returns random numbers
	// defined in 'network.cpp'
	extern init_handle random;

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 			Propogation functions
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * propogation functions define how the the input vectors on each neuron is affected 
	 * by it's weight vector. The result of the propogation function is passed to the 
	 * activation function, which determine's the output of the neuron.
	 *
	 */

	typedef std::shared_ptr<std::function<double( std::vector<double>, std::vector<double> )> > prop_handle;

	// factory function to create type 'prop_handle' pointers
	template <class F>
	prop_handle propFunctionFactory(F f) {
	    return prop_handle( new std::function<double( std::vector<double>, std::vector<double> )>(f) );
	}

	// the default propogation function is the dot product of the input vector and the neuron's weight vector
	extern prop_handle dotprod;

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 					Layer
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * A layer of n 'neurons'
	 *
	 */
	class Layer
	{
	private:
		friend class Network;
		friend class Neuron;
		Network &parent;
		Layer* prev;
		int index;
		int nNeurons;
		int nWeights;
		std::vector<Neuron *> neurons;

		// TODO
		// http://stackoverflow.com/questions/8054273/how-to-implement-an-stl-style-iterator-and-avoid-common-pitfalls/8054856#8054856
		// template <typename T> class iterator
		// {
		// public:
		// 	iterator( const iterator& );
		// 	~iterator();
		// 	iterator& operator++();
		// 	iterator& operator=( const iterator& );
		// 	reference operator*() const;
		// 	friend void swap(iterator& lhs, iterator& rhs);
		// };

	public:

		/**
		 * :param nNeurons - dimension of the layer; eg. number of 'neurons'
		 * :param nWeights - length of the layer; eg. dimension of the weight vector of each of n 'neurons'
		 */
		 
		Layer ( int, int, Network&, int );
		
	};


	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 				Neural Network
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
	class Network 
	{
	public:

		/**
		 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * 					Parameters
		 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 *
		 * The Parameters class makes passing lots of parameters to a Network easier
		 *
		 * :param nInputs - size of the input vector
		 * :param nOutputs - size of the output vector
		 * :param nHiddenLayers - number of hidden layers
		 * :param actf - activation function (default is the sigmoid function)
		 * :param initf - initialization function (default is random)
		 */
		class Parameters
		{
		private:
			friend class Network;
			friend class Layer;
			unsigned int __inputs;
			unsigned int __outputs;
			unsigned int __hiddenLayers;
			unsigned int __hiddenSize;
			bool __biasTerm;
			double __rate;
			ActFunction actf;
			init_handle initf;
			prop_handle propf;

		public:
			Parameters();
			Parameters& inputs ( int );
			Parameters& outputs ( int );
			Parameters& hiddenLayers ( int );
			Parameters& hiddenSize ( int );
			Parameters& rate ( double );
			Parameters& biasTerm ( bool );
			Parameters& activation ( ActFunction );
			Parameters& initialization ( init_handle );
			Parameters& propogation ( prop_handle );

		}; // end class Parameters

		Network ( const Parameters& );
		double init ();

	private:
		friend class Layer;
		const Parameters& params;
		unsigned int hiddenSize;
	};


	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 *					Neuron
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	class Neuron
	{
	private:
		friend class Network;
		friend class Layer;
		Network *parent;

	public:
		Neuron ( int, Network* );
		
	};
}

#endif