#include "types.h"




// função para calcular o produto escalar de dois vetores
double dot(Vector3d v1, Vector3d v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// função para calcular a soma de dois vetores
Vector3d add(Vector3d v1, Vector3d v2)
{
    Vector3d result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

// função para calcular a subtração de dois vetores
Vector3d subtract(Vector3d v1, Vector3d v2)
{
    Vector3d result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

Vector3d	cross(const Vector3d right, const Vector3d forward)
{
	return ((Vector3d){
		right.y * forward.z - right.z * forward.y,
		right.z * forward.x - right.x * forward.z,
		right.x * forward.y - right.y * forward.x});


}

Vector3d	divide(Vector3d vec, double t)
{
	vec.x /= t;
	vec.y /= t;
	vec.z /= t;
	return (vec);
}

// função para calcular o produto de um vetor por um escalar
Vector3d scale(double s, Vector3d v)
 {
    Vector3d result;
    result.x = s * v.x;
    result.y = s * v.y;
    result.z = s * v.z;
    return result;
}

// função para calcular o módulo de um vetor
double length(Vector3d v)
{
    return sqrt(dot(v, v));
}

double norma(Vector3d v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// função para normalizar um vetor
Vector3d normalize(Vector3d v)
 {
    return scale(1.0 / length(v), v);
}

Vector3d reflect(Vector3d v, Vector3d normal)
{
    return subtract(v, scale(2 * dot(v, normal),normal));
}

double	distance(const Vector3d v1, const Vector3d v2)
{
	//return (sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2)));
	float result = 0.0f;

    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;
    result = sqrtf(dx*dx + dy*dy + dz*dz);
}

/*
void get_transformation_matrix(Vector3d position, Vector3d orientation, double mat[4][4])
{
    Vector3d z = normalize(orientation);
    Vector3d x = normalize(cross((Vector3d){0, 1, 0}, z));
    Vector3d y = cross(z, x);
    mat[0][0] = x.x;
    mat[1][0] = x.y;
    mat[2][0] = x.z;
    mat[3][0] = position.x;
    mat[0][1] = y.x;
    mat[1][1] = y.y;
    mat[2][1] = y.z;
    mat[3][1] = position.y;
    mat[0][2] = z.x;
    mat[1][2] = z.y;
    mat[2][2] = z.z;
    mat[3][2] = position.z;
    mat[0][3] = 0;
    mat[1][3] = 0;
    mat[2][3] = 0;
    mat[3][3] = 1;
}

//transform rotation
Vector3d transform_vector(Vector3d vec, double mat[4][4])
{
    Vector3d result;
    result.x = mat[0][0] * vec.x + mat[1][0] * vec.y + mat[2][0] * vec.z;
    result.y = mat[0][1] * vec.x + mat[1][1] * vec.y + mat[2][1] * vec.z;
    result.z = mat[0][2] * vec.x + mat[1][2] * vec.y + mat[2][2] * vec.z;
    return result;
}

Vector3d transform_point(Vector3d point, double mat[4][4])
{
    Vector3d result;
    result.x = mat[0][0] * point.x + mat[1][0] * point.y + mat[2][0] * point.z + mat[3][0];
    result.y = mat[0][1] * point.x + mat[1][1] * point.y + mat[2][1] * point.z + mat[3][1];
    result.z = mat[0][2] * point.x + mat[1][2] * point.y + mat[2][2] * point.z + mat[3][2];
    return result;
}


void transform_ray(Ray* ray, double mat[4][4])
{
    Vector3d origin = transform_point(ray->origin, mat);
    Vector3d direction = transform_vector(ray->direction, mat);


    ray->origin = origin;
    ray->direction = direction;
}

/*/

void identity_matrix(Matrix4x4 m)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = (i == j) ? 1 : 0;
        }
    }
}

void translation_matrix(Vector3d t, Matrix4x4 m) {
    identity_matrix(m);
    m[0][3] = t.x;
    m[1][3] = t.y;
    m[2][3] = t.z;
}

void rotation_matrix(Vector3d r, Matrix4x4 m) {
    double x = r.x, y = r.y, z = r.z;
    double sx = sin(x), cx = cos(x);
    double sy = sin(y), cy = cos(y);
    double sz = sin(z), cz = cos(z);

    identity_matrix(m);
    m[0][0] = cy * cz;
    m[0][1] = -cy * sz;
    m[0][2] = sy;
    m[1][0] = cx * sz + sx * sy * cz;
    m[1][1] = cx * cz - sx * sy * sz;
    m[1][2] = -sx * cy;
    m[2][0] = sx * sz - cx * sy * cz;
    m[2][1] = sx * cz + cx * sy * sz;
    m[2][2] = cx * cy;
}

void scaling_matrix(Vector3d s, Matrix4x4 m) {
    identity_matrix(m);
    m[0][0] = s.x;
    m[1][1] = s.y;
    m[2][2] = s.z;
}


void get_transformation_matrix(Vector3d position, Vector3d orientation, Vector3d scale, Matrix4x4 m)
{
    Matrix4x4 translation, rotation, scaling;

    translation_matrix(position, translation);
    rotation_matrix(orientation, rotation);
    scaling_matrix(scale, scaling);

    // Multiply the matrices in the correct order to get the final transformation matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = translation[i][0] * rotation[0][j] * scaling[0][j]
                + translation[i][1] * rotation[1][j] * scaling[1][j]
                + translation[i][2] * rotation[2][j] * scaling[2][j]
                + translation[i][3] * rotation[3][j] * scaling[3][j];
        }
    }
}

//transform rotation
Vector3d transform_vector(Vector3d vec, Matrix4x4 mat)
{
    Vector3d result;
    result.x = mat[0][0] * vec.x + mat[1][0] * vec.y + mat[2][0] * vec.z;
    result.y = mat[0][1] * vec.x + mat[1][1] * vec.y + mat[2][1] * vec.z;
    result.z = mat[0][2] * vec.x + mat[1][2] * vec.y + mat[2][2] * vec.z;
    return result;
}

Vector3d transform_point(Vector3d point, Matrix4x4 mat)
{
    Vector3d result;
    result.x = mat[0][0] * point.x + mat[1][0] * point.y + mat[2][0] * point.z + mat[3][0];
    result.y = mat[0][1] * point.x + mat[1][1] * point.y + mat[2][1] * point.z + mat[3][1];
    result.z = mat[0][2] * point.x + mat[1][2] * point.y + mat[2][2] * point.z + mat[3][2];
    return result;
}

void transform_ray(Ray* ray, double mat[4][4])
{
    Vector3d origin = transform_point(ray->origin, mat);
    Vector3d direction = transform_vector(ray->direction, mat);


    ray->origin = origin;
    ray->direction = direction;
}

