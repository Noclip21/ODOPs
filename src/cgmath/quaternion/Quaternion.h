#pragma once

#include "../common/types.h"
#include "../mat/Mat.h"

template<typename T>
struct Quaternion: Mat<T,4,1>
{
	using MatData<T,4,1>::data;

	Quaternion();
	Quaternion(const Quaternion&);
	Quaternion(const T,const T,const T,const T);
	Quaternion(const Mat<T,4,1>&);
	Quaternion(const Mat<T,3,1>&,const T);

	inline Quaternion& operator = (const Quaternion&);
	inline Quaternion& operator = (const Mat<T,4,1>&);
};
