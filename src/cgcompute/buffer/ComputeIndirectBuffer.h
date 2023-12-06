#pragma once

#include "Buffer.h"

////////////////////////////////////////////////////////////////////////////////
struct ComputeCommand 
{
	int numGroupsX;
	int numGroupsY;
	int numGroupsZ;
};
////////////////////////////////////////////////////////////////////////////////
class ComputeIndirectBuffer: public Buffer
{
private:

	int _numCommands;
	int _stride;


public:

	int numCommands() const { return _numCommands; }
	int stride() const      { return _stride;      }


	void bind() const
	{
		glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER,getId());
	}


	ComputeIndirectBuffer(const ComputeCommand* data,
			              const int             size,
			              const int             defStride = 0,
			              const int             hints     = 0):
		Buffer((const float*)data,size,hints),
		_numCommands(size/sizeof(ComputeCommand)),
		_stride(defStride)
	{
		// code
	}
	ComputeIndirectBuffer(const ComputeIndirectBuffer& copyBuffer,
			              const int                    hints = 0):
		Buffer(copyBuffer,hints),
		_numCommands(copyBuffer._numCommands),
		_stride(copyBuffer._stride)
	{
		// code
	}
	virtual ~ComputeIndirectBuffer()
	{
		// code
	}


};
