#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>
#include <fstream>


class Shader
{
private:

	GLuint  _shaderProgram;

public:

	const GLuint getId() const
	{
		return _shaderProgram;
	}
	void bind() const
	{
		glUseProgram(_shaderProgram);
	}
	static void unbind()
	{
		glUseProgram(0);
	}


	const GLint getUniform(const char* name) const
	{
		return glGetUniformLocation(_shaderProgram,name);
	}
	const GLint getUniformBlock(const char* name) const
	{
		return glGetUniformBlockIndex(_shaderProgram,name);
	}
	const GLint getStorageBlock(const char* name) const
	{
		return glGetProgramResourceIndex(_shaderProgram,GL_SHADER_STORAGE_BLOCK,name);
	}


	void bindUniformBlock(const GLuint point,const int blockid) const
	{
		glUniformBlockBinding(_shaderProgram,blockid,point);
	}
	void bindStorageBlock(const GLuint point,const int blockid) const
	{
		glShaderStorageBlockBinding(_shaderProgram,blockid,point);
	}


	void uniform1i(const GLint loc,const int i) const
	{
		glProgramUniform1i(_shaderProgram,loc,i);
	}
	void uniform1f(const GLint loc,const float f) const
	{
		glProgramUniform1f(_shaderProgram,loc,f);
	}
	void uniform1iv(const GLint loc,const int* i,const int n = 1) const
	{
		glProgramUniform1iv(_shaderProgram,loc,n,i);
	}
	void uniform1fv(const GLint loc,const float* f,const int n = 1) const
	{
		glProgramUniform1fv(_shaderProgram,loc,n,f);
	}
	void uniform2fv(const GLint loc,const float* v,const int n = 1) const
	{
		glProgramUniform2fv(_shaderProgram,loc,n,v);
	}
	void uniform3fv(const GLint loc,const float* v,const int n = 1) const
	{
		glProgramUniform3fv(_shaderProgram,loc,n,v);
	}
	void uniform4fv(const GLint loc,const float* v,const int n = 1) const
	{
		glProgramUniform4fv(_shaderProgram,loc,n,v);
	}
	void uniformMat3fv(const GLint loc,const float* m,
	                   const bool trans = false,
	                   const int  n     = 1) const
	{
		glProgramUniformMatrix3fv(_shaderProgram,loc,n,trans,m);
	}
	void uniformMat4fv(const GLint loc,const float* m,
	                   const bool trans = false,
	                   const int  n     = 1) const
	{
		glProgramUniformMatrix4fv(_shaderProgram,loc,n,trans,m);
	}
	void uniformHandle(const GLint loc,const uint64_t handle)
	{
		glProgramUniformHandleui64ARB(_shaderProgram,loc,handle);
	}
	void uniformHandlev(const GLint loc,const uint64_t* handle,
	                    const int n = 1) const
	{
		glProgramUniformHandleui64vARB(_shaderProgram,loc,n,handle);
	}


	void uniform1i(const char* name,const int i) const
	{
		uniform1i(getUniform(name),i);
	}
	void uniform1f(const char* name,const float f) const
	{
		uniform1f(getUniform(name),f);
	}
	void uniform1iv(const char* name,const int* i,const int n = 1) const
	{
		uniform1iv(getUniform(name),i,n);
	}
	void uniform1fv(const char* name,const float* f,const int n = 1) const
	{
		uniform1fv(getUniform(name),f,n);
	}
	void uniform2fv(const char* name,const float* v,const int n = 1) const
	{
		uniform2fv(getUniform(name),v,n);
	}
	void uniform3fv(const char* name,const float* v,const int n = 1) const
	{
		uniform3fv(getUniform(name),v,n);
	}
	void uniform4fv(const char* name,const float* v,const int n = 1) const
	{
		uniform4fv(getUniform(name),v,n);
	}
	void uniformMat3fv(const char* name,const float* m,
	                   const bool trans = false,
	                   const int  n     = 1) const
	{
		uniformMat3fv(getUniform(name),m,trans,n);
	}
	void uniformMat4fv(const char* name,const float* m,
	                   const bool trans = false,
	                   const int  n     = 1) const
	{
		uniformMat4fv(getUniform(name),m,trans,n);
	}
	void uniformHandle(const char* name,const uint64_t handle)
	{
		uniformHandle(getUniform(name),handle);
	}
	void uniformHandlev(const char* name,const uint64_t* handle,
	                    const int n = 1) const
	{
		uniformHandlev(getUniform(name),handle,n);
	}


private:


	static GLuint _compile(const char* source,
	                       const bool  debug = true)
	{
		GLuint shader = glCreateShaderProgramv(GL_COMPUTE_SHADER,1,&source);
		if(!debug) return shader;
 	
		GLint compileStatus;
    	glGetProgramiv(shader,GL_LINK_STATUS,&compileStatus);
    	std::cout << "shader compiling "
	          	  << ((compileStatus == GL_TRUE)?"sucessful":"failed")
	          	  << std::endl;
    	if(compileStatus == GL_TRUE) return shader;

    	char errorBuffer[512];
    	glGetProgramInfoLog(shader,512,NULL,errorBuffer);
    	std::cout << errorBuffer << std::endl;
    	
    	return 0;
	}


public:

	Shader( const bool  fromFile,
           const char* source,
	       const bool  debug = true)
	{
		if(!fromFile)
		{
			_shaderProgram = _compile(source,debug);
			return;
		}

		std::ifstream fileHandle;
		int           fileSize;
		char*         fileBuffer;

		fileHandle.open(source,(std::ios_base::openmode)std::ios::in);
		if(!fileHandle.good())
		{
			if(debug)
				std::cout << source
                          << ": error opening file"
                          << std::endl;
			return;
		}

    	fileHandle.seekg(0,fileHandle.end);
    	fileSize = fileHandle.tellg();
    	fileHandle.seekg(0,fileHandle.beg);

		fileBuffer = new char[fileSize+1];
    	fileHandle.read(fileBuffer,fileSize);
    	fileBuffer[fileSize] = '\0';

		fileHandle.close();

		if(debug) std::cout << source << ": ";
		_shaderProgram = _compile(fileBuffer,debug);
		delete[] fileBuffer;
	}
	virtual ~Shader()
	{
		glDeleteProgram(_shaderProgram);
	}


};
