/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   _   _                                  _     _            
 *  | |_| |__   ___    _ __ ___   __ _  ___| |__ (_)_ __   ___ 
 *  | __| '_ \ / _ \  | '_ ` _ \ / _` |/ __| '_ \| | '_ \ / _ \
 *  | |_| | | |  __/  | | | | | | (_| | (__| | | | | | | |  __/
 *   \__|_| |_|\___|  |_| |_| |_|\__,_|\___|_| |_|_|_| |_|\___|
 *                                                          
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * A machine learning experiment by @jonbrennecke / https://github.com/jonbrennecke
 *
 * Released under the MIT license (see the accompanying LICENSE.md)
 */  


#ifndef MEXUTILS_H
#define MEXUTILS_H

#include "mex.h"
#include <typeinfo>

// this is just an arbitrary hash that we'll use this to verify out classes
#define CLASS_HANDLE_SIGNATURE 0xFF00F0A5

namespace mex {

	/**
	 *
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 				Marshal
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * marshal data types from Mex (mxArray*) to native C++ types
	 *
	 */
	struct Marshal
	{
		Marshal ( mxArray* ptr ) : ptr_(ptr) {}

		inline operator int() { return 1; }

		inline operator std::string() { return std::string((char *)(*this)); }

		inline explicit operator char*() { return mxArrayToString(ptr_); }

		inline explicit operator double() { return mxGetScalar(ptr_); }

	private:
		mxArray* ptr_;
	};

	/**
	 *
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 				Base
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Base class signature
	 *
	 */
	template<class T> class Base
	{
	public:
		Base(T *ptr) : name_m( typeid(T).name() ), ptr_m(ptr)
		{ 
			signature_m = CLASS_HANDLE_SIGNATURE; 
		}

		~Base() 
		{ 
			signature_m = 0; 
			delete ptr_m; 
		}

		bool isValid() { 
			return ((signature_m == CLASS_HANDLE_SIGNATURE) && !strcmp(name_m.c_str(), typeid(T).name())); 
		}

		T *ptr() {
			return ptr_m; 
		}
	    
	private:
		uint32_t signature_m;
		std::string name_m;
		T *ptr_m;
	};

	/**
	 *
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * 				Handle
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * Persistent handle to a C++ class instance 
	 *
	 * "Handle" lets us convert back and forth between pointers to C++ data types 
	 * and mxArray's that we can pass back to matlab.
	 *
	 */
	template<class T> class Handle
	{
	private:
		mxArray* mxptr_;

	public:

		// constructor from T*
		Handle( T* cptr )
		{
			mexLock();
			mxptr_ = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
			*((uint64_t *)mxGetData(mxptr_)) = reinterpret_cast<uint64_t>(new Base<T>(cptr));
		}

		// constructor from mxArray*
		Handle( mxArray* mxptr ) : mxptr_(mxptr) {}

		~Handle()
		{
			// invoke the destructor of the C++ class
			delete (T*)(*this);
			mexUnlock();
		}

		// conversion operators

		inline operator mxArray*() { return mxptr_; }

		inline operator Base<T>*() 
		{
			if (mxGetNumberOfElements(mxptr_) != 1 || mxGetClassID(mxptr_) != mxUINT64_CLASS || mxIsComplex(mxptr_))
			    mexErrMsgTxt("Input must be a real uint64 scalar.");
			Base<T> *ptr = reinterpret_cast<Base<T> *>(*((uint64_t *)mxGetData(mxptr_)));
			if (!ptr->isValid())
			    mexErrMsgTxt("Base not valid.");
			return ptr;
		}

		inline operator T*() { return ((Base<T>*)(*this))->ptr(); }
		
	};

}

#endif