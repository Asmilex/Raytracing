#version 460
#extension GL_EXT_ray_tracing : require   // GL_NV_ray_tracing ?
#extension GL_GOOGLE_include_directive : enable

#include "globals.glsl"

layout(location = 1) rayPayloadInEXT shadowPayload prd;   //rayPayloadInNV?

void main()
{
  prd.isHit = false;
}
