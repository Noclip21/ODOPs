#pragma once

#include <vector>
#include <functional>

#include <cgmath>

#include "../common/PrimimitiveArrayLayout.h"
#include "../shape/ShapeBase.h"
#include "Node.h"


template<typename V,uint R = 1>
requires(std::is_base_of_v<VolumeBase,V>)
struct BVH
{
	static const uint radius = R;

	using Volume = V;
	using Node = ::Node<V>;

	std::vector<Node> nodes;

	static const int leafMask = 0x80000000;


	BVH() = default;
	template<typename V2,uint R2>
	BVH(const BVH<V2,R2>&);
	template<typename LAYOUT>
	requires(is_PrimitiveArray<LAYOUT>)
	BVH(const LAYOUT&,const bool = false);

	template<typename VP = V,typename LAYOUT>
	requires(std::is_base_of_v<VolumeBase,VP> && is_PrimitiveArray<LAYOUT>)
	static const BVH partitionedBuild(const LAYOUT&,const uint,const bool = false);


	template<typename V2,uint R2>
	inline const BVH& operator = (const BVH<V2,R2>&);


	inline void traverse(
		std::function<const bool(const Volume&,const int)>,
		std::function<const bool(const int)>) const;
};
