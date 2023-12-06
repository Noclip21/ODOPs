#pragma once

#include <iostream>
#include <vector>

#include <cgmath>


////////////////////////////////////////////////////////////////////////////////
struct Mesh
{
	std::vector<vec3> verts;
	std::vector<std::vector<uint>> indices;

	static uint offset;

	friend const Mesh operator * (const mat4& m4,Mesh m)
	{
		for(vec3& v: m.verts)
			v = (m4*vec4(v,1)).xyz;
		return m;
	};

	friend std::ostream& operator << (std::ostream& out,const Mesh& m)
	{
		uint newOffset = 0;
		for(const vec3& v: m.verts)
		{
			std::cout << "v " << v.x << " " << v.y << " " << v.z << std::endl;
			newOffset++;
		}
		for(const std::vector<uint>& fi: m.indices)
		{
			std::cout << "f";
			for(const uint& i: fi)
			{
				std::cout << " " << i+offset+1;
			}
			std::cout << std::endl;
		}
		offset += newOffset;
		return out;
	}
};
uint Mesh::offset = 0;
////////////////////////////////////////////////////////////////////////////////
static const Mesh BOX = {
	.verts = {
		{ 1, 1, 0},
		{ 1, 0, 0},
		{ 1, 1, 1},
		{ 1, 0, 1},
		{ 0, 1, 0},
		{ 0, 0, 0},
		{ 0, 1, 1},
		{ 0, 0, 1}
	},
	.indices = {
		{0,4,6,2},
		{3,2,6,7},
		{7,6,4,5},
		{5,1,3,7},
		{1,0,2,3},
		{5,4,0,1}
	}
};
////////////////////////////////////////////////////////////////////////////////
static const Mesh BOXWIRE = {
	.verts = {
		{ 1, 1, 0},
		{ 1, 0, 0},
		{ 1, 1, 1},
		{ 1, 0, 1},
		{ 0, 1, 0},
		{ 0, 0, 0},
		{ 0, 1, 1},
		{ 0, 0, 1}
	},
	.indices = {
		{0,4},{4,6},{6,2},{2,0},
		{3,2},{2,6},{6,7},{7,3},
		{7,6},{6,4},{4,5},{5,7},
		{5,1},{1,3},{3,7},{7,5},
		{1,0},{0,2},{2,3},{3,1},
		{5,4},{4,0},{0,1},{1,5}
	}
};
////////////////////////////////////////////////////////////////////////////////
static const Mesh BOXDIRECTIONS = {
	.verts = {
		{0,0,0},
		{1,0,0},{0,1,0},{0,0,1},
		{-1,0,0},{0,-1,0},{0,0,-1},
	},
	.indices = {
		{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},
	}
};
////////////////////////////////////////////////////////////////////////////////
struct Point: public Mesh
{
	Point(const vec3& p)
	{ verts = {p}; indices = {{0}}; }
};
////////////////////////////////////////////////////////////////////////////////
struct Line: public Mesh
{
	Line(const vec3& a,const vec3& b)
	{ verts = {a,b}; indices = {{0,1}}; }
};
////////////////////////////////////////////////////////////////////////////////
struct Direction: public Line
{
	Direction(const vec3& d):Line(0,d){}
};
////////////////////////////////////////////////////////////////////////////////
//struct Triangle: public Mesh
//{
//	Triangle(const vec3& p1,const vec3& p2,const vec3& p3)
//	{ verts = {p1,p2,p3}; indices = {{0,1,2}}; };
//};
////////////////////////////////////////////////////////////////////////////////
