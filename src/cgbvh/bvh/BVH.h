#pragma once

#include <vector>
#include <functional>

#include <cgmath>

#include "../volumes/Volume.h"
#include "Node.h"


template<typename V,uint R = 1>
requires(std::is_base_of_v<Volume,V>)
struct BVH
{
	using Volume = V;
	using Node = Node<V>;

	std::vector<Node> nodes;

	static const int leafMask = 0x80000000;


	BVH() = default;
	template<typename V2,uint R2>
	BVH(const BVH<V2,R2>&);
	BVH(const vec3*,const uint);
	BVH(const vec3*,const int*,const uint);
	BVH(const Volume*,const uint);

	template<typename V2,uint R2>
	inline const BVH& operator = (const BVH<V2,R2>&);

	inline void traverse(
		std::function<const bool(const Volume&,const int)>,
		std::function<const bool(const int)>) const;
};
