#include "random.glsl"
#include "globals.glsl"

// Muestreo aleatorio alrededor de +Z
vec3 sampling_hemisphere(inout uint seed, in vec3 x, in vec3 y, in vec3 z) {
    float r1 = rnd(seed);
    float r2 = rnd(seed);
    float sq = sqrt(1.0 - r2);

    vec3 direction = vec3(cos(2 * M_PI * r1) * sq, sin(2 * M_PI * r1) * sq, sqrt(r2));
    direction      = direction.x * x + direction.y * y + direction.z * z;

    return direction;
}

vec3 cosine_sample_hemisphere(inout uint seed, in vec3 x, in vec3 y, in vec3 z, out float pdf) {
    vec3  direction;

    float r1 = rnd(seed);
    float r2 = rnd(seed);

    float r   = sqrt(r1);
    float phi = TWO_PI * r2;

    direction.x = r * cos(phi);
    direction.y = r * sin(phi);
    direction.z = sqrt(max(0.0, 1.0 - direction.x * direction.x - direction.y * direction.y));

    pdf = direction.z;

    direction = direction.x * x + direction.y * y + direction.z * z;

    return direction;
}

vec3 random_in_unit_sphere(inout uint seed) {
    // https://datagenetics.com/blog/january32020/index.html
    // Método Pol#2

    float u = rnd(seed);
    float v = rnd(seed);

    float theta = TWO_PI * u;
    float phi   = acos(2.0 * v - 1.0);

    float r = pow(rnd(seed), 0.333);

    return vec3(
        r * sin(phi) * cos(theta),
        r * sin(phi) * sin(theta),
        r * cos(phi)
    );
}

float reflectance (in float cosine, in float eta) {
    // Aproximación de Schlick para la reflectancia.
    float r0 = (1.0 - eta) / (1.0 + eta);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * pow(1.0 - cosine, 5.0);
}

// Return the tangent and binormal from the incoming normal
// http://www.pbr-book.org/3ed-2018/Geometry_and_Transformations/Vectors.html#CoordinateSystemfromaVector
void create_coordinate_system(in vec3 N, out vec3 Nt, out vec3 Nb) {
    // if(abs(N.x) > abs(N.y))
    //     Nt = vec3(N.z, 0, -N.x) / sqrt(N.x * N.x + N.z * N.z);
    // else
    //     Nt = vec3(0, -N.z, N.y) / sqrt(N.y * N.y + N.z * N.z);
    // Nb = cross(N, Nt);

    Nt = normalize(((abs(N.z) > 0.99999f) ? vec3(-N.x * N.y, 1.0f - N.y * N.y, -N.y * N.z) :
                                          vec3(-N.x * N.z, -N.y * N.z, 1.0f - N.z * N.z)));
    Nb = cross(Nt, N);
}