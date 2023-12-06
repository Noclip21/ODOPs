#pragma once

#include <iostream>

#include "VertStream.h"
#include "Command.h"

////////////////////////////////////////////////////////////////////////////////
inline std::istream& read(std::istream& is,VertStream* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		VertStream& vs = data[i];
		int attributeLayoutSize;
		int nComponentsSize;
		int drawCommandsSize;
		int indicesSize;
		int vertexArraySize;

		is.read((char*)&attributeLayoutSize,sizeof(int));
		is.read((char*)&nComponentsSize,sizeof(int));
		is.read((char*)&drawCommandsSize,sizeof(int));
		is.read((char*)&indicesSize,sizeof(int));
		is.read((char*)&vertexArraySize,sizeof(float));

		vs.attributeLayout.resize(attributeLayoutSize);
		vs.nComponents.resize(nComponentsSize);
		vs.drawCommands.resize(drawCommandsSize);
		vs.indices.resize(indicesSize);
		vs.vertexArray.resize(vertexArraySize);

		is.read((char*)&vs.attributeLayout[0],sizeof(int)*attributeLayoutSize);
		is.read((char*)&vs.nComponents[0],sizeof(int)*nComponentsSize);
		is.read((char*)&vs.drawCommands[0],sizeof(VertStream::Command)*drawCommandsSize);
		is.read((char*)&vs.indices[0],sizeof(int)*indicesSize);
		is.read((char*)&vs.vertexArray[0],sizeof(float)*vertexArraySize);
	}
	return is;
}
////////////////////////////////////////////////////////////////////////////////
inline std::ostream& write(std::ostream& os,const VertStream* data,uint count = 1)
{
	for(uint i=0; i<count; ++i)
	{
		const VertStream& vs = data[i];
		const int attributeLayoutSize = vs.attributeLayout.size();
		const int nComponentsSize     = vs.nComponents.size();
		const int drawCommandsSize    = vs.drawCommands.size();
		const int indicesSize         = vs.indices.size();
		const int vertexArraySize     = vs.vertexArray.size();

		os.write((const char*)&attributeLayoutSize,sizeof(int));
		os.write((const char*)&nComponentsSize,sizeof(int));
		os.write((const char*)&drawCommandsSize,sizeof(int));
		os.write((const char*)&indicesSize,sizeof(int));
		os.write((const char*)&vertexArraySize,sizeof(float));

		os.write((const char*)&vs.attributeLayout[0],sizeof(int)*attributeLayoutSize);
		os.write((const char*)&vs.nComponents[0],sizeof(int)*nComponentsSize);
		os.write((const char*)&vs.drawCommands[0],sizeof(VertStream::Command)*drawCommandsSize);
		os.write((const char*)&vs.indices[0],sizeof(int)*indicesSize);
		os.write((const char*)&vs.vertexArray[0],sizeof(float)*vertexArraySize);
	}
	return os;
}
////////////////////////////////////////////////////////////////////////////////
