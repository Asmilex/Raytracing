#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require

#include "raycommon.glsl"
#include "wavefront.glsl"

/*
    We share the clear color of the rasterization with the ray tracer.

    While the Constants struct contains more members, here we use the fact that
    clearColor is the first member in the struct, and do not even declare the subsequent members.
*/

layout(location = 0) rayPayloadInEXT hitPayload prd;

layout(push_constant) uniform _PushConstantRay
{
    PushConstantRay pcRay;
};

void main()
{
    if (prd.depth == 0) {
        prd.hitValue = pcRay.clearColor.xyz * 0.8;
    }
    else {
        prd.hitValue = vec3(0.3);   // Tiny contribution from environment
    }

    // If nothing is hit, there's no point in continuing the ray.
    prd.depth = pcRay.maxDepth;
}