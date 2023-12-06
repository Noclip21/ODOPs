#pragma once

#include <iostream>

#include "BVH.h"
#include "../shape/serialize.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename V>
requires(std::is_base_of_v<ShapeBase,V>)
inline std::istream& read(std::istream& is,BVH<V>* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		BVH<V>& bvh = data[i];
		int numNodes;
		is.read((char*)&numNodes,sizeof(int));
		bvh.nodes.resize(numNodes);
		for(uint i=0; i<numNodes; ++i)
		{
			typename BVH<V>::Node& node = bvh.nodes[i];
			is.read((char*)&node.parent,sizeof(int));
			is.read((char*)&node.left,sizeof(int));
			is.read((char*)&node.right,sizeof(int));
			read(is,&node.volume);
		}
	}
	return is;
}
////////////////////////////////////////////////////////////////////////////////
template<typename V>
requires(std::is_base_of_v<ShapeBase,V>)
inline std::ostream& write(std::ostream& os,const BVH<V>* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		const BVH<V>& bvh = data[i];
		const int numNodes = bvh.nodes.size();
		os.write((const char*)&numNodes,sizeof(int));
		for(uint i=0; i<numNodes; ++i)
		{
			const typename BVH<V>::Node& node = bvh.nodes[i];
			os.write((const char*)&node.parent,sizeof(int));
			os.write((const char*)&node.left,sizeof(int));
			os.write((const char*)&node.right,sizeof(int));
			write(os,&node.volume);
		}
	}
	return os;
}
////////////////////////////////////////////////////////////////////////////////
