#include "types.h"

/*
• E is the eye vector, pointing from P to the origin of the ray (usually, where the eye exists that is looking at the scene).
• L is the light vector, pointing from P to the position of the light source.
• N is the surface normal, a vector that is perpendicular to the surface at P.
• R is the reflection vector, pointing in the direction that incoming light would bounce, or reflect.
*/
void scene_add_cylinder(Scene *scene, double x, double y, double z, double ox, double oy, double oz,double r, double g,double b,double height, double radius)
{
     Cylinder *s= malloc(sizeof(Cylinder));
     Model  *model = malloc(sizeof(Model));


     s->position    =(Vector3d){x,y,z};
     s->orientation= normalize((Vector3d){ox,oy,oz});
     s->color=(Color){r,g,b};

     double rad = radius / 2;

     s->diameter = radius;//rad * rad ;



     s->height   = height;
     model->mesh=s;
     model->type=ICYLINDER;
     model->dist= length(subtract(s->position,scene->camera.position));
     model->index = scene->objects->size;

     list_add(scene->objects,model);

}



void scene_add_sphere(Scene *scene, double x, double y, double z, double r, double g,double b,double radius)
{
     Sphere *s= malloc(sizeof(Sphere));
     Model  *model = malloc(sizeof(Model));


     s->position=(Vector3d){x,y,z};
     s->color=(Color){r,g,b};
     s->diameter = radius * radius;
     model->mesh=s;
     model->type=ISPHERE;
     model->dist= length(subtract(s->position,scene->camera.position));
     model->index = scene->objects->size;

     list_add(scene->objects,model);

}

void scene_add_plane(Scene *scene, double x, double y, double z, double r, double g,double b,double nx, double ny, double nz)
{
     Plane *s= malloc(sizeof(Plane));
     Model  *model = malloc(sizeof(Model));


     s->position=(Vector3d){x,y,z};
     s->color=(Color){r,g,b};
     s->normal =(Vector3d){nx,ny,nz};
     model->mesh=s;
     model->type=IPLANE;
     model->dist= length(subtract(s->position,scene->camera.position));
     model->index = scene->objects->size;

     list_add(scene->objects,model);

}

bool scene_object_in_shadow(Scene *scene, Model *object, Vector3d intersection)
{
    Vector3d lightDirection = normalize(subtract(scene->light.position, intersection));
    Ray shadowRay = {intersection, lightDirection};
    for (int i=0; i < scene->objects->size;i++)
    {
        Intersection impact;
        impact.distance=20000000.0;


        List *list = scene->objects;
        Model *current = list->data[i];
        if (current==object)
            continue;
        if (current->type==ISPHERE)
        {
            Sphere sphere = *((Sphere*) current->mesh);

            if (intersect_ray_sphere(shadowRay, sphere, &impact))
            {
              return true;
            }
        }else
         if (current->type==IPLANE)
         {
                 Plane plane = *((Plane*) current->mesh);
                 if (intersect_ray_plane(shadowRay, plane, &impact))
                {
                    return true;
                }
         }else
         if (current->type==ICYLINDER)
         {
                 Cylinder cylinder = *((Cylinder*) current->mesh);
                 if (intersect_ray_cylinder(shadowRay, cylinder, &impact))
                {
                    return true;
                }
         }

    }
    return false;
}

Color scene_process(Scene *scene, Ray ray)
{
    Color finalColor =(Color){0.2,0.2,0.6};
    bool intersectionFound=false;
    Model *object=NULL;
    double minDist = 1e6;

    Intersection impact;
    impact.distance=INFINITY;

    impact.hit=false;


    for (int i=0; i < scene->objects->size;i++)
    {
        List *list = scene->objects;
        Model *current = list->data[i];

         bzero(&impact,sizeof(Intersection));
         impact.distance=INFINITY;
         impact.hit=false;


        if (current->type==ISPHERE)
        {
            Sphere sphere = *((Sphere*) current->mesh);

            if (intersect_ray_sphere(ray, sphere, &impact))
            {
                    double dist  =impact.distance;// length(subtract(impact.point , ray.origin));
                    if (dist < minDist)
					{
                      minDist = dist;
                      object=current;
                      intersectionFound=true;
                      break;
                    }
            }

        }else
         if (current->type==IPLANE)
         {
                 Plane plane = *((Plane*) current->mesh);
                 if (intersect_ray_plane(ray, plane, &impact))
                {
                      double dist  =impact.distance;// length(subtract(impact.point , ray.origin));
                    if (dist < minDist)
					{
                      minDist = dist;
                      object=current;
                      intersectionFound=true;
                      break;
					}
              }

      }else
         if (current->type==ICYLINDER)
         {
                 Cylinder cylinder = *((Cylinder*) current->mesh);

                 if (intersect_ray_cylinder(ray, cylinder, &impact))
                {
                    double dist  =impact.distance;// length(subtract(impact.point , ray.origin));
                    if (dist < minDist)
					{
                      minDist = dist;
                      object=current;
                      intersectionFound=true;
                      break;
					}
                }
         }
    }

    if (intersectionFound && object!=NULL)
    {

        if (object->type==ISPHERE)
        {
             Sphere sphere = *((Sphere*) object->mesh);
             if (!scene_object_in_shadow(scene,object,impact.point))
             {
             finalColor = lighting_sphere(scene->light,ray,impact);
             finalColor = color_add(finalColor,color_mul_scalar(scene->ambient.color, scene->ambient.ratio));
             } else
               return  color_mul_scalar(scene->ambient.color, scene->ambient.ratio);


        } else
         if (object->type==IPLANE)
         {
            Plane plane = *((Plane*) object->mesh);
            if (!scene_object_in_shadow(scene,object,impact.point))
            {
            finalColor = lighting_plane(scene->light,ray,impact);
            finalColor = color_add(finalColor,color_mul_scalar(scene->ambient.color, scene->ambient.ratio));

            } else
                return color_mul_scalar(scene->ambient.color, scene->ambient.ratio);

        }else
         if (object->type==ICYLINDER)
         {
            Cylinder cl = *((Cylinder*) object->mesh);
            if (!scene_object_in_shadow(scene,object,impact.point))
            {
            finalColor = lighting_cylinder(scene->light,ray,impact);
            finalColor = color_add(finalColor,color_mul_scalar(scene->ambient.color, scene->ambient.ratio));
            } else
                return color_mul_scalar(scene->ambient.color, scene->ambient.ratio);

        }
    }

    /*
    for (int i=0; i < scene->objects->size;i++)
    {
        Intersection impact;
        impact.distance=20000000.0;
        impact.hit=false;


        List *list = scene->objects;
        Model *current = list->data[i];

        if (current->type==ISPHERE)
        {
            Sphere sphere = *((Sphere*) current->mesh);

            if (intersect_ray_sphere(ray, sphere, &impact))
            {
                if (scene_object_in_shadow(scene,current,impact.point))
                    break;
               finalColor = lighting_sphere(scene->light,ray,impact);
               finalColor = color_add(finalColor,color_mul_scalar(scene->ambient.color, scene->ambient.ratio));
            }

        }else
         if (current->type==IPLANE)
         {
                 Plane plane = *((Plane*) current->mesh);
                 if (intersect_ray_plane(ray, plane, &impact))
                {
                    if (scene_object_in_shadow(scene,current,impact.point))
                        break;
                    finalColor = lighting_plane(scene->light,ray,impact);
                    finalColor = color_add(finalColor,color_mul_scalar(scene->ambient.color, scene->ambient.ratio));
                }
         }

    }
*/

   return finalColor;
}

