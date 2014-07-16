#ifndef ACTIVATION_H
#define ACTIVATION_H

namespace machine {

	typedef double (*act)(double);

	class ActivationFunction
	{
	private:
		act _dxdy;
		act _dydx;

	public:
		double dxdy ( double x )
		{
			return this->_dxdy( x );
		}

		double dydx ( double y )
		{
			return this->_dydx( y );
		}

		void setDXDY ( act f )
		{
			this->_dxdy = *f;
		}

		void setDYDX ( act f )
		{
			this->_dydx = *f;
		} 
	};
}

#endif