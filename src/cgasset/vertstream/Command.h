#pragma once

#include "VertStream.h"


// Taken from OpenGL draw indirect command
struct VertStream::Command
{
    int count;        /* Number of indices */
    int primCount;    /* Number of instances */
    int firstIndex;   /* Byte offset of the first index */
    int baseVertex;   /* Constant to be added to each index */
    int baseInstance; /* Base instance for use in fetching instanced vertex attributes */
    int padding[3];   /* std140 padding */
};
