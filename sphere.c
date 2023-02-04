#include "types.h"

/*
http://www.lighthouse3d.com/tutorials/maths/ray-sphere-intersection/
*/
bool intersect_ray_sphere(Ray ray, Sphere sphere,Intersection *result)
{

   result->hit=false;
   result->position = sphere.position;
   result->distance=20000.0;

	//calculate A, B, and C variables to determine the discriminant
	double A = dot(ray.direction, ray.direction);

	Vector3d dist = subtract(ray.origin, sphere.position);
	double B = 2 * dot(ray.direction, dist);
	double C = dot(dist, dist) - (sphere.diameter * sphere.diameter);

	//equation for the discriminant B^2 - 4AC
	double discriminant = (B * B) - 4 * (A * C);

	//return no hit if disc < 0
	if(discriminant < 0)
		result->hit=false;;

	//determining the t0 and t1 for the sphere intersection
		double discsqrt = sqrt(discriminant);
		double t0 = (-B - discsqrt)/(2);
		double t1 = (-B + discsqrt)/(2);
		//set t0 to not greater (closer) than t1
		if(t0<0 && t1==0)
        {
		 result->hit=false;
		}
		if(t0 < 0.0)
		{
		      result->distance = t1;
		}

       //if its also greater than 0, return 1 for hit. If not then 0
       if(t0 > 0.0)
        {
          result->distance = t0;
          result->hit=true;
        }
        else
        {
           result->hit=false;
        }



	result->color=sphere.color;
	result->point=add(ray.origin, scale(result->distance, ray.direction));



    return result->hit;
}


Color lighting_sphere(Light light, Ray ray,  Intersection impact)
{
    Color RGB = {0, 0, 0};


	//vectors for normal, N
	Vector3d normv = scale(impact.distance, normalize(ray.direction));
	Vector3d normorg = add(ray.origin, normv);


	//vector for light L
	Vector3d litlen = subtract(light.position, normorg);
	Vector3d L = scale(1.0/impact.distance, litlen);


	//normalized vector N (difference	of sphere center and norm origin)
	Vector3d N = normalize(subtract(normorg, impact.position));

	// diffuse lighting:	cos theta = N dot L; angle between light and normal vectors	multiplied by the current light amounts, added to rgb
	double cos_theta = dot(L, N);
	if(cos_theta > 0)
    {
		RGB.r += cos_theta * (light.color.r * light.ratio) * (impact.color.r);
		RGB.g += cos_theta * (light.color.g * light.ratio) * (impact.color.g);
		RGB.b += cos_theta * (light.color.b * light.ratio) * (impact.color.b);
	}

	//* equation for R = L - 2 * (N dot L) * N,	R is the vector between N and V	broken down into working components via temp variables*
	Vector3d tempr = (scale((2 * dot(L, N)), N));
	Vector3d R = normalize(subtract(L, tempr));
	// specular lighting	 cosine alpha = dot product of R and V	 add (cosine alpha^40) to rgb
	double cos_alpha = dot(R, normalize(ray.direction));
	double n = (40.0/cos_alpha) / light.ratio;
	if((cos_alpha >0) && (cos_theta > 0))
    {
		RGB.r += pow(cos_alpha, n);
		RGB.g += pow(cos_alpha, n);
		RGB.b += pow(cos_alpha, n);
	}
return RGB;
}

