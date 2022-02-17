#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require
#extension GL_EXT_buffer_reference2 : require

#include "raycommon.glsl"
#include "wavefront.glsl"

hitAttributeEXT vec3 attribs;

layout(location = 0) rayPayloadInEXT hitPayload prd;

layout(buffer_reference, scalar) buffer Vertices { Vertex v[]; };              // Posición del objeto
layout(buffer_reference, scalar) buffer Indices { ivec3 i[]; };                // Indices del triángulo
layout(buffer_reference, scalar) buffer Materials { WaveFrontMaterial m[]; };  // Array con todos los materiales
layout(buffer_reference, scalar) buffer MatIndices { int i[]; };                // ID del material para cada triángulo
layout(set = 1, binding = eObjDescs, scalar) buffer ObjDesc_ { ObjDesc i[]; } objDesc;

layout (push_constant) uniform _PushConstantRay { PushConstantRay pcRay; };

void main()
{
    // gl_InstanceCustomIndexEXT -> qué objeto fue impactado
    ObjDesc    objResource = objDesc.i[gl_InstanceCustomIndexEXT];
    MatIndices matIndices  = MatIndices(objResource.materialIndexAddress);
    Materials  materials   = Materials(objResource.materialAddress);
    Indices    indices     = Indices(objResource.indexAddress);
    Vertices   vertices    = Vertices(objResource.vertexAddress);

    // Indices of the triangle
    ivec3 ind = indices.i[gl_PrimitiveID];

    // Vertex of the triangle
    Vertex v0 = vertices.v[ind.x];
    Vertex v1 = vertices.v[ind.y];
    Vertex v2 = vertices.v[ind.z];

    const vec3 barycentrics = vec3(1.0 - attribs.x - attribs.y, attribs.x, attribs.y);

    // Computing the coordinates of the hit position
    const vec3 pos      = v0.pos * barycentrics.x + v1.pos * barycentrics.y + v2.pos * barycentrics.z;
    const vec3 worldPos = vec3(gl_ObjectToWorldEXT * vec4(pos, 1.0));

    // Computing the normal at hit position
    const vec3 nrm      = v0.nrm * barycentrics.x + v1.nrm * barycentrics.y + v2.nrm * barycentrics.z;
    const vec3 worldNrm = normalize(vec3(nrm * gl_WorldToObjectEXT));

    // Vector toward the light
    vec3 L;
    float lightIntensity = pcRay.lightIntensity;
    float lightDistance = 100000.0;

    if (pcRay.lightType == 0) {         // Point light
        vec3 lDir      = pcRay.lightPosition - worldPos;
        lightDistance  = length(lDir);
        lightIntensity = pcRay.lightIntensity / (lightDistance * lightDistance);
        L              = normalize(lDir);
    }
    else {                            // Directional light
        L = normalize(pcRay.lightPosition);
    }

    prd.hitValue = L;
}
