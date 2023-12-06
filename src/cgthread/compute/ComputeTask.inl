#pragma once

#include "../thread/Thread.h"
#include "ComputeTask.h"


////////////////////////////////////////////////////////////////////////////////
template<typename ... N>
ComputeTask<N...>::ComputeTask(N ... n):
	dimension(sizeof...(N)),size{uint64(n)...},numTasks((n*...)),
	numThreads((numTasks <= Thread::numThreads())?numTasks:Thread::numThreads())
{
	numTasksPerThread      = new uint64[numThreads];
	taskOffsetPerThread    = new uint64[numThreads];

	uint64 minTasksPerThread = numTasks/numThreads;
	int  extraTasks        = numTasks%(minTasksPerThread*numThreads);

	taskOffsetPerThread[0] = 0;
	numTasksPerThread[0]   = minTasksPerThread+uint64(extraTasks-- > 0);
	for(uint64 i=1; i<numThreads; ++i)
	{
		taskOffsetPerThread[i] = taskOffsetPerThread[i-1]+numTasksPerThread[i-1];
		numTasksPerThread[i]   = minTasksPerThread+uint64(extraTasks-- > 0);
	}
}
////////////////////////////////////////////////////////////////////////////////
template<typename ... N>
ComputeTask<N...>::~ComputeTask()
{
	delete[] numTasksPerThread;
	delete[] taskOffsetPerThread;
}
////////////////////////////////////////////////////////////////////////////////
template<typename ... N>
const ComputeInput<sizeof...(N)> ComputeTask<N...>::operator () (const uint64 i) const
{
	ComputeInput<sizeof...(N)> input;
	uint64 d = 1;
	for(uint64 j=0; j<sizeof...(N); ++j)
	{
		input.size[j] = size[j];
		input.id[j]   = (i/d)%size[j];
		d *= size[j];
	}
	return input;
}
////////////////////////////////////////////////////////////////////////////////
