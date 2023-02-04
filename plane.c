#include "types.h"


bool intersect_ray_plane(Ray ray, Plane plane,Intersection *result)
{

 result->hit = false;
 result->position = plane.position;
 result->distance=20000.0;

    double denom = dot(plane.normal, ray.direction);

    // verificar se o raio está paralelo ao plano
    if (fabs(denom) > EPSILON)
    {
        // calcular o numerador
        Vector3d diff = subtract(plane.position, ray.origin);
        double t = dot(diff, plane.normal) / denom;

        // verificar se o raio atravessa o plano
        if (t >= 0)
        {
            result->hit = true;
            result->distance = t;
            result->point = add(ray.origin, scale(t, ray.direction));
            result->color = plane.color;
        }
    }


    return result->hit;
}

Color lighting_plane(Light light, Ray ray,   Intersection impact)
{
    Color RGB = {0, 0, 0};


    Vector3d normv = scale(impact.distance, normalize(ray.direction));
	Vector3d normorg = add(ray.origin, normv);

	/*normalized vector N (difference	of plane point and norm origin)*/
	Vector3d N = normalize(subtract(normorg, impact.position));


	//vector for light L
	Vector3d d = subtract(light.position, normorg);

	//length of the distance from the origin of the plane
	double dL = length(d);

	//constants for attenuated lighting
	double c1 = .002;
	double c2 = .02;
	double c3 = .2;

	//attenuated lighting, calculated and added to rgb
	double atten = 1.0/((c1 * (dL * dL)) + (c2 * dL) + c3);
	if(atten < 0.1)
    {
		atten = 0.1;
 	}
    RGB.r += atten * (light.color.r * light.ratio) * (impact.color.r);
    RGB.g += atten * (light.color.g * light.ratio) * (impact.color.g);
    RGB.b += atten * (light.color.b * light.ratio) * (impact.color.b);

	return RGB;
}
