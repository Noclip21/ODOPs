#pragma once

#include <cstdint>

using uint   = uint32_t;
using uint64 = uint64_t;
class Thread;
template<uint64>       struct ComputeInput;
template<typename ...> struct ComputeTask;
template<typename ...> struct Compute;
