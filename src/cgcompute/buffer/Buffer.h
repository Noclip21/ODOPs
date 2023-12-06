#pragma once

#include <GL/glew.h>
#include <GL/gl.h>


class Buffer
{
private:

	GLuint _buffer;
	int    _size;

public:

	enum
	{
		DYNAMIC_STORAGE = (GL_DYNAMIC_STORAGE_BIT),
		MAP_PERSISTENT  = (GL_MAP_WRITE_BIT | GL_MAP_READ_BIT |
		                  GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT),
	};


public:

	const GLuint getId() const
	{ 
		return _buffer;
	}
	const int getSize() const
	{
		return _size;
	}


	void bind(const int index) const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER,index,_buffer);
	}
	void bind(const int index,const int size,const int offset) const
	{
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER,index,_buffer,offset,size);
	}
	static void unbind(const int index)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER,index,0);
	}


	void clear() const
	{
		glInvalidateBufferData(_buffer);
	}
	void clear(const int size,const int offset) const
	{
		glInvalidateBufferSubData(_buffer,offset,size);
	}
	void copy(const Buffer& readBuffer)
	{
		glCopyNamedBufferSubData(readBuffer._buffer,_buffer,0,0,
			(readBuffer._size < _size)?readBuffer._size:_size);
	}
	void copy(const Buffer& readBuffer,const int size,
	          const int readOffset,const int writeOffset) const
	{
		glCopyNamedBufferSubData(readBuffer._buffer,_buffer,readOffset,writeOffset,size);
	}
	// Only allowed if DYNAMIC_STORAGE
	void read(void* data) const
	{
		glGetNamedBufferSubData(_buffer,0,_size,data);
	}
	void write(const void* data) const
	{
		glNamedBufferSubData(_buffer,0,_size,data);
	}
	void read(void* data,const int size,const int offset) const
	{
		glGetNamedBufferSubData(_buffer,offset,size,data);
	}
	void write(const void* data,const int size,const int offset) const
	{
		glNamedBufferSubData(_buffer,offset,size,data);
	}
	// Only allowed if MAP_PERSISTENT
	float* map() const
	{
		return (float*)glMapNamedBuffer(_buffer,GL_READ_WRITE);
	}
	float* map(const int size,const int offset) const
	{
		return (float*)glMapNamedBufferRange(_buffer,offset,size,
			GL_MAP_WRITE_BIT | GL_MAP_READ_BIT |
			GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	}
	void unmap() const
	{
		glUnmapNamedBuffer(_buffer);
	}


	Buffer(const void* data,const int size,const int hints = 0):
		_size(size)
	{
		glCreateBuffers(1,&_buffer);
		glNamedBufferStorage(_buffer,_size,data,hints);
	}
	Buffer(const Buffer& copyBuffer,const int hints = 0):
		Buffer(NULL,copyBuffer._size,hints)
	{
		copy(copyBuffer,_size,0,0);
	}
	virtual ~Buffer()
	{
		glDeleteBuffers(1,&_buffer);
	}


};
