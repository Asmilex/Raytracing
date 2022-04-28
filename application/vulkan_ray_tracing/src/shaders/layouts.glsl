#ifndef LAYOUTS_GLSL
#define LAYOUTS_GLSL 1

#extension GL_EXT_buffer_reference2 : require
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require
#extension GL_EXT_buffer_reference2 : require

#include "host_device.h"

//layout(location = 0) rayPayloadEXT HitPayload prd;
layout(location = 1) rayPayloadEXT ShadowPayload prdShadow;

layout(set = 0, binding = eTlas)                uniform accelerationStructureEXT topLevelAS;
layout(set = 0, binding = eOutImage, rgba32f)   uniform image2D image;
layout(set = 1, binding = eGlobals)             uniform _GlobalUniforms { GlobalUniforms uni; };
layout(push_constant)                           uniform _PushConstantRay { PushConstantRay pcRay; };

layout(buffer_reference, scalar) buffer Vertices   { Vertex v[]; };             // Posición del objeto
layout(buffer_reference, scalar) buffer Indices    { ivec3 i[]; };              // Indices del triángulo
layout(buffer_reference, scalar) buffer Materials  { WaveFrontMaterial m[]; };  // Array con todos los materiales
layout(buffer_reference, scalar) buffer MatIndices { int i[]; };                // ID del material para cada triángulo

layout(set = 1, binding = eObjDescs, scalar) buffer ObjDesc_ { ObjDesc i[]; } objDesc;
layout(set = 1, binding = eTextures) uniform sampler2D textureSamplers[];

#endif