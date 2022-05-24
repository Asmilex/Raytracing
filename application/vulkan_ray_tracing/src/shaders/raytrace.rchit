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


    // ────────────────────────────────────────────────────── SIGUIENTE DIRECCION ─────

    prd.ray_origin = world_position;

    // Rendering equation:
    //    L_o = Le + (BSDF(omega_o <- omega_i) * L_i(omega_o) * cos_theta) / pdf(omega)
    // donde
    //    weight = (BSDF(omega_o <- omega_i) * cos_theta) / pdf(omega)
    //    L_i(omega_o) se acumula automáticamente en el raygen.

    vec3 hit_value  = mat.emission;   // Componente Le

    vec3  weight    = vec3(1);
    vec3  BSDF      = vec3(1);
    float cos_theta = 1;
    float pdf       = 1;

    vec3 ray_dir;

    if(mat.illum == 3) {            // Materiales reflectantes (sin Fresnel)
        // Se comportan como espejos => refleja y fuera
        ray_dir = reflect(gl_WorldRayDirectionEXT, normal);
        BSDF    = mat.specular;
    }
    else if (mat.illum == 2 || mat.illum == 4) {      // Materiales difusos y glossy.
        // Si es glossy, entonces Ns == shininess > 10
        // Ten en cuenta la especularidad y la difusión.
        // Una buena medida sería sacar un aleatorio con probabilidad
        //   |difusa| / (|difusa| + |especular|)
        // Y coger la BRDF correspondiente a ese aleatorio.
        // Falta por ver cómo modifica Ns
        //
        // También debería tener en cuenta el índice de refracción (Ni == ior)

        float prob_diffuse = length(mat.diffuse) / (length(mat.diffuse) + length(mat.specular));

        if (prob_diffuse > 0.98 || prob_diffuse > rnd(prd.seed)) { // Componente difusa
            // Escoger una dirección aleatoria y continuar el camino.
            vec3 tangent, bitangent;
            create_coordinate_system(world_normal, tangent, bitangent);

            if (COSINE_HEMISPHERE_SAMPLING == 1) {   // Muestreo por importancia de la esfera
                ray_dir   = cosine_sample_hemisphere(prd.seed, tangent, bitangent, world_normal);
                cos_theta = dot(ray_dir, world_normal);
                pdf       = cos_theta/M_PI;
            }
            else {                                   // Hemisphere sampling
                ray_dir   = sampling_hemisphere(prd.seed, tangent, bitangent, world_normal);
                cos_theta = dot(ray_dir, world_normal);
                pdf       = 1 / M_PI;
            }

            // Aplicar BRDF de materiales puramente difusos lambertianos.
            vec3 diffuse = mat.diffuse;

            if (mat.textureId >= 0) {
                uint txtId    = mat.textureId + objDesc.i[gl_InstanceCustomIndexEXT].txtOffset;
                vec2 texCoord = v0.texCoord * barycentrics.x + v1.texCoord * barycentrics.y + v2.texCoord * barycentrics.z;

                diffuse *= texture(textureSamplers[nonuniformEXT(txtId)], texCoord).xyz;
            }

            BSDF = prob_diffuse * diffuse / M_PI;
        }
        else {
            // Realmente, esto es un modelo de material metálico usando el coeficiente de especularidad.
            // Debería remodelarlo en el futuro para seguir blin-phong propiamente.
            ray_dir = reflect(gl_WorldRayDirectionEXT, normal);
            ray_dir = ray_dir + (1024 - mat.shininess) / 990 * random_in_unit_sphere(prd.seed);
            BSDF    = mat.specular * (1.0 - prob_diffuse);
        }
    }
    else if (mat.illum == 5) {      // Materiales reflectantes (con Fresnel)
        ray_dir = reflect(gl_WorldRayDirectionEXT, normal);
        BSDF    = mat.specular;
    }
    else if (mat.illum == 6 || mat.illum == 7) {      // Materiales refractantes (sin Fresnel)
        // Estilo In One Weekend
        bool  front_facing   = dot(-gl_WorldRayDirectionEXT, normal) > 0.0;
        vec3  forward_normal = front_facing ? normal : -normal;
        float eta            = front_facing? (ETA_AIR / mat.ior) : mat.ior;

        vec3  unit_dir  = normalize(gl_WorldRayDirectionEXT);
              cos_theta = min(dot(-unit_dir, forward_normal), 1.0);
        float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = eta * sin_theta > 1.0;
        bool reflect_condition = mat.illum == 6
            ? cannot_refract
            : cannot_refract || reflectance(cos_theta, eta) > rnd(prd.seed);

        BSDF = vec3(0.98);

        if (reflect_condition) {
            ray_dir = reflect(gl_WorldRayDirectionEXT, forward_normal);
        }
        else {
            BSDF    = mat.transmittance;
            ray_dir = refract(gl_WorldRayDirectionEXT, forward_normal, eta);
        }
    }


    // ──────────────────────────────────────────────────── CONTRIBUCION DE LUCES ─────

    vec3 L;
    float light_intensity = pcRay.light_intensity;
    float light_distance = 100000.0;

    // BSDF vale lo mismo que para el material
    float pdf_light       = 1;
    float cos_theta_light = 1;

    if (pcRay.light_type == 0) {         // Point light
        vec3 L_dir = pcRay.light_position - world_position;

        light_distance   = length(L_dir);
        light_intensity = pcRay.light_intensity / (light_distance * light_distance);
        L               = normalize(L_dir);
        // Solo tenemos un punto => pdf light = 1, cos_theta light = 1.
        cos_theta_light = dot(L, world_normal);
    }
    else if (pcRay.light_type == 1) {                            // Directional light
        L = normalize(pcRay.light_position);
        cos_theta_light = dot(L, world_normal);
    }

    if (dot(normal, L) > 0) {
        float tMin = 0.001;
        float tMax = light_distance;

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
        float attenuation = 1;

        if (!prdShadow.is_hit) {
            hit_value = hit_value + light_intensity*BSDF*cos_theta_light / pdf_light;
        }
    }


    // ────────────────────────────────────────────────────────────────── RETORNO ─────

    weight = BSDF * cos_theta / pdf;

    prd.ray_dir   = ray_dir;
    prd.hit_value = hit_value;
    prd.weight    = weight;
}