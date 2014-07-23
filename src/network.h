/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 							Artificial Neural Network Base Classes
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * by jonbrennecke / https://github.com/jonbrennecke
 *
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <memory>
#include <functional>
#include <typeinfo>
#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>
#include <iterator>
#include <fstream>

namespace machine {

	// forward declare our classes
	class Network;
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
		act_handle _dxdy;
		act_handle _dydx;

		double dxdy ( double x ) const
		{
			return (*this->_dxdy)(x);
		}

		double dydx ( double y ) const
		{
			return (*this->_dydx)(y);
		}
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
	 * 			Training functions
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Training functions used to train the neural network
	 *
	 * training functions take 3 parameters:
	 * :param input { std::vector<double> } - input to the network (i.e., input to feedForward)
	 * :param expected { std::vector<double> } - expected output of the network (i.e., output of feedForward)
	 * :param network { Network& } - the network to train
	 */

	typedef std::shared_ptr<std::function<void(std::vector<double>, std::vector<double>, Network&)> > train_handle;

	// factory function to create type 'train_handle' function pointers
	template <class F>
	train_handle trainingFunctionFactory(F f) {
	    return train_handle( new std::function<void(std::vector<double>, std::vector<double>, Network&)>(f) );
	}

	// the default training function
	extern train_handle backPropogation;


	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 									Neural Network
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * by default, the Network class creates a multilayer feed-forward back-propogating network
	 * with one hidden layer
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
		 * 					Layer
		 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * A layer of n 'neurons'
		 *
		 */
		class Layer
		{
		public:

			/**
			 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			 *					Neuron
			 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			 *
			 * right now, this could probably just be a vector within Layer, instead of a seperate class
			 * but we'll probably need it to be its own class later on when we get to more complicated networks
			 * so it exists for now.
			 */
			class Neuron
			{
			public:
				Neuron ( int, Network& );
				~Neuron();
				std::vector<double>::iterator begin();
				std::vector<double>::iterator end();

			private:
				friend class Network;
				friend class Layer;
				Network &parent;
				int nWeights;
				std::vector<double> weights;
			};

			/**
			 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			 * 					Neuron Iterator
			 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			 *
			 * provides an STL-like iterater over the Neurons in a Layer
			 *
			 * see
			 *		http://stackoverflow.com/questions/8054273/how-to-implement-an-stl-style-iterator-and-avoid-common-pitfalls/8054856#8054856
			 *		https://gist.github.com/jeetsukumaran/307264#file-custom_iterator-cpp-L50
			 */
			class iterator
			{
			public:
				typedef iterator self_type;
				typedef Neuron value_type;
				typedef Neuron& reference;
				typedef Neuron* pointer;
				typedef std::forward_iterator_tag iterator_category;
				typedef int difference_type;

				iterator( std::vector<pointer>::iterator );
				~iterator();
				
				self_type operator++(); 
				self_type operator++( int i );
				reference operator*();
				pointer operator->();
				bool operator==(const self_type& rhs);
				bool operator!=(const self_type& rhs);

			private:
				std::vector<pointer>::iterator it_;
			};

			/**
			 * :param nNeurons - dimension of the layer; eg. number of 'neurons'
			 * :param nWeights - length of the layer; eg. dimension of the weight vector of each of n 'neurons'
			 */
			Layer ( int, int, Network&, int );
			~Layer();
			std::vector<double> getInput();
			std::vector<double> getOutput();
			std::vector<double> feedForward( std::vector<double> );
			Layer::iterator begin();
			Layer::iterator end();
			int size() const;
			int index;

			// stream operators for serializing the layer
			friend std::ostream& operator<<( std::ostream&, const Layer& );
			friend std::istream& operator>>( std::istream&, Layer& );
		
		private:
			friend class Network;
			Network &parent;
			int nNeurons;
			int nWeights;
			std::vector<Layer::Neuron *> neurons;
			std::vector<double> input;	
			std::vector<double> output;	
		
		}; // end class Layer

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
		 * :param hiddenSize - size of each hidden layer (default is the floor of the mean of the input and output layer's size)
		 * :param actf - activation function (default is the sigmoid function)
		 * :param initf - initialization function (default is random)
		 * :param trainf - training function (default is backPropogation)
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
			train_handle trainf;

		public:
			Parameters();
			~Parameters();
			Parameters& inputs ( int );
			Parameters& outputs ( int );
			Parameters& hiddenLayers ( int );
			Parameters& hiddenSize ( int );
			Parameters& rate ( double );
			Parameters& biasTerm ( bool );
			Parameters& activation ( ActFunction );
			Parameters& initialization ( init_handle );
			Parameters& propogation ( prop_handle );
			Parameters& training ( train_handle );

		}; // end class Parameters

		/**
		 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 * 					Layer Iterators
		 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		 *
		 * provides a few STL-like iteraters over the Layers, except that we don't need templates here since we know 
		 * exactly what we're iterating over.
		 *
		 * see
		 *		http://stackoverflow.com/questions/8054273/how-to-implement-an-stl-style-iterator-and-avoid-common-pitfalls/8054856#8054856
		 *		https://gist.github.com/jeetsukumaran/307264#file-custom_iterator-cpp-L50
		 */
		struct iterator
		{
			typedef iterator self_type;
			typedef Layer value_type;
			typedef Layer& reference;
			typedef Layer* pointer;
			typedef std::forward_iterator_tag iterator_category;
			typedef int difference_type;

			iterator( std::vector<pointer>::iterator );
			~iterator();
			
			self_type operator++(); 
			self_type operator++( int i );
			reference operator*();
			pointer operator->();
			bool operator==(const self_type& rhs);
			bool operator!=(const self_type& rhs);

		private:
			std::vector<pointer>::iterator it_;
		};

		// struct const_iterator
  //       {
		// 	typedef const_iterator self_type;
		// 	typedef T value_type;
		// 	typedef T& reference;
		// 	typedef T* pointer;
		// 	typedef int difference_type;
		// 	typedef std::forward_iterator_tag iterator_category;
		// 	const_iterator(pointer ptr) : ptr_(ptr) { }
		// 	self_type operator++() { self_type i = *this; ptr_++; return i; }
		// 	self_type operator++(int junk) { ptr_++; return *this; }
		// 	const reference operator*() { return *ptr_; }
		// 	const pointer operator->() { return ptr_; }
		// 	bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
		// 	bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
		// private:
		// 	pointer ptr_;
		// };

		Network ( const Parameters& );
		Network ( std::string, const Parameters& );
		~Network();

		std::vector<double> feedForward ( std::vector<double> );
		double propogate ( std::vector<double>, std::vector<double> );
		const ActFunction& activate ();
		void train ( std::vector<double>, std::vector<double> );
		void toggleTrainingMode();
		double init ();
		int size () const;
		double rate ();
		void save ( std::string );
		void load ( std::string );

		// stream operators
		friend std::ostream& operator<<( std::ostream&, const Network& );
		friend std::istream& operator>>( std::istream&, Network& );

		// iterator methods
		iterator begin();
		iterator end();
		std::vector<Layer*>::reverse_iterator rbegin();
		std::vector<Layer*>::reverse_iterator rend();

	private:
		friend class Layer;
		friend class Trainer;
		
		const Parameters& params;
		std::vector<Layer*> layers; // TODO this should be Layer&
		train_handle trainf;
		bool training;

	}; // end class Network
}

#endif