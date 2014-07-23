
#ifndef MACHINE_H
#define MACHINE_H

#include <string>

namespace machine {

	/**
	 *
	 *
	 *  Abstract classes
	 *
	 */

	// abstract base class for serializable objects
	class Serializable
	{
	public:
		virtual void save ( std::string ) = 0;
	};

	// /**
	//  *
	//  * construct a default Neural Network
	//  *
	//  */
	// class Machine
	// {
	// public:
	// 	Machine();
	// 	~Machine();
		
	// };

}

#endif