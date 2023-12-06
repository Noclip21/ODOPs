#pragma once

#include <algorithm>

#include "BVH.h"


template<typename V,uint R>
requires(std::is_base_of_v<VolumeBase,V>)
inline void BVH<V,R>::traverse(
	std::function<const bool(const Volume&,const int)> nodeCallback,
	std::function<const bool(const int)> leafCallback) const
{
	enum Traversal
	{
		MOVE_DOWN,
		MOVE_RIGHT,
		MOVE_UP,
	};
	uint prevId = 0,curId = 0,state = MOVE_DOWN;
	const Node& root = nodes[0];
	while(true)
	{
		const Node& node = nodes[curId];
		if(prevId == node.right)
		{
			prevId = curId;
			curId  = node.parent;
			state  = MOVE_UP;
		}else{
			const uint nextId = (prevId == node.parent)?node.left:node.right;
			const bool intersect = (state == MOVE_DOWN && nodeCallback)?nodeCallback(node.volume,curId):true;
			if(intersect)
			{
				prevId = curId;
				curId  = nextId;
				state  = MOVE_DOWN;
				if(curId & leafMask)
				{
					if(leafCallback && !leafCallback(curId & ~leafMask)) break;
					std::swap(curId,prevId);
					state = MOVE_RIGHT;
				}
			}else{
				prevId = curId;
				curId  = node.parent;
				state  = MOVE_UP;
			}
		}

		if(curId == 0 && prevId == root.right) break;
	}
}
