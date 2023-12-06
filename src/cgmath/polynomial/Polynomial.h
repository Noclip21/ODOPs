#pragma once


template<typename T,uint N>
struct Polynomial
{
	static constexpr uint nCoefs = N;
	static constexpr uint deg    = N-1;
	static constexpr uint nRoots = N-1;

	using Type = T;
	T coefs[nCoefs];
	T roots[nRoots];

	Polynomial();
	template<typename T2>
	Polynomial(const Polynomial<T2,N>&);
	template<typename ...Args>
	Polynomial(const Args&...);

};
