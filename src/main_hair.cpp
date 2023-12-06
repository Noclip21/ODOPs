#include <iostream>
#include <unordered_map>

#include <cgmath>
#include <cgasset>
#include <cgtrace>

#include "Mesh.h"

////////////////////////////////////////////////////////////////////////////////
struct BevelSegment: Mesh
{
	BevelSegment(
		const uint nVerts,
		const vec3& p0,const vec3& p1,
		const vec3& d0,const vec3& d1,
		const float r0,const float r1)
	{
		verts.resize(2*nVerts);
		const mat3 m0 = (abs(dot(d0,vec3(0,1,0))) > 0.99)?mat3():lookRotation(d0);
		const mat3 m1 = (abs(dot(d1,vec3(0,1,0))) > 0.99)?mat3():lookRotation(d1);
		for(uint i=0; i<nVerts; ++i)
		{
			const float a = 2*M_PI*float(i)/nVerts;
			verts[i].x = r0*cos(a); verts[nVerts+i].x = r1*cos(a);
			verts[i].y = r0*sin(a); verts[nVerts+i].y = r1*sin(a);
			verts[i].z = verts[nVerts+i].z = 0;
			verts[i]   = p0+m0*verts[i];
			verts[nVerts+i] = p1+m1*verts[nVerts+i];
		}
		indices.resize(2*nVerts);
		for(uint i=0; i<nVerts; ++i)
		{
			const uint j = (i+1)%nVerts;
			indices[i]        = {i,i+nVerts,j};
			indices[nVerts+i] = {j,i+nVerts,j+nVerts};
		}
	}
};
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
	Scene scene("./res/bunny_fur.obj");

	// Create a map of all connected lines
	std::unordered_map<int,int> lineMap;
	std::unordered_map<int,int> isNotRootMap;
	for(auto& m: scene.meshes)
	{
		for(uint i=0; i<m.indices.size(); i+=2)
		{
			int id0 = m.indices[i];
			int id1 = m.indices[i+1];
			lineMap[id0] = id1;
			isNotRootMap[id1] = 1;
		}
	}

	// Find all connected lines
	std::vector<std::vector<int>> lines;
	for(auto it = lineMap.begin(); it != lineMap.end(); ++it)
	{
		if(isNotRootMap[it->first] == 1) continue;
		auto it2 = it;
		std::vector<int> line;
		while(it2 != lineMap.end())
		{
			line.push_back(it2->first);
			auto v = it2->second;
			it2 = lineMap.find(v);
			if(it2 == lineMap.end())
				line.push_back(v);
		}
		if(line.size() > 1) lines.push_back(line);
	}

	for(auto line: lines)
	{
		std::vector<vec3> lineVerts(line.size());
		for(uint i=0; i<line.size(); ++i)
			lineVerts[i] = *(const vec3*)(scene.vertices[line[i]].position);

		// Fit bezier segments to polyline
		std::vector<Bezier> bezier;
		for(uint i=0; i+4<lineVerts.size(); i += 3)
			bezier.push_back(Bezier(&lineVerts[i],(i+4>=line.size())?line.size()-i:4));
		if(bezier.size() > 0) bezier[bezier.size()-1].d = lineVerts[line.size()-1];

		// Fix bezier segments continuity
		for(uint i=1; i<bezier.size(); ++i)
		{
			bezier[i].a = bezier[i-1].d;
			bezier[i].b = 2*bezier[i-1].d-bezier[i-1].c;
		}

		// Convert beziers to mesh
		for(uint i=0; i<bezier.size(); ++i)
		{
			auto b = bezier[i];
			const float
				r0 = 0.001*(1-(float(i)/bezier.size())),
				r1 = 0.001*(1-(float(i+1)/bezier.size()));
			static const uint nSegments = 1;
			for(uint j=0; j<nSegments; ++j)
			{
				const float 
					t0 = float(j)/nSegments,
					t1 = float(j+1)/nSegments;
				const vec3 
					p0 = b.interpolate(t0), p1 = b.interpolate(t1),
					d0 = normalize(b.interpolatedt(t0)), d1 = normalize(b.interpolatedt(t1));
				BevelSegment bs(3,p0,p1,d0,d1,mix(r0,r1,t0),mix(r0,r1,t1));
				std::cout << bs << std::endl;
			}
		}
	}

	return 0;
}
