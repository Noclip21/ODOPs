#pragma once

#include <iostream>

#include "Mat.h"

////////////////////////////////////////////////////////////////////////////////
template<typename T,uint rows,uint cols>
inline std::istream& read(std::istream& is,Mat<T,rows,cols>* data,uint count = 1)
{
	return is.read((char*)data,sizeof(Mat<T,rows,cols>)*count);
}
////////////////////////////////////////////////////////////////////////////////
template<typename T,uint rows,uint cols>
inline std::ostream& write(std::ostream& os,const Mat<T,rows,cols>* data,uint count = 1)
{
	return os.write((const char*)data,sizeof(Mat<T,rows,cols>)*count);
}
////////////////////////////////////////////////////////////////////////////////
