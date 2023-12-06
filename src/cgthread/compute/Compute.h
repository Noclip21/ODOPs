#pragma once

#include <functional>

#include "ComputeTask.h"
#include "compute/ComputeInput.h"

#include "../thread/Thread.h"
#include "../thread/Thread.inl"


template<typename ... N>
struct Compute
{
	const ComputeTask<N...> task;

	Compute(N ... n);

	template<typename F,typename ...T>
	void dispatch(F,T&&...);
	void sync();
};
