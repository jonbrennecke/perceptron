
#include <typeinfo>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>

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
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 *			 Back Propogation
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 				Parameters
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	Network::Parameters::Parameters() 
		: __inputs(3), __outputs(5), __hiddenLayers(1), __hiddenSize(0), actf(sigmoid), initf(random), __biasTerm(true), __rate(0.001) {}
	
	Network::Parameters::~Parameters() {}

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

	Network::Parameters& Network::Parameters::training ( train_handle trainf )
	{
		this->trainf = trainf;
		return *this;
	}

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 					Network
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	Network::Network ( const Network::Parameters& params ) : params(params), training(false)
	{
		// initialize the trainer
		this->trainer = new Network::Trainer( *this, this->params.trainf );

		// initialize the layers
		this->layers = std::vector<Network::Layer*>( this->params.__hiddenLayers + 2 );

		// for all but the input layer, the size of the weight vector is equal 
		// to the number of neurons in the previous layer
		for (auto it = this->layers.begin(); it != this->layers.end(); ++it)
		{
			int nNeurons, nWeights;

			if ( it == this->layers.begin() ) // input layer
			{
				nNeurons = this->params.__inputs;
				nWeights = this->params.__inputs;
			} 
			else if ( it+1 == this->layers.end() ) // output layer
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
			*it = new Layer( nNeurons, nWeights, *this, it - this->layers.begin() );
		}
	
	}

	Network::~Network(){};

	/**
	 * Feed-Forward algorithm
	 * ---
	 * http://en.wikipedia.org/wiki/Feedforward_neural_network
	 */
	std::vector<double> Network::feedForward ( std::vector<double> feed )
	{
		// iterate through the layers, transforming the input vector by the neurons in each layer
		for (auto it = layers.begin(); it != layers.end(); ++it)
			feed = (*it)->feedForward( feed );

		return feed;
	}

	// call the propogation function
	double Network::propogate ( std::vector<double> a, std::vector<double> b )
	{
		return (*this->params.propf)(a,b);
	}

	// call the initialization function
	double Network::init ()
	{
		return (*this->params.initf)();
	}

	// call the training method of the trainer class
	void Network::train ( std::vector<double> input, std::vector<double> expected )
	{
		this->trainer->train( input, expected );
	}

	// return the activation function
	const ActFunction& Network::activate ()
	{
		return this->params.actf;
	}

	// return the number of layers in the network
	int Network::size ()
	{
		return this->layers.size();
	}

	// return the learning rate
	double Network::rate ()
	{
		return this->params.__rate;
	}

	// toggle the training bool
	void Network::toggleTrainingMode ()
	{
		this->training = ~this->training;
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// some iterator methods... 
	// which are really just calling the iterator methods of the layer vector
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	std::vector<Network::Layer*>::iterator Network::begin()
	{
		return this->layers.begin();
	}

	std::vector<Network::Layer*>::iterator Network::end()
	{
		return this->layers.end();
	}

	std::vector<Network::Layer*>::reverse_iterator Network::rbegin()
	{
		return this->layers.rbegin();
	}

	std::vector<Network::Layer*>::reverse_iterator Network::rend()
	{
		return this->layers.rend();
	}


	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 					Layer
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	Network::Layer::Layer ( int nNeurons, int nWeights, Network &parent, int index ) 
		: nNeurons(nNeurons), nWeights(nWeights), parent(parent), index(index)
	{
		this->neurons = std::vector<Layer::Neuron *>(nNeurons);

		for (auto it = neurons.begin(); it != neurons.end(); ++it)
			(*it) = new Layer::Neuron( nWeights, parent );
	}

	Network::Layer::~Layer(){}

	/**
	 * feed 'input' to the layer and return a the resulting vector
	 *
	 * for each neuron in the layer, the propogation function is called on the neuron's
	 * weight vector and the input vector, the (scalar) result of each propogation is passed 
	 * to the activation function, and the result of that transformation is stored in the output vector
	 */
	std::vector<double> Network::Layer::feedForward ( std::vector<double> input )
	{
		std::vector<double> output(this->neurons.size());

		auto it2 = output.begin();

		for (auto it = this->neurons.begin(); it != this->neurons.end(); ++it, ++it2 )
			(*it2) = this->parent.activate().dxdy( this->parent.propogate( input, (*it)->weights ) );

		// when the network is in 'training mode' the input and output to each neuron should be stored
		if ( this->parent.training ) {
			this->input = std::vector<double>(input);
			this->output = std::vector<double>(output);
		}

		return output;
	}

	// get the input vector
	std::vector<double> Network::Layer::getInput()
	{
		return this->input;
	}

	// get the output vector
	std::vector<double> Network::Layer::getOutput()
	{
		return this->output;
	}

	// get the size of the layer
	int Network::Layer::size()
	{
		return this->neurons.size();
	}

	std::vector<Network::Layer::Neuron*>::iterator Network::Layer::begin()
	{
		return this->neurons.begin();
	}

	std::vector<Network::Layer::Neuron*>::iterator Network::Layer::end()
	{
		return this->neurons.end();
	}

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 					Neuron
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	Network::Layer::Neuron::Neuron ( int nWeights, Network &parent ) 
		: nWeights(nWeights), parent(parent)
	{
		this->weights = std::vector<double>(nWeights);

		for (auto it = this->weights.begin(); it != this->weights.end(); ++it)
			(*it) = parent.init();
	}

	Network::Layer::Neuron::~Neuron(){}

	std::vector<double>::iterator Network::Layer::Neuron::begin()
	{
		return this->weights.begin();
	}

	std::vector<double>::iterator Network::Layer::Neuron::end()
	{
		return this->weights.end();
	}

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 					Trainer
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	Network::Trainer::Trainer ( Network& net, train_handle trainf ) : network(net), trainf(trainf) {} 

	Network::Trainer::~Trainer(){}

	// call the training function
	void Network::Trainer::train ( std::vector<double> input, std::vector<double> expected )
	{
		// when the network is in 'training mode' the input to each neuron will be stored
		if ( !this->network.training )
			this->network.toggleTrainingMode();

		(*this->trainf)( input, expected, this->network );
	}

}