#pragma once

#include <type_traits>

#include <cgmath>

#include "../shape/ShapeBase.h"


struct PrimitiveArrayBase {};
struct PrimitiveArrayBaseIsIndexed {};
struct PrimitiveArrayBaseIsKDOP {};
struct PrimitiveArrayBaseIsBezier {};


template<typename T>
struct PrimitiveArrayDataBase: PrimitiveArrayBase { using PrimType = T; T* data; uint count; };
template<typename T>
struct PrimitiveArrayDataIndexed: PrimitiveArrayBase,PrimitiveArrayBaseIsIndexed { using IndexType = T; T* indices; };
struct PrimitiveArrayDataTriangle: PrimitiveArrayBase { bool backface; };
template<uint N>
struct PrimitiveArrayDataBezier: PrimitiveArrayBase,PrimitiveArrayBaseIsBezier { Polynomial<float,N> pr; };


template<class ... T>
requires(std::is_base_of_v<PrimitiveArrayBase,T> && ...) 
struct PrimitiveArrayPayload: T...{};


using VertexArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<vec3>>;
using AABBArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<AABB>>;
using OBBArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<OBB>>;
template<uint K>
using KDOPArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<KDOP<K>>,PrimitiveArrayBaseIsKDOP>;
template<uint N>
using BezierArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<Bezier>,PrimitiveArrayDataBezier<N>>;
using SphereArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<Sphere>>;
using TriangleArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<Triangle>,PrimitiveArrayDataTriangle>;
using TriangleIndicesArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<vec3>,PrimitiveArrayDataTriangle,PrimitiveArrayDataIndexed<ivec3>>;

using IndexedVertexArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<vec3>,PrimitiveArrayDataIndexed<int>>;
using IndexedAABBArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<AABB>,PrimitiveArrayDataIndexed<int>>;
using IndexedOBBArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<OBB>,PrimitiveArrayDataIndexed<int>>;
template<uint K>
using IndexedKDOPArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<KDOP<K>>,PrimitiveArrayBaseIsKDOP,PrimitiveArrayDataIndexed<int>>;
template<uint N>
using IndexedBezierArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<Bezier>,PrimitiveArrayDataBezier<N>,PrimitiveArrayDataIndexed<int>>;
using IndexedSphereArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<Sphere>,PrimitiveArrayDataIndexed<int>>;
using IndexedTriangleArray = PrimitiveArrayPayload<PrimitiveArrayDataBase<Triangle>,PrimitiveArrayDataTriangle,PrimitiveArrayDataIndexed<int>>;



template<typename LAYOUT>
constexpr bool is_PrimitiveArray = std::is_base_of_v<PrimitiveArrayBase,LAYOUT>;
template<typename LAYOUT>
constexpr bool is_PrimitiveArrayIndexed = std::is_base_of_v<PrimitiveArrayBaseIsIndexed,LAYOUT>;


template<typename LAYOUT>
constexpr bool is_VertexArray = std::is_same_v<VertexArray,LAYOUT>;
template<typename LAYOUT>
constexpr bool is_AABBArray = std::is_same_v<AABBArray,LAYOUT>;
template<typename LAYOUT>
constexpr bool is_OBBArray = std::is_same_v<OBBArray,LAYOUT>;
template<typename LAYOUT>
constexpr bool is_KDOPArray = std::is_base_of_v<PrimitiveArrayBaseIsKDOP,LAYOUT>;
template<typename LAYOUT>
constexpr bool is_BezierArray = std::is_same_v<PrimitiveArrayBaseIsBezier,LAYOUT>;
template<typename LAYOUT>
constexpr bool is_SphereArray = std::is_same_v<SphereArray,LAYOUT>;
template<typename LAYOUT>
constexpr bool is_TriangleArray = std::is_same_v<TriangleArray,LAYOUT>;
template<typename LAYOUT>
constexpr bool is_TriangleIndicesArray = std::is_same_v<TriangleIndicesArray,LAYOUT>;

template<typename LAYOUT>
constexpr bool is_IndexedVertexArray = is_PrimitiveArrayIndexed<LAYOUT> && is_VertexArray<LAYOUT>;
template<typename LAYOUT>
constexpr bool is_IndexedAABBArray = is_PrimitiveArrayIndexed<LAYOUT> && is_AABBArray<LAYOUT>;
template<typename LAYOUT>
constexpr bool is_IndexedOBBArray = is_PrimitiveArrayIndexed<LAYOUT> && is_OBBArray<LAYOUT>;
template<typename LAYOUT>
constexpr bool is_IndexedKDOPArray = is_PrimitiveArrayIndexed<LAYOUT> && is_KDOPArray<LAYOUT>;
template<typename LAYOUT>
constexpr bool is_IndexedBezierArray = is_PrimitiveArrayIndexed<LAYOUT> && is_BezierArray<LAYOUT>;
template<typename LAYOUT>
constexpr bool is_IndexedSphereArray = is_PrimitiveArrayIndexed<LAYOUT> && is_SphereArray<LAYOUT>;
template<typename LAYOUT>
constexpr bool is_IndexedTriangleArray = is_PrimitiveArrayIndexed<LAYOUT> && is_TriangleArray<LAYOUT>;

template<typename LAYOUT>
using PrimitiveArray_makeIndexed_t = 
	std::conditional_t<is_PrimitiveArrayIndexed<LAYOUT>,
		LAYOUT,
		PrimitiveArrayPayload<LAYOUT,PrimitiveArrayDataIndexed<int>>>;
