#pragma once

#include <type_traits>

#include "../common/Hit.h"


template<typename HIT>
requires(std::is_base_of_v<HitDataDistance,HIT>)
const HIT opU(const HIT& h1,const HIT& h2)
{
    if((h2.t < h1.t && h2.t > 0) || h1.t < 0) return h2;
    return h1;
}
