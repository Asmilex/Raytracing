#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require
#extension GL_EXT_buffer_reference2 : require


#include "globals.glsl"
#include "layouts.glsl"
#include "wavefront.glsl"
#include "sampling.glsl"

hitAttributeEXT vec3 attribs;
layout(location = 0) rayPayloadInEXT HitPayload prd;


VisibilityContribution luz_directa() {
    // https://github.com/nvpro-samples/vk_raytrace/blob/master/shaders/pathtrace.glsl
    // MIRA ESTE ARCHIVO!!! QUE SE PUEDE HACER. Es simplemente tener en cuenta lo correcto,
    // y hacer el ray trace para ver si está ocluido o no.
    // MUY WIP

    VisibilityContribution contribucion;
    contribucion.radiance = vec3(0);
    contribucion.visible  = false;


    // Vector toward the light
    /*
    vec3 L;
    float light_intensity = pcRay.light_intensity;
    float lightDistance = 100000.0;

    if (pcRay.light_type == 0) {         // Point light
        vec3 lDir = pcRay.light_position - world_position;

        lightDistance   = length(lDir);
        light_intensity = pcRay.light_intensity / (lightDistance * lightDistance);
        L               = normalize(lDir);
    }
    else {                            // Directional light
        L = normalize(pcRay.light_position);
    }

    // Diffuse
    vec3 diffuse = compute_diffuse_lambertian(mat, L, normal);

    if (mat.textureId >= 0) {
        uint txtId = mat.textureId + objDesc.i[gl_InstanceCustomIndexEXT].txtOffset;
        vec2 texCoord = v0.texCoord * barycentrics.x + v1.texCoord * barycentrics.y + v2.texCoord * barycentrics.z;
        diffuse *= texture(textureSamplers[nonuniformEXT(txtId)], texCoord).xyz;
    }

    // Specular
    vec3 specular = vec3(0);
    float attenuation = 1;

    // Trazar shadow rays solo si la luz es visible desde la superficie
    //if (dot(normal, L) > 0) {
    if (contribucion_luces.visible) {
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
            specular = compute_diffuse_lambertian(mat, L, normal);
        }
    }
    */

    return contribucion;
}

void main()
{
// ────────────────── CALCULAR ELEMENTOS RELACIONADOS CON EL OBJETO IMPACTADO ─────

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


// ──────────────────────────────────────────────────── CONTRIBUCION DE LUCES ─────

    VisibilityContribution contribucion_luces = luz_directa();

// ────────────────────────────────────────────────────── SIGUIENTE DIRECCION ─────

    prd.ray_origin = world_position;
    prd.hit_value  = mat.emission;   // Componente Le

    if(mat.illum == 3) {            // Materiales reflectantes (sin Fresnel)
        // Se comportan como espejos => refleja y fuera
        prd.ray_dir = reflect(gl_WorldRayDirectionEXT, normal);
        prd.weight  = mat.specular;
    }
    else if (mat.illum == 2) {      // Materiales difusos y glossy.
        // Si es glossy, entonces Ns == shininess > 10
        // Ten en cuenta la especularidad y la difusión.
        // Una buena medida sería sacar un aleatorio con probabilidad
        //   |difusa| / (|difusa| + |especular|)
        // Y coger la BRDF correspondiente a ese aleatorio.
        // Falta por ver cómo modifica Ns
        //
        // También debería tener en cuenta el índice de refracción (Ni == ior)

        float prob_diffuse = length(mat.diffuse) / (length(mat.diffuse) + length(mat.specular));

        if (prob_diffuse > 0.98 || prob_diffuse > rnd(prd.seed)) {
            // Pick a random direction from here and keep going
            vec3 tangent, bitangent;
            create_coordinate_system(world_normal, tangent, bitangent);

            vec3 ray_dir;
            float cos_theta;
            float pdf;

            if (COSINE_HEMISPHERE_SAMPLING) {
                float prob;
                ray_dir = cosine_sample_hemisphere(prd.seed, tangent, bitangent, world_normal, prob);
                cos_theta = dot(ray_dir, world_normal);
                pdf = prob / M_PI;
            }
            else {
                ray_dir = sampling_hemisphere(prd.seed, tangent, bitangent, world_normal);
                cos_theta = dot(ray_dir, world_normal);
                pdf = cos_theta / M_PI;
            }

            // Aplicar BRDF de materiales puramente difusos lambertianos.
            vec3 diffuse = mat.diffuse;

            if (mat.textureId >= 0) {
                uint txtId    = mat.textureId + objDesc.i[gl_InstanceCustomIndexEXT].txtOffset;
                vec2 texCoord = v0.texCoord * barycentrics.x + v1.texCoord * barycentrics.y + v2.texCoord * barycentrics.z;

                diffuse *= texture(textureSamplers[nonuniformEXT(txtId)], texCoord).xyz;
            }

            const vec3 BRDF = diffuse / M_PI;

            prd.ray_dir = ray_dir;
            prd.weight = (prob_diffuse * BRDF * cos_theta) / pdf;
        }
        else {
            // Realmente, esto es un modelo de material metálico usando el coeficiente de especularidad.
            // Debería remodelarlo en el futuro para seguir blin-phong propiamente.
            prd.ray_dir = reflect(gl_WorldRayDirectionEXT, normal);
            prd.ray_dir = prd.ray_dir + (1024 - mat.shininess) / 990 * random_in_unit_sphere(prd.seed);
            prd.weight  = mat.specular * (1.0 - prob_diffuse);
        }
    }
    else if (mat.illum == 4) {      // Transparencia: Glass on, ray traced reflections

    }
    else if (mat.illum == 5) {      // Materiales reflectantes (con Fresnel)
        prd.ray_dir = reflect(gl_WorldRayDirectionEXT, normal);
        prd.weight  = mat.specular;
    }
    else if (mat.illum == 6) {      // Materiales refractantes (sin Fresnel)

    }
    else if (mat.illum == 7) {      // Materiales refractantes (con Fresnel)

    }
}