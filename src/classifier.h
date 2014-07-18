#ifndef CLASSIFIER_H
#define CLASSIFIER_H	

#include <typeinfo>
#include <vector>

namespace machine {

	template<class T> class Classifier
	{
	public:
		Classifier( Network& net )
		{

		}

		std::vector<double> softmax ( std::vector<double> )
		{
						
		}

		// softmax = function ( vec ) {
		// var sum = vec.reduce(function ( prev, cur ) {
		// 	return prev + Math.exp(cur);
		// },0);

		// return vec.map( function ( val, i ) {
		// 	return Math.exp( val ) / sum;
		// });
		
	};
}

#endif