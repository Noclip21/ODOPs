#pragma once

#include <type_traits>

#include <cgmath>


struct HitDataAttributeBase{};
struct HitDataId: HitDataAttributeBase{ int id = -1; };
struct HitDataBool: HitDataAttributeBase{ bool isHit = false; };
struct HitDataDistance: HitDataAttributeBase{ union{ float t = -1,tmin; }; float tmax; };
struct HitDataNormal: HitDataAttributeBase{ vec3 n; };
struct HitDataUV: HitDataAttributeBase{ vec2 uv; };
struct HitDataUVDifferential: HitDataAttributeBase{ vec2 uvdx,uvdy; };

template<class ... T> 
requires(std::is_base_of_v<HitDataAttributeBase,T> && ...) 
struct HitPayload: T...{};

using Hitb   = HitPayload<HitDataId,HitDataBool>;
using Hitf   = HitPayload<HitDataId,HitDataDistance>;
using Hitn   = HitPayload<HitDataId,HitDataDistance,HitDataNormal>;
using Hituv  = HitPayload<HitDataId,HitDataDistance,HitDataNormal,HitDataUV>;
using Hitduv = HitPayload<HitDataId,HitDataDistance,HitDataNormal,HitDataUV,HitDataUVDifferential>;

template<typename ... T>
constexpr bool is_Hitb = (std::is_base_of_v<HitDataBool,T> && ...);
template<typename ... T>
constexpr bool is_Hitf = (std::is_base_of_v<HitDataDistance,T> && ...);
template<typename ... T>
constexpr bool is_Hitn = (std::is_base_of_v<HitDataNormal,T> && ...);
template<typename ... T>
constexpr bool is_Hituv = (std::is_base_of_v<HitDataUV,T> && ...);
template<typename ... T>
constexpr bool is_Hitduv = (std::is_base_of_v<HitDataUVDifferential,T> && ...);
