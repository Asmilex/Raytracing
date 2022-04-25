//
// ────────────────────────────────────────────────────── CONSTANTES GLOBALES ─────
//

#ifndef GLOBALS_GLSL
#define GLOBALS_GLSL

#define PI 3.14159265358979323
#define TWO_PI 6.28318530717958648
#define EPS 0.0001

#define M_PI 3.14159265358979323846             // pi
#define M_TWO_PI 6.28318530717958648            // 2*pi
#define M_PI_2 1.57079632679489661923           // pi/2
#define M_PI_4 0.785398163397448309616          // pi/4
#define M_1_OVER_PI 0.318309886183790671538     // 1/pi
#define M_2_OVER_PI 0.636619772367581343076     // 2/pi

//
// ────────────────────────────────────────────────────────────── ESTRUCTURAS ─────
//

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

#endif