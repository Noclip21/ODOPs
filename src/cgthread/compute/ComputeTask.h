#pragma once

#include "ComputeInput.h"


template<typename ... N>
struct ComputeTask
{
	const uint64 dimension;
	const uint64 size[sizeof...(N)];
	const uint64 numTasks;
	const uint64 numThreads;
	uint64*      numTasksPerThread;
	uint64*      taskOffsetPerThread;

	ComputeTask(N ...);
	~ComputeTask();

	const ComputeInput<sizeof...(N)> operator () (const uint64) const;
};
