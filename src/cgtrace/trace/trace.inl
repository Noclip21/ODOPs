#pragma once

#include "../common/Hit.h"
#include "../common/Ray.h"
#include "../common/PrimimitiveArrayLayout.h"
#include "../bvh/traverse.inl"

#include "opU.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename RAY,typename LAYOUT>
const bool traceAnyHit(const RAY& ray,const LAYOUT& layout)
{
	for(uint i=0; i<layout.count; ++i)
		if(intersect<Hitb>(ray,layout,i).isHit) return true;
	return false;
}
////////////////////////////////////////////////////////////////////////////////
template<typename HIT,typename RAY,typename LAYOUT>
const HIT traceClosestHit(const RAY& ray,const LAYOUT& layout)
{
	HIT hit;
	for(uint i=0; i<layout.count; ++i)
		hit = opU(hit,intersect<HIT>(ray,layout,i));
	return hit;
}
////////////////////////////////////////////////////////////////////////////////
template<typename RAY,typename BVH,typename LAYOUT>
const bool traceAnyHit(const RAY& ray,const BVH& bvh,const LAYOUT& layout)
{
	bool isHit = false;
	bvh.traverse(
		[&](const auto& volume,const int id)->bool
		{
			return intersect<Hitb>(ray,volume).isHit;
		},
		[&](const int id)->bool
		{
			isHit = intersect<Hitb>(ray,layout,id).isHit;
			return !isHit;
		});
	return isHit;
}
////////////////////////////////////////////////////////////////////////////////
template<typename HIT,typename RAY,typename BVH,typename LAYOUT>
const HIT traceClosestHit(const RAY& ray,const BVH& bvh,const LAYOUT& layout)
{
	HIT hit;
	bvh.traverse(
		[&](const auto& volume,const int id)->bool
		{
			return intersect<Hitb>(ray,volume).isHit;
		},
		[&](const int id)->bool
		{
			hit = opU(hit,intersect<HIT>(ray,layout,id));
			return true;
		});
	return hit;
}
////////////////////////////////////////////////////////////////////////////////
