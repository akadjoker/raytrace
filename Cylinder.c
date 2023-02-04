#include "types.h"


Vector3d vector_normalize(Vector3d v) {
    double magnitude = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
    return (Vector3d) {
        v.x / magnitude,
        v.y / magnitude,
        v.z / magnitude
    };
}

double vector_magnitude(Vector3d v) {
    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}


Vector3d vector_rotate(Vector3d vec, Vector3d axis, double angle) {
    double x = vec.x, y = vec.y, z = vec.z;
    double u = axis.x, v = axis.y, w = axis.z;
    double cos_angle = cos(angle);
    double sin_angle = sin(angle);
    double ms = pow(u, 2) + pow(v, 2) + pow(w, 2);
    double m = sqrt(ms);
    double x_ = u * (u * x + v * y + w * z) * (1 - cos_angle) / ms + x * cos_angle + (-w * y + v * z) * sin_angle;
    double y_ = v * (u * x + v * y + w * z) * (1 - cos_angle) / ms + y * cos_angle + (w * x - u * z) * sin_angle;
    double z_ = w * (u * x + v * y + w * z) * (1 - cos_angle) / ms + z * cos_angle + (-v * x + u * y) * sin_angle;
    return (Vector3d) { x_, y_, z_ };
}
Ray ray_rotate(Ray r, Vector3d axis, double angle) {
    Vector3d origin = vector_rotate(r.origin, axis, angle);
    Vector3d direction = vector_rotate(r.direction, axis, angle);
    return (Ray) { origin, direction };
}


/*
bool intersect_ray_cylinder(Ray ray, Cylinder cylinder, Intersection *result)
{

   result->distance=-1;
   result->color=cylinder.color;
   result->position=cylinder.position;

     double a = pow(ray.direction.x, 2) + pow(ray.direction.z, 2);
    double b = 2 * (ray.origin.x - cylinder.position.x) * ray.direction.x + 2 * (ray.origin.z - cylinder.position.z) * ray.direction.z;
    double c = pow(ray.origin.x - cylinder.position.x, 2) + pow(ray.origin.z - cylinder.position.z, 2) - pow(cylinder.diameter / 2, 2);
    double discriminant = pow(b, 2) - 4 * a * c;
    if (discriminant < 0) {
        return false;
    }
    double t1 = (-b + sqrt(discriminant)) / (2 * a);
    double t2 = (-b - sqrt(discriminant)) / (2 * a);
    double t = fmin(t1, t2);
    if (t < 0) {
        t = fmax(t1, t2);
    }
    if (t < 0) {
        return false;
    }
    result->hit = true;
    result->distance = t;
    result->point = (Vector3d)
    {
        ray.origin.x + t * ray.direction.x,
        ray.origin.y + t * ray.direction.y,
        ray.origin.z + t * ray.direction.z
    };
    double y_intersection = result->point.y;
    if (y_intersection < cylinder.position.y || y_intersection > cylinder.position.y + cylinder.height)
    {
        result->hit = false;
        return false;
    }
    result->normal = (Vector3d)
  {
        (result->point.x - cylinder.position.x) / cylinder.diameter / 2,
        0,
        (result->point.z - cylinder.position.z) / cylinder.diameter / 2
    };
    if (t1 < 0)
    {
        result->normal = scale(-1,result->normal);
    }

}
*/

Vector3d ComputeCylinderNormal(const Vector3d intersection, const Cylinder cylinder)
{
    Vector3d v = subtract(intersection , cylinder.position);
    double y = dot(v, cylinder.orientation);
    Vector3d normal =scale(y,subtract(v, cylinder.orientation));
    return normalize(normal);
}

bool intersect_ray_cylinder(Ray ray, Cylinder cylinder, Intersection *result)
 {
    result->color=cylinder.color;
    result->position=cylinder.position;

    Vector3d rayOriginLocal = subtract(ray.origin     , cylinder.position);
    Vector3d rayDirectionLocal = subtract(ray.direction, cylinder.orientation);





	Vector3d v = scale(dot(ray.direction, cylinder.orientation),cylinder.orientation);
	v = subtract(ray.direction, v);
	Vector3d u = scale(dot(subtract(ray.origin, cylinder.position), cylinder.orientation),cylinder.orientation);
	u = subtract(subtract(ray.origin, cylinder.position), u);

	double a = dot(v, v);
	double b = 2 * dot(v, u);
    double radius = cylinder.diameter / 2;
	double c = dot(u, u) - pow(radius, 2);


	double t0 = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
	double t1 = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);


    double t = fmin(t0, t1);
    if (t < 0)
    {
    t = fmax(t0, t1);
    }
    if (t < 0)
        return false;




    result->point =  add( ray.origin, scale(t0, ray.direction));
    result->distance=t;


	double dist1 = dot(cylinder.orientation, subtract(scale(t0,ray.direction), subtract(cylinder.position, ray.origin)));
	double dist2 = dot(cylinder.orientation, subtract(scale(t1,ray.direction), subtract(cylinder.position, ray.origin)));


	if (!((dist1 >= 0 && dist1 <= cylinder.height && t0 > EPSILON) || (dist2 >= 0 && dist2 <= cylinder.height && t1 > EPSILON)))
    {
        // Check if the intersection point is on the top or bottom cap
        Vector3d top = add(cylinder.position, scale(cylinder.height / 2, cylinder.orientation));
        Vector3d bottom = add(cylinder.position, scale(-cylinder.height / 2, cylinder.orientation));

        Vector3d topNormal = vector_normalize(subtract(result->point, top));
        Vector3d bottomNormal = vector_normalize(subtract(result->point, bottom));

        double topDist = distance(result->point, top);
        double bottomDist = distance(result->point, bottom);
        if (topDist <= cylinder.diameter / 2 && dot(topNormal, cylinder.orientation) > 0)

         {
            result->normal = topNormal;
         //   return true;
        } else if (bottomDist <= cylinder.diameter / 2 && dot(bottomNormal, cylinder.orientation) < 0)
        {
           // result->normal = bottomNormal;
         //   return true;
        }

        return false;
    }




    double y = dot(subtract(result->point , cylinder.position), cylinder.orientation);
    Vector3d normal =scale(y,subtract(subtract(result->point , cylinder.position), cylinder.orientation));
    result->normal= normalize(normal);


    double dist = dot(subtract(result->point, cylinder.position), cylinder.orientation);
    if (fabs(dist) <= cylinder.height/2 && distance(result->point, cylinder.position) <= cylinder.diameter/2) {
        return true;
    }
  return true;

   // return true;
}


Color lighting_cylinder(Light light, Ray ray,  Intersection impact)
{
  Color RGB = {0, 0, 0};

	//vector for light L
	Vector3d litlen = subtract(light.position, impact.normal);
	Vector3d L = scale(1.0/impact.distance, litlen);



	// diffuse lighting:	cos theta = N dot L; angle between light and normal vectors	multiplied by the current light amounts, added to rgb
	double cos_theta = dot(L, impact.normal);
	if(cos_theta > 0)
    {
		RGB.r += cos_theta * (light.color.r * light.ratio) * (impact.color.r);
		RGB.g += cos_theta * (light.color.g * light.ratio) * (impact.color.g);
		RGB.b += cos_theta * (light.color.b * light.ratio) * (impact.color.b);
	}

	//* equation for R = L - 2 * (N dot L) * N,	R is the vector between N and V	broken down into working components via temp variables*
	Vector3d tempr = (scale((2 * dot(L, impact.normal)), impact.normal));
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
