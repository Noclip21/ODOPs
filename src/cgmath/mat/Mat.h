#pragma once

#include <type_traits>

#include "../common/types.h"
#include "../matdata/MatData.h"


template<typename T,uint r,uint c>
struct Mat: MatData<T,r,c>
{
	using MatBase<T,r,c>::rows;
	using MatBase<T,r,c>::cols;
	using MatBase<T,r,c>::n;
	using MatData<T,r,c>::data;

	Mat(const Mat&);
	Mat();
	Mat(const T*);
	template<typename T2>
	Mat(const T2);
	template<typename T2>
	Mat(const Mat<T2,r,c>&);
	template<uint mask>
	Mat(const Swizzle<T,r,mask>&);
	template<typename T2,uint r2,uint c2>
	requires(r2 < r || c2 < c)
	explicit Mat(const Mat<T2,r2,c2>&);
	template<typename T2,uint r2,uint c2>
	requires(r2 > r || c2 > c)
	explicit Mat(const Mat<T2,r2,c2>&);
	template<typename ... T2>
	requires(sizeof...(T2) == c)
	Mat(const Mat<T2,r,1>& ...);
	template<typename ... Args>
	Mat(const Args& ...);


	inline const Mat& operator = (const Mat&) const;
	inline Mat& operator = (const Mat&);

	inline operator T() const requires(r*c == 1);

	inline operator const T* () const;
	const T& operator [] (const uint i) const;
	inline const T& operator () (const uint i,const uint j) const;
	inline const Mat<T,r,1>& operator () (const uint i) const;

	inline operator T* ();
	inline T& operator [] (const uint i);
	inline T& operator () (const uint i,const uint j);
	inline Mat<T,r,1>& operator () (const uint i);

	inline const T* begin() const;
	inline const T* end() const;

	inline T* begin();
	inline T* end();
};
