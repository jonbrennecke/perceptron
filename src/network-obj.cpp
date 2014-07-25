/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 								Network Objects
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Implementations for the classes defined in 'network.h'
 *
 * by jonbrennecke / https://github.com/jonbrennecke
 *
 */


#include <typeinfo>
#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>
#include <fstream>
#include "network.h"

namespace machine {

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 				Parameters
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	Network::Parameters::Parameters() 
		: __inputs(3), __outputs(5), __hiddenLayers(1), __hiddenSize(4), __biasTerm(true), __rate(0.001), actf(sigmoid), initf(random), propf(dotprod), trainf(backPropogation) {}
	
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
	 *
	 * construct a network from a Parameters object
	 *
	 */
	Network::Network ( const Network::Parameters* params ) : params(params), training(false)
	{
		// initialize the layers
		this->layers = std::vector<Network::Layer*>( this->params->__hiddenLayers + 2 );

		// for all but the input layer, the size of the weight vector is equal 
		// to the number of neurons in the previous layer
		for (auto it = this->layers.begin(); it != this->layers.end(); ++it)
		{
			int nNeurons, nWeights;

			if ( it == this->layers.begin() ) // input layer
			{
				nNeurons = this->params->__inputs;
				nWeights = this->params->__inputs;
			} 
			else if ( it+1 == this->layers.end() ) // output layer
			{
				nNeurons = this->params->__outputs;
				nWeights = (*(it-1))->nNeurons;
			}
			else // hidden layer
			{
				// if the hiddenSize hasn't been set by the user, it should be set automatically.
				// by default, the hidden size is equal to the floor of the mean of the number
				// of inputs and outputs
				if ( this->params->__hiddenSize )
					nNeurons = this->params->__hiddenSize;
				else
					nNeurons = (unsigned int)(( this->params->__inputs + this->params->__outputs) * 0.5);

				nWeights = (*(it-1))->nNeurons;
			}

			// create the layer
			*it = new Layer( nNeurons, nWeights, *this, it - this->layers.begin() );
		}
	
	}

	Network::~Network(){};

	void Network::load ( std::string file )
	{

	}

	// 
	void Network::save ( std::string file )
	{
		std::ofstream out( file, std::ios::out | std::ios::binary );
		if ( out )
		{
			out << *this;
			out.close();
		}
		else
		{
			// throw an error or something
		}
	}

	std::ostream& operator<< ( std::ostream& os, const Network& net )
	{
		// serialize the layers and the parameters
		// for (auto layer = net.begin(); layer != net.end(); ++layer)
		// {
		// 	os << (*layer);
		// }
		// out.write( reinterpret_cast<const char*>(&net.field1), sizeof(net.field1) );

		return os;
	}

	std::istream& operator>> ( std::istream& is, Network& )
	{
		return is;
	}


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
		return (*this->params->propf)(a,b);
	}

	// call the initialization function
	double Network::init ()
	{
		return (*this->params->initf)();
	}

	// call the training method of the trainer class
	void Network::train ( std::vector<double> input, std::vector<double> expected )
	{
		// when the network is in 'training mode' the input to each neuron will be stored
		if ( !this->training )
			this->toggleTrainingMode();

		(*this->trainf)( input, expected, *this );
	}

	// return the activation function
	const ActFunction& Network::activate ()
	{
		return this->params->actf;
	}

	// return the number of layers in the network
	int Network::size () const
	{
		return this->layers.size();
	}

	// return the learning rate
	double Network::rate ()
	{
		return this->params->__rate;
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

	Network::iterator Network::begin()
	{
		return Network::iterator( this->layers.begin() );
	}

	Network::iterator Network::end()
	{
		return Network::iterator( this->layers.end() );
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
	 * 					Network::iterator
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */

	Network::iterator::iterator( std::vector<pointer>::iterator it ) : it_(it) {}
	Network::iterator::~iterator(){}	

	Network::iterator::self_type Network::iterator::operator++() 
	{
		self_type i = *this; it_++; return i;
	}

	Network::iterator::self_type Network::iterator::operator++( int i )
	{
		it_++; return *this;
	}

	Network::iterator::reference Network::iterator::operator*()
	{
		return *(*it_);
	}

	Network::iterator::pointer Network::iterator::operator->()
	{
		return *it_;
	}

	bool Network::iterator::operator==(const Network::iterator::self_type& rhs)
	{
		return it_ == rhs.it_;
	}

	bool Network::iterator::operator!=(const Network::iterator::self_type& rhs)
	{
		return it_ != rhs.it_;
	}


	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 					Layer
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	Network::Layer::Layer ( int nNeurons, int nWeights, Network &parent, int index ) 
		: index(index), parent(parent), nNeurons(nNeurons), nWeights(nWeights)
	{
		this->neurons = std::vector<Layer::Neuron *>(this->nNeurons);

		for (auto it = neurons.begin(); it != neurons.end(); ++it)
			(*it) = new Layer::Neuron( this->nWeights, parent );
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
	int Network::Layer::size() const
	{
		return this->neurons.size();
	}

	Network::Layer::iterator Network::Layer::begin()
	{
		return Network::Layer::iterator( this->neurons.begin() );
	}

	Network::Layer::iterator Network::Layer::end()
	{
		return Network::Layer::iterator( this->neurons.end() );
	}

	std::ostream& operator<<( std::ostream& os, const Network::Layer& layer )
	{
		// serialize the neurons
		// for (auto neuron = layer.begin(); neuron != layer.end(); ++neuron)
		// 	os << (*neuron);

		return os;
	}

	std::istream& operator>>( std::istream& is, Network::Layer& layer )
	{
		return is;
	}

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 					Network::iterator
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */

	Network::Layer::iterator::iterator( std::vector<pointer>::iterator it ) : it_(it) {}
	Network::Layer::iterator::~iterator(){}	

	Network::Layer::iterator::self_type Network::Layer::iterator::operator++() 
	{
		self_type i = *this; it_++; return i;
	}

	Network::Layer::iterator::self_type Network::Layer::iterator::operator++( int i )
	{
		it_++; return *this;
	}

	Network::Layer::iterator::reference Network::Layer::iterator::operator*()
	{
		return *(*it_);
	}

	Network::Layer::iterator::pointer Network::Layer::iterator::operator->()
	{
		return *it_;
	}

	bool Network::Layer::iterator::operator==(const Network::Layer::iterator::self_type& rhs)
	{
		return it_ == rhs.it_;
	}

	bool Network::Layer::iterator::operator!=(const Network::Layer::iterator::self_type& rhs)
	{
		return it_ != rhs.it_;
	}

	/**
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 					Neuron
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	Network::Layer::Neuron::Neuron ( int nWeights, Network &parent ) 
		: parent(parent), nWeights(nWeights)
	{
		this->weights = std::vector<double>(this->nWeights);

		for (auto it = this->weights.begin(); it != this->weights.end(); ++it)
			(*it) = this->parent.init();
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

}