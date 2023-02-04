#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 1020
#define WINDOW_HEIGHT 780

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define EPSILON 0.000001

#define ISPHERE   0
#define IPLANE    1
#define ICYLINDER 2
#define ILIGHT    5

typedef double Matrix4x4[4][4];

typedef struct Matrix {
    float m0, m4, m8, m12;      // Matrix first row (4 components)
    float m1, m5, m9, m13;      // Matrix second row (4 components)
    float m2, m6, m10, m14;     // Matrix third row (4 components)
    float m3, m7, m11, m15;     // Matrix fourth row (4 components)
} Matrix;

typedef struct {
    double r, g, b;
} Color;

typedef struct {
    double x, y, z;
} Vector3d;

typedef struct
{
     Vector3d origin;
     Vector3d direction;
} Ray;


typedef struct {
    Vector3d position;
    Vector3d orientation;
    double diameter;
    double height;
    Color color;
} Cylinder;

typedef struct {
    Vector3d position;
    double diameter;
    Color color;
} Sphere;


typedef struct {
    Vector3d position;
     Vector3d normal;
    Color color;
} Plane;


typedef struct {
    Vector3d position;
    double ratio;
    Color color;
} Light;


typedef struct {
    double ratio;
    Color color;
} AmbientLight;

typedef struct {
    Vector3d position;
    Vector3d orientation;
    Vector3d up;
    double u;
    double v;
    double fov;
} Camera;


typedef struct
{
   void *mesh;
   int  type;
   double dist;
   int    index;
} Model;


typedef struct
{
    Vector3d position;  //ponto do object
    Vector3d point;    // ponto de colisão
    Vector3d normal;
    double distance;   // distância do raio à origem
    bool hit;          // indica se houve interseção
    Color color;
} Intersection;

typedef struct
{
  Model** data;
  size_t size;
  size_t capacity;
} List;

typedef struct
{
    Camera camera;
    Light light;
    AmbientLight ambient;
    List *objects;
} Scene;


double dot(Vector3d v1, Vector3d v2);
Vector3d add(Vector3d v1, Vector3d v2);
Vector3d subtract(Vector3d v1, Vector3d v2);
Vector3d scale(double s, Vector3d v);
double length(Vector3d v);
Vector3d normalize(Vector3d v);
Vector3d reflect(Vector3d v, Vector3d normal);
Vector3d diff(Vector3d v1, Vector3d v2);
double	distance(const Vector3d p1, const Vector3d p2);
double norma(Vector3d v);
Vector3d	cross(const Vector3d right, const Vector3d forward);
Vector3d	divide(Vector3d vec, double t);


void identity_matrix(Matrix4x4 m);
void translation_matrix(Vector3d t, Matrix4x4 m) ;
void rotation_matrix(Vector3d r, Matrix4x4 m);
void scaling_matrix(Vector3d s, Matrix4x4 m);
void get_transformation_matrix(Vector3d position, Vector3d orientation, Vector3d scale, Matrix4x4 m);

//transform rotation
Vector3d transform_vector(Vector3d vec, Matrix4x4 mat);
Vector3d transform_point(Vector3d point, Matrix4x4 mat);
void transform_ray(Ray* ray, Matrix4x4 mat);


Color color_add(Color c1, Color c2) ;
Color color_add_scalar(Color a, float scalar) ;
Color color_sub(Color c1, Color c2) ;
Color color_mul(Color c1, Color c2) ;
Color color_mul_scalar(Color c, double k) ;


bool intersect_ray_sphere(Ray ray, Sphere sphere,Intersection *result);
bool intersect_ray_plane(Ray ray, Plane plane,Intersection *result);
Color lighting_sphere(Light light, Ray ray,  Intersection impact);
Color lighting_plane(Light light, Ray ray,    Intersection impact);
Color lighting_cylinder(Light light, Ray ray,  Intersection impact);
bool intersect_ray_cylinder(Ray ray, Cylinder cylinder, Intersection *result);

List* create_list(void);
void clear_list(List* array);
void destroy_list(List* array);
void list_add(List* array, const void* data);
void list_remove(List* array, size_t index);
void list_sort(List* array,int (*cmp)(void*, void*));

void scene_add_sphere(Scene *scene, double x, double y, double z, double r, double g,double b,double radius);
void scene_add_plane(Scene *scene, double x, double y, double z, double r, double g,double b,double nx, double ny, double nz);
void scene_add_cylinder(Scene *scene, double x, double y, double z, double ox, double oy, double oz,double r, double g,double b,double height, double radius);

Color scene_process(Scene *scene, Ray ray);
