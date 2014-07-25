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

// this is just an arbitrary hash that we'll use this to verify our classes
#define CLASS_HANDLE_SIGNATURE 0xFF00F0A5

namespace mex {

	template<class T> class Base;
	template<class T> class Handle;

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

		inline operator int() { return round((double)(*this)); }

		inline operator std::string() { return std::string((char *)(*this)); }

		inline explicit operator char*() { return mxArrayToString(ptr_); }

		inline explicit operator double() { return mxGetScalar(ptr_); }

	private:
		mxArray* ptr_;
	};

	/**
	 *
	 * marshall an mxArray* into a vector of a template type
	 *
	 */
	template<class T> std::vector<T> mex2vector( const mxArray* mx )
	{
		T* array = (T*)mxGetData(mx);
		size_t size = mxGetNumberOfElements(mx);

		std::vector<T> vec(size);

		for (size_t i = 0; i < size; ++i)
			vec[i] = array[i];

		return vec;
	}

	/**
	 *
	 * marshall a vector of a template type into an mxArray*
	 *
	 */
	template<class T> mxArray* vector2mex( const std::vector<T> vec  )
	{
		mxArray * mx = mxCreateDoubleMatrix( vec.size(), 1, mxREAL );
		double *mxptr = mxGetPr(mx);

		for( auto it=vec.begin(); it!=vec.end(); ++it )
			mxptr[ it - vec.begin() ] = (T)*it;

		return mx;
	}

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
		Base ( T *ptr ) : name_( typeid(T).name() ), ptr_(ptr)
		{ 
			signature_ = CLASS_HANDLE_SIGNATURE; 
		}

		~Base() { delete ptr_; }

		bool isValid() { 
			return ((signature_ == CLASS_HANDLE_SIGNATURE) && !strcmp(name_.c_str(), typeid(T).name())); 
		}
	    
	private:
		template<class U> friend class Handle;
		uint32_t signature_;
		std::string name_;
		T *ptr_;
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
		Base<T> *base_;

	public:

		Handle ( T* tptr ) { base_ = new Base<T>(tptr); }

		// constructor from T*
		Handle ( const mxArray* mx )
		{
			if (mxGetNumberOfElements(mx) != 1 || mxGetClassID(mx) != mxUINT64_CLASS || mxIsComplex(mx))
				mexErrMsgTxt("Input must be a real uint64 scalar.");
			base_ = reinterpret_cast<Base<T> *>(*((uint64_t *)mxGetData(mx)));
			if (!base_->isValid())
				mexErrMsgTxt("Handle not valid.");
		}

		~Handle() {
			// mexUnlock();
		}

		// conversion operators

		inline operator mxArray*() 
		{
			mxArray *out = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
			*((uint64_t *)mxGetData(out)) = reinterpret_cast<uint64_t>(base_);
			return out;
		}

		inline operator T* () {  return base_->ptr_; }
	};

}

#endif