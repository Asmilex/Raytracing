struct hitPayload
{
    vec3 hitValue;

    // Reflections
    int  depth;
    vec3 attenuation;
    int done;
    vec3 rayOrigin;
    vec3 rayDir;

    uint seed;
};

struct shadowPayload
{
    bool isHit;
    uint seed;
};