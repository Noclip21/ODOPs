#pragma once

#include "../common/types.h"

template<uint64 D>
struct ComputeInput
{
	uint64 threadId;
	uint64 id[D];
	uint64 size[D];
};
