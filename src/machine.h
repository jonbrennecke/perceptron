
#ifndef MACHINE_H
#define MACHINE_H

#include <string>

namespace machine {

	// abstract base class for serializable objects
	class Serializable
	{
	public:
		virtual void save ( std::string ) = 0;
	};

}

#endif