#include "globals.glsl"
#include "layouts.glsl"

layout(location = 0) rayPayloadEXT HitPayload prd;

vec3 pathtrace(vec4 ray_origin, vec4 ray_dir, float t_min, float t_max, uint ray_flags) {
    // Inicializar el payload correctamente
    prd.depth      = 0;
    prd.hit_value  = vec3(0);
    prd.ray_origin = ray_origin.xyz;
    prd.ray_dir    = ray_dir.xyz;
    prd.weight     = vec3(0);
    //prd.attenuation = vec3(1.f, 1.f, 1.f);
    // prd.seed ya estaba inicializado

    vec3 current_weight = vec3(1);
    vec3 hit_value      = vec3(0);

    // Evitar llamadas recursivas a traceRayEXT() desde el closest hit.
    for (; prd.depth < pcRay.max_depth; prd.depth++) {
        traceRayEXT(topLevelAS, // acceleration structure
            ray_flags,          // rayFlags
            0xFF,               // cullMask
            0,                  // sbtRecordOffset
            0,                  // sbtRecordStride
            0,                  // missIndex
            prd.ray_origin,     // ray origin
            t_min,              // ray min range
            prd.ray_dir,        // ray direction
            t_max,              // ray max range
            0                   // payload (location = 0)
        );

        hit_value      += prd.hit_value * current_weight;
        current_weight *= prd.weight;
    }

    return hit_value;
}


vec3 sample_pixel(ivec2 image_coords, ivec2 image_res) {
    float r1 = rnd(prd.seed);
    float r2 = rnd(prd.seed);

    // Subpixel jitter: mandar el rayo desde una pequeña perturbación del pixel para aplicar antialiasing
    vec2 subpixel_jitter = pcRay.frame == 0
        ? vec2(0.5f, 0.5f)
        : vec2(r1, r2);

    const vec2 pixelCenter = vec2(image_coords.xy) + subpixel_jitter;
    const vec2 inUV        = pixelCenter/vec2(image_res.xy);
    vec2  d                = inUV * 2.0 - 1.0;

    vec4 origin    = uni.viewInverse * vec4(0, 0, 0, 1);
    vec4 target    = uni.projInverse * vec4(d.x, d.y, 1, 1);
    vec4 direction = uni.viewInverse * vec4(normalize(target.xyz), 0);

    uint  ray_flags = gl_RayFlagsNoneEXT;
    float t_min     = 0.001;
    float t_max     = 10000.0;

    vec3 radiance = pathtrace(origin, direction, t_min, t_max, ray_flags);

    return radiance;
}
