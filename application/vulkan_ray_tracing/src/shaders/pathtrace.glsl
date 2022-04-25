#include "globals.glsl"
#include "layouts.glsl"

layout(location = 0) rayPayloadEXT HitPayload prd;

vec3 sample_pixel(
    ivec2 image_coords,
    ivec2 image_res
) {
    vec3 hit_value = vec3(0);

    float r1 = rnd(prd.seed);
    float r2 = rnd(prd.seed);

    // Subpixel jitter: send the ray through a different position inside the pixel each time to provide antialiasing
    vec2 subpixel_jitter = pcRay.frame == 0
        ? vec2(0.5f, 0.5f)
        : vec2(r1, r2);

    const vec2 pixelCenter = vec2(image_coords.xy) + subpixel_jitter;
    const vec2 inUV        = pixelCenter/vec2(image_res.xy);
    vec2  d                = inUV * 2.0 - 1.0;

    vec4 origin    = uni.viewInverse * vec4(0, 0, 0, 1);
    vec4 target    = uni.projInverse * vec4(d.x, d.y, 1, 1);
    vec4 direction = uni.viewInverse * vec4(normalize(target.xyz), 0);

    uint rayFlags = gl_RayFlagsNoneEXT;
    float tMin = 0.001;
    float tMax = 10000.0;

    // Llamada a la generación del rayo.
    // Para saber por qué ponemos esos valores en sbtRecordOffset y sbtRecordStride, mira este artículo:
    // https://www.willusher.io/graphics/2019/11/20/the-sbt-three-ways
    // (Y mira en general el capítulo 11 de nvpro tutorial que esto tiene mucha chicha)

    // Inicializar el payload correctamente
    prd.depth     = 0;
    prd.hit_value  = vec3(0);
    prd.ray_origin = origin.xyz;
    prd.ray_dir    = direction.xyz;
    prd.weight    = vec3(0);
    //prd.attenuation = vec3(1.f, 1.f, 1.f);    // No debería ser necesario con path tracing
    // seed ya estaba puesto

    vec3 current_weight = vec3(1);
            hit_value       = vec3(0);

    // Con este bucle evitamos generar rayos desde el closest hit
    for (; prd.depth < pcRay.max_depth; prd.depth++) {
        // Cada vez que impactemos con algo, se actualizará el origen y la nueva dirección.
        // Dependerá del closest hit poner los parámetros de impacto correctos.
        traceRayEXT(topLevelAS, // acceleration structure
            rayFlags,       // rayFlags
            0xFF,           // cullMask
            0,              // sbtRecordOffset
            0,              // sbtRecordStride
            0,              // missIndex
            prd.ray_origin,  // ray origin
            tMin,           // ray min range
            prd.ray_dir,     // ray direction
            tMax,           // ray max range
            0               // payload (location = 0)
        );

        // A partir de aquí, debemos determinar el color que vaya adquiriendo.
        hit_value += prd.hit_value * current_weight;
        current_weight *= prd.weight;
    }

    return hit_value;
}