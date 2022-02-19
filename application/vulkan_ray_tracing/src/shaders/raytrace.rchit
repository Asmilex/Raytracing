#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require
#extension GL_EXT_buffer_reference2 : require

#include "gltf.glsl"
#include "raycommon.glsl"
#include "host_device.h"

hitAttributeEXT vec2 attribs;

layout(location = 0) rayPayloadInEXT hitPayload prd;
layout(location = 1) rayPayloadEXT shadowPayload prdShadow;

layout(set = 0, binding = eTlas) uniform accelerationStructureEXT topLevelAS;
layout(set = 0, binding = ePrimLookup) readonly buffer _InstanceInfo {PrimMeshInfo primInfo[];};

layout(buffer_reference, scalar) readonly buffer Vertices  { vec3 v[]; };
layout(buffer_reference, scalar) readonly buffer Indices   { uint i[]; };
layout(buffer_reference, scalar) readonly buffer Normals   { vec3 n[]; };
layout(buffer_reference, scalar) readonly buffer TexCoords { vec2 t[]; };
layout(buffer_reference, scalar) readonly buffer Materials { GltfShadeMaterial m[]; };

layout(set = 1, binding = eSceneDesc ) readonly buffer SceneDesc_ { SceneDesc sceneDesc; };
layout(set = 1, binding = eTextures) uniform sampler2D texturesMap[]; // all textures


layout (push_constant) uniform _PushConstantRay { PushConstantRay pcRay; };

void main()
{
    // gl_InstanceCustomIndexEXT -> qué objeto fue impactado
    PrimMeshInfo pinfo = primInfo[gl_InstanceCustomIndexEXT];

    // Getting the 'first index' for this mesh (offset of the mesh + offset of the triangle)
    uint indexOffset  = pinfo.indexOffset + (3 * gl_PrimitiveID);
    uint vertexOffset = pinfo.vertexOffset;           // Vertex offset as defined in glTF
    uint matIndex     = max(0, pinfo.materialIndex);  // material of primitive mesh

    Materials gltfMat   = Materials(sceneDesc.materialAddress);
    Vertices  vertices  = Vertices(sceneDesc.vertexAddress);
    Indices   indices   = Indices(sceneDesc.indexAddress);
    Normals   normals   = Normals(sceneDesc.normalAddress);
    TexCoords texCoords = TexCoords(sceneDesc.uvAddress);
    Materials materials = Materials(sceneDesc.materialAddress);

    // Getting the 3 indices of the triangle (local)
    ivec3 triangleIndex = ivec3(indices.i[indexOffset + 0], indices.i[indexOffset + 1], indices.i[indexOffset + 2]);
    triangleIndex += ivec3(vertexOffset);  // (global)

    const vec3 barycentrics = vec3(1.0 - attribs.x - attribs.y, attribs.x, attribs.y);

    // Vertex of the triangle
    const vec3 pos0           = vertices.v[triangleIndex.x];
    const vec3 pos1           = vertices.v[triangleIndex.y];
    const vec3 pos2           = vertices.v[triangleIndex.z];
    const vec3 position       = pos0 * barycentrics.x + pos1 * barycentrics.y + pos2 * barycentrics.z;
    const vec3 world_position = vec3(gl_ObjectToWorldEXT * vec4(position, 1.0));

    // Normal
    const vec3 nrm0         = normals.n[triangleIndex.x];
    const vec3 nrm1         = normals.n[triangleIndex.y];
    const vec3 nrm2         = normals.n[triangleIndex.z];
    vec3       normal       = normalize(nrm0 * barycentrics.x + nrm1 * barycentrics.y + nrm2 * barycentrics.z);
    const vec3 world_normal = normalize(vec3(normal * gl_WorldToObjectEXT));
    const vec3 geom_normal  = normalize(cross(pos1 - pos0, pos2 - pos0));

    // TexCoord
    const vec2 uv0       = texCoords.t[triangleIndex.x];
    const vec2 uv1       = texCoords.t[triangleIndex.y];
    const vec2 uv2       = texCoords.t[triangleIndex.z];
    const vec2 texcoord0 = uv0 * barycentrics.x + uv1 * barycentrics.y + uv2 * barycentrics.z;

    // Vector toward the light
    vec3  L;
    float lightIntensity = pcRay.lightIntensity;
    float lightDistance  = 100000.0;


    if (pcRay.lightType == 0) {         // Point light
        vec3 lDir      = pcRay.lightPosition - world_position;
        lightDistance  = length(lDir);
        lightIntensity = pcRay.lightIntensity / (lightDistance * lightDistance);
        L              = normalize(lDir);
    }
    else {                            // Directional light
        L = normalize(pcRay.lightPosition);
    }

    // Material of the object
    GltfShadeMaterial mat = materials.m[matIndex];

    // Diffuse
    vec3 diffuse = computeDiffuse(mat, L, world_normal);

    if (mat.pbrBaseColorTexture  >= 0) {
        uint txtId = mat.pbrBaseColorTexture;
        diffuse *= texture(texturesMap[nonuniformEXT(txtId)], texcoord0).xyz;
    }

    // Specular
    vec3  specular    = vec3(0);
    float attenuation = 1;

    // Trazar shadow rays solo si la luz es visible desde la superficie
    if (dot(world_normal, L) > 0) {
        float tMin   = 0.001;
        float tMax   = lightDistance;
        vec3  origin = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;
        vec3  rayDir = L;

        uint flags = gl_RayFlagsSkipClosestHitShaderEXT;

        prdShadow.isHit = true;
        prdShadow.seed = prd.seed;

        traceRayEXT(topLevelAS,
            flags,       // rayFlags
            0xFF,        // cullMask
            1,           // sbtRecordOffset
            0,           // sbtRecordStride
            1,           // missIndex
            origin,      // ray origin
            tMin,        // ray min range
            rayDir,      // ray direction
            tMax,        // ray max range
            1            // payload (location = 1)
        );

        prd.seed = prdShadow.seed;

        if (prdShadow.isHit) {
            attenuation = 1.0 / (1.0 + lightDistance);
        }
        else {
            specular = computeSpecular(mat, gl_WorldRayDirectionEXT, L, world_normal);
        }
    }

    // Si el material es reflectivo, disparamos un rayo
    /* WIP al transicionar a glTF, no tenemos especularidad todavía.
    if (mat.illum == 3) {
        vec3 origin = world_position;
        vec3 rayDir = reflect(gl_WorldRayDirectionEXT, world_normal);

        prd.attenuation *= mat.specular;
        prd.done         = 0;
        prd.rayOrigin    = origin;
        prd.rayDir       = rayDir;
    }
    */

    prd.hitValue = vec3(lightIntensity * attenuation * (diffuse + specular));
}
