#pragma once

#include <type_traits>

#include <cgmath>


struct RayDataAttributeBase{};
struct RayDataBase: RayDataAttributeBase{ vec3 o,d; };
struct RayDataCone: RayDataAttributeBase{ float w,a; };
struct RayDataDifferential: RayDataAttributeBase{ vec3 odx,ody,ddx,ddy; };

template<class ... T>
requires(std::is_base_of_v<RayDataAttributeBase,T> && ...) 
struct RayPayload: T...{};

using Ray  = RayPayload<RayDataBase>;
using Rayc = RayPayload<RayDataBase,RayDataCone>;
using Rayd = RayPayload<RayDataBase,RayDataDifferential>;

template<typename ... T>
constexpr bool is_Ray = (std::is_base_of_v<RayDataBase,T> && ...);
template<typename ... T>
constexpr bool is_Rayc = (std::is_base_of_v<RayDataCone,T> && ...);
template<typename ... T>
constexpr bool is_Rayd = (std::is_base_of_v<RayDataDifferential,T> && ...);
