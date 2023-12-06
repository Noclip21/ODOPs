#pragma once

#include "Buffer.h"



class UniformBuffer: public Buffer
{
public:

	void bind(const int index) const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER,index,getId());
	}
	void bind(const int index,const int size,const int offset) const
	{
		glBindBufferRange(GL_UNIFORM_BUFFER,index,getId(),offset,size);
	}
	static void unbind(const int index)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER,index,0);
	}


	UniformBuffer(const float* data,
	              const int    size,
	              const int    hints = 0):
		Buffer(data,size,hints)
	{
		// code
	}
	UniformBuffer(const UniformBuffer& copyBuffer,const int hints = 0):
		Buffer(copyBuffer,hints)
	{
		// code
	}
	virtual ~UniformBuffer()
	{
		// code
	}


};
