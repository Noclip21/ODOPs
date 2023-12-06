#pragma once

#include "types.h"

#include "../mat/Mat.h"
#include "../mat/constructors.inl"
#include "../mat/operators.inl"

#include "../swizzle/Swizzle.h"
#include "../swizzle/operators.inl"

#include "dot.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint ca,uint cb,uint n>
inline const Mat<std::common_type_t<T1,T2>,ca,cb> transposeMulti(const Mat<T1,n,ca>& a,const Mat<T2,n,cb>& b)
{
	Mat<std::common_type_t<T1,T2>,ca,cb> m;
	for(uint i=0; i<cb; ++i)
	{
		auto& mv = m(i);
		const auto& bv = b(i);
    	for(uint j=0; j<ca; ++j)
    	{
    		mv[j] = 0;
    		const auto& av = a(j);
    		for(uint k=0; k<n; ++k)
        		mv[j] += av[k]*bv[k];
    	}
    }
	return m;
}
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint cb,uint n,uint mask>
inline auto transposeMulti(const Swizzle<T1,n,mask>& a,const Mat<T2,n,cb>& b)
{ return transposeMulti((const Mat<T1,n,1>)a,b); }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint ca,uint n,uint mask>
inline auto transposeMulti(const Mat<T1,n,ca>& a,const Swizzle<T2,n,mask>& b)
{ return transposeMulti(a,(const Mat<T2,n,1>)b); }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n,uint mask1,uint mask2>
inline const Mat<std::common_type_t<T1,T2>,1,1> transposeMulti(const Swizzle<T1,n,mask1>& a,const Swizzle<T2,n,mask2>& b)
{ return Mat<std::common_type_t<T1,T2>,1,1>(dot(a,b)); }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n>
inline const Mat<std::common_type_t<T1,T2>,1,1> transposeMulti(const Mat<T1,n,1>& a,const Mat<T2,n,1>& b)
{ return Mat<std::common_type_t<T1,T2>,1,1>(dot(a,b)); }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n,uint mask>
inline const Mat<std::common_type_t<T1,T2>,1,1> transposeMulti(const Swizzle<T1,n,mask>& a,const Mat<T2,n,1>& b)
{ return Mat<std::common_type_t<T1,T2>,1,1>(dot(a,b)); }
////////////////////////////////////////////////////////////////////////////////
template<typename T1,typename T2,uint n,uint mask>
inline const Mat<std::common_type_t<T1,T2>,1,1> transposeMulti(const Mat<T1,n,1>& a,const Swizzle<T2,n,mask>& b)
{ return Mat<std::common_type_t<T1,T2>,1,1>(dot(a,b)); }
////////////////////////////////////////////////////////////////////////////////
