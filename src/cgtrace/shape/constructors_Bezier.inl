#pragma once

#include <vector>

#include "Bezier.h"


////////////////////////////////////////////////////////////////////////////////
Bezier::Bezier(const vec3& a,const vec3& b,const vec3& c,const vec3& d):
	a(a),b(b),c(c),d(d){}
////////////////////////////////////////////////////////////////////////////////
// Compute bezier from vertices using polynomial regression
Bezier::Bezier(const vec3* y,const uint n)
{
	// coefs = (X^t*X)*X^t*y

	// X
	std::vector<vec4> x(n);
	x[0] = 0;
	for(uint i=0; i<n; ++i) x[i][0] = 1;
	for(uint i=1; i<n; ++i) x[i][1] = x[i-1][1] + distance(y[i-1],y[i]);
	for(uint i=1; i<n; ++i) x[i][1] /= x[n-1][1];
	for(uint i=1; i<n; ++i) for(uint j=2; j<4; ++j) x[i][j] = pow(x[i][1],float(j));

	// M = (X^t*X)^-1
	mat4 M;
	for(uint i=0; i<4; ++i)
		for(uint j=i; j<4; ++j)
		{
			float Mx = 0;
			for(uint k=0; k<n; ++k)
				Mx += x[k][i]*x[k][j];
			M(i,j) = M(j,i) = Mx;
		}
	M = inverse(M);

	// coefs = M*X^t*y
	mat4x3 coefs;
	for(uint i=0; i<3; ++i)
	{
		vec4 v;
		for(uint j=0; j<4; ++j)
		{
			float vx = 0;
			for(uint k=0; k<n; ++k)
				vx += x[k][j]*y[k][i];
			v[j] = vx;
		}
		coefs(i) = M*v;
	}

	mat3x4 coefsT = transpose(coefs);
	a = coefsT(0);
	b = coefsT(1)/3 + a;
	c = coefsT(2)/3 - a + 2*b;
	d = coefsT(3)   + a - 3*b + 3*c;

	a = y[0];
	d = y[n-1];
}
////////////////////////////////////////////////////////////////////////////////
