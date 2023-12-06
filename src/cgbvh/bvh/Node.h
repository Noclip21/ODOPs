#pragma once

#include "../volumes/Volume.h"


template<typename V>
requires(std::is_base_of_v<Volume,V>)
struct Node
{
	uint parent;
	uint left,right;
	V volume;
	
	Node():parent(0),left(0),right(0),volume(){}

	Node(const uint p,const uint l,const uint r,const V& v):
		parent(p),left(l),right(r),volume(v){}

	template<typename V2>
	Node(const Node<V2>& node):
		Node(node.parent,node.left,node.right,V(node.volume)){}
};
