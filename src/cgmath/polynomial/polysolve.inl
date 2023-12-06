#pragma once

#include <cmath>
#include <limits>
#include "../common/functions.inl"

#include "Polynomial.h"
#include "polyeval.inl"
#include "derivative.inl"

#include "../mat/operators.inl"

////////////////////////////////////////////////////////////////////////////////
template<typename T>
void polysolve(Polynomial<T,3>& p)
{
	const T &a = p.coefs[2], &b = p.coefs[1], &c = p.coefs[0];
	const T discriminant = b*b-4*a*c;
	if(discriminant <= 0)
		p.roots[1] = p.roots[0] = std::numeric_limits<T>::quiet_NaN();
	else{
		const T delta = b+sign(b)*sqrt(discriminant);
		p.roots[0] = -2*c/delta;
		p.roots[1] = delta/(-2*a);
	}
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
void polysolve(Polynomial<Mat<T,r,c>,3>& pv)
{
	static const uint n = r*c;
	Polynomial<T,3> p;
	for(uint i=0; i<n; ++i)
	{
		for(uint j=0; j<pv.nCoefs; ++j)
			p.coefs[j] = pv.coefs[i][j];
		polysolve(p);
		for(uint j=0; j<pv.nCoefs; ++j)
			pv.roots[i][j] = p.coefs[j];
	}
}
////////////////////////////////////////////////////////////////////////////////
// http://www.cemyuksel.com/research/polynomials/polynomial_roots_hpg2022_supplemental.pdf
template<typename T,uint N>
const T polysolveInterval(T x0,T x1,const Polynomial<T,N>& p,const Polynomial<T,N-1>& pd,const T tol = 5e-5,const uint iterations = 32)
{
	T y0 = polyeval(x0,p), y1 = polyeval(x1,p);
	if(!(y0*y1 < 0)) return std::numeric_limits<T>::quiet_NaN();

	T xr = (x0+x1)*0.5;

	if(x1-x0 < tol) return xr;

	if constexpr (N == 4)
	{
		for(uint i=0; i<iterations; ++i)
		{
			T xn = xr - polyeval(xr,p)/polyeval(xr,pd);
			xn = clamp(xn,x0,x1);
			if(abs(xr-xn) <= tol) return xn;
			xr = xn;
		}
		if(std::isnan(xr))
			xr = (x0+x1)*0.5;
	}

	T yr = polyeval(xr,p);
	for(uint i=0; i<iterations; ++i)
	{
		if(y0*yr > 0) x0 = xr;
		else          x1 = xr;
		
		T xn = xr-yr/polyeval(xr,pd);
		
		if(x0 < xn && xn < x1)
		{
			if(abs(xr-xn) > tol)
			{
				xr = xn;
				yr = polyeval(xr,p);
			}else{
				xr = xn + ((y0*yr > 0)?+tol:-tol);
				T y = polyeval(xr,p);
				if(y*yr < 0) return xn;
				else         yr = y;
			}
		}else{
			xr = (x0+x1)*0.5;
			if(xr == x0 || xr == x1 || x1-x0 <= tol*tol)
				return xr;
			else
				yr = polyeval(xr,p);
		}
	}
	return xr;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint N>
void polysolveInterval(const T& x0,const T& x1,Polynomial<T,N>& p,const T tol = 5e-5,const uint iterations = 32)
{
	if constexpr (N == 3) return polysolve(p);
	else{
		Polynomial<T,N-1> pd = derivative(p);
		polysolveInterval(x0,x1,pd);
		T xStart = x0;
		for(uint i=0; i<N-1; ++i)
		{
			T xEnd = (i == N-2)?x1:pd.roots[i];
			p.roots[i] = std::isnan(xStart*xEnd)?
				std::numeric_limits<T>::quiet_NaN():polysolveInterval(xStart,xEnd,p,pd);
			xStart = (std::isnan(xEnd))?xStart:xEnd;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c,uint N>
void polysolveInterval(
	const Mat<T,r,c>& x0,
	const Mat<T,r,c>& x1,
	Polynomial<Mat<T,r,c>,N>& pv,
	const Mat<T,r,c> tol = Mat<T,r,c>(5e-5),
	const uint iterations = 32)
{
	static const uint n = r*c;
	Polynomial<T,3> p;
	for(uint i=0; i<n; ++i)
	{
		for(uint j=0; j<pv.nCoefs; ++j)
			p.coefs[j] = pv.coefs[i][j];
		polysolveInterval(x0[i],x1[i],p);
		for(uint j=0; j<pv.nCoefs; ++j)
			pv.roots[i][j] = p.coefs[j];
	}
}
////////////////////////////////////////////////////////////////////////////////
