#include <iostream>

#include "PathTracer.h"

int main(int argc,char* argv[])
{
	return !(PathTracer::instance().load() && PathTracer::instance().draw());
}
