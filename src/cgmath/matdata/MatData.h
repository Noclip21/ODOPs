#pragma once

#include "../common/types.h"
#include "../matbase/MatBase.h"
#include "../swizzle/shuffle.inl"
#include "../swizzle/Swizzle.h"


////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint c>
struct MatData: MatBase<T,r,c>
{ 
	union{
		T data[r*c];
	};
};
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint r,uint _c>
requires(r > 1 && _c > 1)
struct MatData<T,r,_c>: MatBase<T,r,_c>
{ 
	union{
		T data[r*_c];
		MatData<T,r,1> c[_c];
	};
};
////////////////////////////////////////////////////////////////////////////////
template<typename _T>
struct MatData<_T,2,1>: MatBase<_T,2,1>
{ 
	union{
		_T data[2];
		struct{ _T x,y; };
		struct{ _T r,g; };
		struct{ _T s,t; };
		MatData<_T,1,2> T;
		VEC2_SWIZZLE_UNION(_T)
	};
};
////////////////////////////////////////////////////////////////////////////////
template<typename _T>
struct MatData<_T,3,1>: MatBase<_T,3,1>
{ 
	union{
		_T data[3];
		struct{ _T x,y,z; };
		struct{ _T r,g,b; };
		struct{ _T s,t,u; };
		MatData<_T,1,3> T;
		VEC3_SWIZZLE_UNION(_T)
	};
};
////////////////////////////////////////////////////////////////////////////////
template<typename _T>
struct MatData<_T,4,1>: MatBase<_T,4,1>
{ 
	union{
		_T data[4];
		struct{ _T x,y,z,w; };
		struct{ _T r,g,b,a; };
		struct{ _T s,t,u,v; };
		MatData<_T,1,4> T;
		VEC4_SWIZZLE_UNION(_T)
	};
};
////////////////////////////////////////////////////////////////////////////////
