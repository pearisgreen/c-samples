//hdr
#ifndef VMATH_H
#define VMATH_H

struct vec2
{
    double x;
    double y;
};

struct vec3
{
    double x;
    double y;
    double z;
};

struct vec4
{
    double x;
    double y;
    double z;
    double a;
};

struct vec2i
{
    int x;
    int y;
};

struct vec3i
{
    int x;
    int y;
    int z;
};

struct vec4i
{
    int x;
    int y;
    int z;
    int a;
};

void vec2_add(struct vec2 *v1, struct vec2 *v2);
void vec3_add(struct vec3 *v1, struct vec3 *v3);
void vec4_add(struct vec4 *v1, struct vec4 *v3);

void vec2_scale(struct vec2 *v1, double scalar);
void vec3_scale(struct vec3 *v1, double scalar);
void vec4_scale(struct vec4 *v1, double scalar);

void vec2i_add(struct vec2i *v1, struct vec2i *v2);
void vec3i_add(struct vec3i *v1, struct vec3i *v3);
void vec4i_add(struct vec4i *v1, struct vec4i *v3);

void vec2i_scale(struct vec2i *v1, int scalar);
void vec3i_scale(struct vec3i *v1, int scalar);
void vec4i_scale(struct vec4i *v1, int scalar);

#endif

//src
#ifndef VMATH_C

void vec2_add(struct vec2 *v1, struct vec2 *v2)
{
    v1->x += v2->x;
    v1->y += v2->y;
}
void vec3_add(struct vec3 *v1, struct vec3 *v2)
{
    v1->x += v2->x;
    v1->y += v2->y;
    v1->z += v2->z;
}
void vec4_add(struct vec4 *v1, struct vec4 *v2)
{
    v1->x += v2->x;
    v1->y += v2->y;
    v1->z += v2->z;
    v1->a += v2->a;
}

void vec2_scale(struct vec2 *v1, double scalar)
{
    v1->x *= scalar;
    v1->y *= scalar;
}
void vec3_scale(struct vec3 *v1, double scalar)
{
    v1->x *= scalar;
    v1->y *= scalar;
    v1->z *= scalar;
}
void vec4_scale(struct vec4 *v1, double scalar)
{
    v1->x *= scalar;
    v1->y *= scalar;
    v1->z *= scalar;
    v1->a *= scalar;
}

void vec2i_add(struct vec2i *v1, struct vec2i *v2)
{
    v1->x += v2->x;
    v1->y += v2->y;
}
void vec3i_add(struct vec3i *v1, struct vec3i *v2)
{
    v1->x += v2->x;
    v1->y += v2->y;
    v1->z += v2->z;
}
void vec4i_add(struct vec4i *v1, struct vec4i *v2)
{
    v1->x += v2->x;
    v1->y += v2->y;
    v1->z += v2->z;
    v1->a += v2->a;
}

void vec2i_scale(struct vec2i *v1, int scalar)
{
    v1->x *= scalar;
    v1->y *= scalar;
}
void vec3i_scale(struct vec3i *v1, int scalar)
{
    v1->x *= scalar;
    v1->y *= scalar;
    v1->z *= scalar;
}
void vec4i_scale(struct vec4i *v1, int scalar)
{
    v1->x *= scalar;
    v1->y *= scalar;
    v1->z *= scalar;
    v1->a *= scalar;
}

#endif