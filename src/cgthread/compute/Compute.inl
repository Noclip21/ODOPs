#pragma once

#include "Compute.h"
#include "../thread/Thread.h"
#include "../thread/Thread.inl"


////////////////////////////////////////////////////////////////////////////////
template<typename ... N>
Compute<N...>::Compute(N ... n):task(n...){};
////////////////////////////////////////////////////////////////////////////////
template<typename ... N>
template<typename F,typename ... T>
void Compute<N...>::dispatch(F callback,T&& ... args)
{
	for(uint64 i=0; i<task.numThreads; ++i)
	{
		Thread::getThread(i).setCallback([&,i](void)->void
		{
			for(uint64 j=0; j<task.numTasksPerThread[i]; ++j)
			{
				auto input = task(task.taskOffsetPerThread[i]+j);
				input.threadId = i;
				callback(input,std::forward<T>(args)...);
			}
		});
		Thread::getThread(i).dispatch();
	}
}
////////////////////////////////////////////////////////////////////////////////
template<typename ... N>
void Compute<N...>::sync()
{
	for(uint64 i=0; i<task.numThreads; ++i)
		Thread::getThread(i).sync();
}
////////////////////////////////////////////////////////////////////////////////
