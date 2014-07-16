#include "activation.h"

namespace machine {

	// auto sigmoid = new ActivationFunction();
	// auto softplus = new ActivationFunction();
	// auto tanh = new ActivationFunction();
	
	// sigmoid->setDXDY([]( double x ) {
	// 	return 1 / ( 1 + exp(-x) );
	// });



}

// // sigmoid function
	// // see http://en.wikipedia.org/wiki/Sigmoid_function
	// struct sigmoid
	// {
	// 	double dxdy ( double x )
	// 	{
	// 		return 1 / ( 1 + exp(-x) );
	// 	}

	// 	double dydx ( double y )
	// 	{
	// 		return y * (1 - y);
	// 	}
	// };

	// // softplus function
	// // see http://en.wikipedia.org/wiki/Rectifier_(neural_networks)
	// struct softplus
	// {
	// 	double dxdy ( double x )
	// 	{
	// 		return log10( 1 + exp(x) );
	// 	}

	// 	double dydx ( double y )
	// 	{
	// 		return 1 / (1 + exp(-y));
	// 	}
	// };

	// // hyperbolic tangent
	// // see http://en.wikipedia.org/wiki/Hyperbolic_tangent
	// struct _tanh
	// {
	// 	double dxdy ( double x ) 
	// 	{
	// 		return tanh( x );
	// 	}

	// 	// d(tanh)/dy = sech^2(y)
	// 	double dydx ( double y ) 
	// 	{
	// 		return pow( (2 * exp(-y)) / ( 1 + exp(-2 * y)), 2);
	// 	}
	// };