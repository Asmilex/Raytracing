#pragma once

#include "nvvk/appbase_vk.hpp"
#include "nvvk/resourceallocator_vk.hpp"

// ────────────────────────────────────────────────────────────────────────────────

static int const SAMPLE_WIDTH  = 1280;
static int const SAMPLE_HEIGHT = 720;

static const bool BENCHMARK_MODE = true;

// ────────────────────────────────────────────────────────────────────────────────


struct ObjModel
{
    uint32_t     nbIndices{0};
    uint32_t     nbVertices{0};
    nvvk::Buffer vertexBuffer;    // Device buffer of all 'Vertex'
    nvvk::Buffer indexBuffer;     // Device buffer of the indices forming triangles
    nvvk::Buffer matColorBuffer;  // Device buffer of array of 'Wavefront material'
    nvvk::Buffer matIndexBuffer;  // Device buffer of array of 'Wavefront material'
};

struct ObjInstance
{
    nvmath::mat4f transform;    // Matrix of the instance
    uint32_t      objIndex{0};  // Model index reference
};
