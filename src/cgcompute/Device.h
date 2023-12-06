#pragma once

#include <iostream>
#include <list>
#include <algorithm>

#include <GL/glew.h>
#include <GL/gl.h>

#include <GLFW/glfw3.h>


class Device
{
private:

	GLFWwindow* _window;

	int _verbose;

public:

	enum
	{
		DEBUG_GLVERSION   = 6,
		DEBUG_GLSLVERSION = 7,
		DEBUG_GLEXT       = 8,
		DEBUG_GLVERBOSE   = 9,

		GLVERBOSE_ALL    = 10,
		GLVERBOSE_NOTE   = 11,
		GLVERBOSE_LOW    = 12,
		GLVERBOSE_MEDIUM = 13,
		GLVERBOSE_HIGH   = 14,
		GLVERBOSE_NONE   = 15,
	};


private:

	int& instanceCount() const
	{
		static int __instanceCount = 0;
		return __instanceCount;
	}

public:

	int getVerbse() const  { return _verbose; }
	void setVerbose(int v) { _verbose = v;    }


	void bindContext() const
	{
		glfwMakeContextCurrent(_window);
	}


	Device():
		_window(NULL),
		_verbose(GLVERBOSE_NONE)
	{
		if(instanceCount()++ <= 0) glfwInit();
	}
	virtual ~Device()
	{
		if(_window) glfwDestroyWindow(_window);
		if(--instanceCount() <= 0) glfwTerminate();
	}


	void createWindow(const char* name,const int* layout)
	{
		if(_window) glfwDestroyWindow(_window);
		_window =   glfwCreateWindow(1,1,name,NULL,NULL);
	}
	void init(const int* layout)
	{
		glewInit();
		while(int l = *layout++)
		{
			switch(l)
			{
				case(Device::DEBUG_GLVERSION):
				{
					const char* v = (const char*)glGetString(GL_VERSION);
					std::cout << "OpenGL version: " << ((v)?v:"ERROR") << std::endl;
					break;
				}
				case(Device::DEBUG_GLSLVERSION):
				{
					const char* s = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
					std::cout << "GLSL version:   " << ((s)?s:"ERROR") << std::endl;
					break;
				}
				case(Device::DEBUG_GLEXT):
				{
					const char* e = (const char*)glGetString(GL_EXTENSIONS);
					std::cout << "Extensions:\n" << ((e)?e:"ERROR") << std::endl;
					break;
				}
				case(Device::DEBUG_GLVERBOSE):
				{
					_verbose = *layout++;
					glEnable(GL_DEBUG_OUTPUT);
					glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
					glDebugMessageCallback(_debugCallback,this);
					break;
				}
			}
		}
	}


private:

	static void _debugCallback(GLenum  source,
                    	       GLenum  type,
                    	       GLuint  id,
                    	       GLenum  severity,
                    	       GLsizei length,
                    	       const GLchar* message,
                    	       const void*   userParam)
	{
		int verbose = GLVERBOSE_NONE;
		switch(severity)
		{
			case(GL_DEBUG_SEVERITY_HIGH):        verbose = GLVERBOSE_HIGH;   break;
			case(GL_DEBUG_SEVERITY_MEDIUM):      verbose = GLVERBOSE_MEDIUM; break;
			case(GL_DEBUG_SEVERITY_LOW):         verbose = GLVERBOSE_LOW;    break;
			case(GL_DEBUG_SEVERITY_NOTIFICATION):verbose = GLVERBOSE_NOTE;   break;
		}
		const Device* device = (Device*)userParam;
		if(verbose < device->_verbose) return;

		std::cout << "OpenGL ";
		switch(type)
		{
    		case(GL_DEBUG_TYPE_ERROR):
    			std::cout << "(ERROR)"; break;
    		case(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR):
        		std::cout << "(DEPRECATED BEHAVIOR)"; break;
    		case(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR):
        		std::cout << "(UNDEFINED BEHAVIOR)"; break;
    		case(GL_DEBUG_TYPE_PORTABILITY):
        		std::cout << "(PORTABILITY)"; break;
    		case(GL_DEBUG_TYPE_PERFORMANCE):
        		std::cout << "(PERFORMANCE)"; break;
    		case(GL_DEBUG_TYPE_OTHER):
        		std::cout << "(OTHER)"; break;
		}
    	std::cout << ": " << message << std::endl;
	}


};
