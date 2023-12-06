#pragma once

#include "../common/Ray.h"
#include "../common/Hit.h"
#include "../shape/Bezier.h"
#include "../shape/interpolate_Bezier.inl"
#include "polynomial/polysolve.inl"


////////////////////////////////////////////////////////////////////////////////
// http://www.cemyuksel.com/research/polynomials/polynomial_roots_hpg2022_supplemental.pdf
template<typename HIT,typename RAY,uint N>
const HIT intersect(const RAY& ray,const Bezier& bezier,const Polynomial<float,N>& pr)
{
	const Quaternion q = make(ray.d,vec3(0,0,1));
	Bezier bez = bezier;
	bez.a = q*(bez.a-ray.o);
	bez.b = q*(bez.b-ray.o);
	bez.c = q*(bez.c-ray.o);
	bez.d = q*(bez.d-ray.o);

	// f(t)  = t³(-a+3b-3c+d) +t²(+3a-6b+3c) +t(-3a+3b) +a
	// f(t)  = A+Bt+Ct²+Dt³
	// f'(t) = B+2Ct+3Dt²
	const vec3 &a = bez.a, &b = bez.b, &c = bez.c, &d = bez.d;
	const vec3 A = bez.a, B = -3*(a-b), C = 3*(a-2*b+c), D = (-a+3*b-3*c+d);

	const Polynomial<double,4> px(A.x,B.x,C.x,D.x);
	const Polynomial<double,4> py(A.y,B.y,C.y,D.y);
	const Polynomial<double,4> pz(A.z,B.z,C.z,D.z);
	const auto pxd = derivative(px);
	const auto pyd = derivative(py);
	const auto pzd = derivative(pz);

	auto _pp = pxd*px + pyd*py;
	auto pp = (pzd*pzd)*(px*px + py*py - pr*pr) + _pp*_pp;
	polysolveInterval(0.0,1.0,pp);

	float t = -1,s;
	for(uint i=0; i<pp.nRoots; ++i)
		if(pp.roots[i] >= 0 && pp.roots[i] <= 1)
		{
			float t0 = dot(
				bez.interpolate(pp.roots[i]),
				bez.interpolatedt(pp.roots[i]))/polyeval(pp.roots[i],pzd);
			if((t0 < t && t0 >= 0) || t < 0)
			{
				s = pp.roots[i];
				t = t0;
			}
		}

	const bool isHit = (t >= 0);
	HIT hit;

	if constexpr (is_Hitb<HIT>)
		hit.isHit = isHit;

	if constexpr (is_Hitf<HIT>)
		hit.t = t;

	if constexpr (is_Hitn<HIT>)
	{
		vec3 n = ray.o+t*ray.d-bezier.interpolate(s);
		hit.n = normalize(n);
	}

	//if constexpr (is_Hitn<HIT>)
	//	hit.n = 1; // TODO

	//if constexpr (is_Hituv<HIT>)
	//	hit.uv = 1; // TODO

	//if constexpr (is_Hitduv<HIT>)
	//{
	//	hit.uvdx = 1; // TODO
	//	hit.uvdy = 1; // TODO
	//}

	return hit;
}
////////////////////////////////////////////////////////////////////////////////
template<typename HIT,typename RAY>
const HIT intersect(const RAY& ray,const Bezier& bezier,const float r)
{
	const Quaternion q = make(ray.d,vec3(0,0,1));
	Bezier bez = bezier;
	bez.a = q*(bez.a-ray.o);
	bez.b = q*(bez.b-ray.o);
	bez.c = q*(bez.c-ray.o);
	bez.d = q*(bez.d-ray.o);

	// f(t)  = t³(-a+3b-3c+d) +t²(+3a-6b+3c) +t(-3a+3b) +a
	// f(t)  = A+Bt+Ct²+Dt³
	// f'(t) = B+2Ct+3Dt²
	const vec3 &a = bez.a, &b = bez.b, &c = bez.c, &d = bez.d;
	const vec3 A = bez.a, B = -3*(a-b), C = 3*(a-2*b+c), D = (-a+3*b-3*c+d);

	// solve f_x(t)f_x'(t) + f_y(t)f_y'(t) = 0
	// f(x)f'(x) = AB + (2AC + B²)t + (3AD + 3x1_C)t² + (4B_D + 2C²)t³ + (5CD)t⁴ + 3D²t⁵
	Polynomial<float,6> pp(
  		  (A.x*B.x + A.y*B.y),
		2*(A.x*C.x + A.y*C.y) +   (B.x*B.x + B.y*B.y),
		3*(A.x*D.x + A.y*D.y) + 3*(B.x*C.x + B.y*C.y),
		4*(B.x*D.x + B.y*D.y) + 2*(C.x*C.x + C.y*C.y),
		5*(C.x*D.x + C.y*D.y),
		3*(D.x*D.x + D.y*D.y));

	polysolveInterval(0.0f,1.0f,pp);

	float t = -1,s;
	for(uint i=0; i<pp.nRoots; ++i)
		if(pp.roots[i] >= 0 && pp.roots[i] <= 1)
		{
			const vec3 sp = bez.interpolate(pp.roots[i]);
			if(sp.z < 0) continue;
			if(length2(vec2(sp.x,sp.y)) > r*r) continue;
			if((sp.z < t && sp.z >= 0) || t < 0)
			{
				s = pp.roots[i];
				t = sp.z;
			}
		}

	const bool isHit = (t >= 0);
	HIT hit;

	if constexpr (is_Hitb<HIT>)
		hit.isHit = isHit;

	if constexpr (is_Hitf<HIT>)
		hit.t = t;

	if constexpr (is_Hitn<HIT>)
	{
		vec3 n = ray.o+t*ray.d-bezier.interpolate(s);
		hit.n = normalize(n);
	}

	//if constexpr (is_Hitn<HIT>)
	//	hit.n = 1; // TODO

	//if constexpr (is_Hituv<HIT>)
	//	hit.uv = 1; // TODO

	//if constexpr (is_Hitduv<HIT>)
	//{
	//	hit.uvdx = 1; // TODO
	//	hit.uvdy = 1; // TODO
	//}

	return hit;
}
////////////////////////////////////////////////////////////////////////////////
