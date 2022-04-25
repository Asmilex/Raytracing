#ifndef LAYOUTS_GLSL
#define LAYOUTS_GLSL 1

#include "host_device.h"

layout(location = 0) rayPayloadEXT HitPayload prd;

/*
    The buffer of camera uses binding = 0 (eGlobalsUniform) as described in host_device.h.
    The set = 1 comes from the fact that it is the second descriptor set passed to
    pipelineLayoutCreateInfo.pSetLayouts in HelloVulkan::createRtPipeline().
*/

layout(set = 0, binding = eTlas)                uniform accelerationStructureEXT topLevelAS;
layout(set = 0, binding = eOutImage, rgba32f)   uniform image2D image;
layout(set = 1, binding = eGlobals)             uniform _GlobalUniforms { GlobalUniforms uni; };
layout(push_constant)                           uniform _PushConstantRay { PushConstantRay pcRay; };

#endif