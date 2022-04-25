struct HitPayload
{
    vec3 hit_value;

    // Reflections
    int  depth;

    //vec3 attenuation;
    vec3 ray_origin;
    vec3 ray_dir;

    uint seed;

    // PBR
    vec3 weight;
};

struct ShadowPayload
{
    bool is_hit;
    uint seed;
};