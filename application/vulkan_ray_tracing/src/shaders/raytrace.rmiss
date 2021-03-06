#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require

#include "globals.glsl"
#include "wavefront.glsl"

/*
    We share the clear color of the rasterization with the ray tracer.

    While the Constants struct contains more members, here we use the fact that
    clear_color is the first member in the struct, and do not even declare the subsequent members.
*/

layout(location = 0) rayPayloadInEXT HitPayload prd;

layout(push_constant) uniform _PushConstantRay
{
    PushConstantRay pcRay;
};

void main()
{
    if (prd.depth == 0) {
        /*
            Primer rayo => hemos impactado con el cielo.
            No hacemos nada, devolvemos simplemente el color del cielo
        */
        prd.hit_value = pcRay.clear_color.xyz;
    }
    else {
        prd.hit_value = 0.02 * pcRay.clear_color.xyz;   // Tiny contribution from environment
    }

    // Como hemos fallado, no hay necesidad de continuar; así que paramos la generación.
    prd.depth = pcRay.max_depth;
}