#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require
#extension GL_EXT_buffer_reference2 : require

#include "globals.glsl"
#include "wavefront.glsl"
#include "sampling.glsl"

hitAttributeEXT vec3 attribs;

layout(location = 0) rayPayloadInEXT HitPayload prd;
layout(location = 1) rayPayloadEXT ShadowPayload prdShadow;

layout(set = 0, binding = eTlas) uniform accelerationStructureEXT topLevelAS;

layout(buffer_reference, scalar) buffer Vertices   { Vertex v[]; };             // Posición del objeto
layout(buffer_reference, scalar) buffer Indices    { ivec3 i[]; };              // Indices del triángulo
layout(buffer_reference, scalar) buffer Materials  { WaveFrontMaterial m[]; };  // Array con todos los materiales
layout(buffer_reference, scalar) buffer MatIndices { int i[]; };                // ID del material para cada triángulo

layout(set = 1, binding = eObjDescs, scalar) buffer ObjDesc_ { ObjDesc i[]; } objDesc;
layout(set = 1, binding = eTextures) uniform sampler2D textureSamplers[];


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
    const vec3 pos            = v0.pos * barycentrics.x + v1.pos * barycentrics.y + v2.pos * barycentrics.z;
    const vec3 world_position = vec3(gl_ObjectToWorldEXT * vec4(pos, 1.0));

    // Computing the normal at hit position
    const vec3 normal       = v0.nrm * barycentrics.x + v1.nrm * barycentrics.y + v2.nrm * barycentrics.z;
    const vec3 world_normal = normalize(vec3(normal * gl_WorldToObjectEXT));

    // Material of the object
    int               matIdx    = matIndices.i[gl_PrimitiveID];
    WaveFrontMaterial mat       = materials.m[matIdx];
    vec3              emittance = mat.emission;

    // Pick a random direction from here and keep going
    vec3 tangent, bitangent;
    create_coordinate_system(world_normal, tangent, bitangent);
    vec3 ray_origin = world_position;
    vec3 ray_dir    = sampling_hemisphere(prd.seed, tangent, bitangent, world_normal);


    // Probability of the new ray (cosine distributed)
    const float p = 1 / M_PI;

    // Compute the BRDF for this ray (assuming Lambertian reflection)
    float cos_theta = dot(ray_dir, world_normal);
    vec3 diffuse = mat.diffuse;

    if (mat.textureId >= 0) {
        uint txtId = mat.textureId + objDesc.i[gl_InstanceCustomIndexEXT].txtOffset;
        vec2 texCoord = v0.texCoord * barycentrics.x + v1.texCoord * barycentrics.y + v2.texCoord * barycentrics.z;
        diffuse *= texture(textureSamplers[nonuniformEXT(txtId)], texCoord).xyz;
    }

    vec3 BRDF = diffuse / M_PI;

    prd.ray_origin = ray_origin;
    prd.ray_dir    = ray_dir;
    //              vvvv h5ay que cambiar eso!!!
    prd.hit_value  = 0.11 * mat.ambient;
    prd.weight    = BRDF * cos_theta / p;

/* Código viejo antes de pasar a path tracing

    // Vector toward the light
    vec3 L;
    float light_intensity = pcRay.light_intensity;
    float lightDistance = 100000.0;

    if (pcRay.light_type == 0) {         // Point light
        vec3 lDir      = pcRay.light_position - worldPos;
        lightDistance  = length(lDir);
        light_intensity = pcRay.light_intensity / (lightDistance * lightDistance);
        L              = normalize(lDir);
    }
    else {                            // Directional light
        L = normalize(pcRay.light_position);
    }

    // Material of the object
    int               matIdx = matIndices.i[gl_PrimitiveID];
    WaveFrontMaterial mat    = materials.m[matIdx];

    // Diffuse
    vec3 diffuse = computeDiffuse(mat, L, normal);

    if (mat.textureId >= 0) {
        uint txtId = mat.textureId + objDesc.i[gl_InstanceCustomIndexEXT].txtOffset;
        vec2 texCoord = v0.texCoord * barycentrics.x + v1.texCoord * barycentrics.y + v2.texCoord * barycentrics.z;
        diffuse *= texture(textureSamplers[nonuniformEXT(txtId)], texCoord).xyz;
    }

    // Specular
    vec3 specular = vec3(0);
    float attenuation = 1;

    // Trazar shadow rays solo si la luz es visible desde la superficie
    if (dot(normal, L) > 0) {
        float tMin = 0.001;
        float tMax = lightDistance;

        vec3 origin = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;
        vec3 ray_dir = L;

        uint flags = gl_RayFlagsSkipClosestHitShaderEXT;
        prdShadow.is_hit = true;
        prdShadow.seed = prd.seed;

        traceRayEXT(topLevelAS,
            flags,       // rayFlags
            0xFF,        // cullMask
            1,           // sbtRecordOffset
            0,           // sbtRecordStride
            1,           // missIndex
            origin,      // ray origin
            tMin,        // ray min range
            ray_dir,      // ray direction
            tMax,        // ray max range
            1            // payload (location = 1)
        );

        prd.seed = prdShadow.seed;

        if (prdShadow.is_hit) {
            attenuation = 1.0 / (1.0 + lightDistance);
        }
        else {
            specular = computeSpecular(mat, gl_WorldRayDirectionEXT, L, normal);
        }
    }

    // Si el material es reflectivo, disparamos un rayo
    if (mat.illum == 3) {
        vec3 origin = worldPos;
        vec3 ray_dir = reflect(gl_WorldRayDirectionEXT, normal);

        prd.attenuation *= mat.specular;
        prd.done         = 0;
        prd.ray_origin    = origin;
        prd.ray_dir       = ray_dir;
    }

    prd.hit_value = vec3(light_intensity * attenuation * (diffuse + specular));
*/
}
