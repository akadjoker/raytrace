#include <stdio.h>
#include <stdlib.h>
#include "types.h"


 void setPixelColor(SDL_Surface* surface, int x, int y, double r, double g ,double b)
 {
    Uint32* pixels = (Uint32*)surface->pixels;
    pixels[y * surface->w + x] = SDL_MapRGB(surface->format, (Uint8)(r), (Uint8)(g ), (Uint8)(b ));
}


int compare(Model *a, Model*b)
{

    return (a->dist < b->dist);
}


Ray generate_ray(Camera camera, int screen_x, int screen_y, int screen_width, int screen_height)
{

Vector3d up ={0,1,0};

    double x = (2 * ((double)screen_x + 0.5) / (double)screen_width - 1) * tan(camera.fov/2) * up.x;
    double y = (1 - 2 * ((double)screen_y + 0.5) / (double)screen_height) * tan(camera.fov/2) * up.y;
    Vector3d direction = normalize(add(add(scale(x,camera.orientation),scale(y,camera.up)),camera.orientation));
    return (Ray) { .origin = camera.position, .direction = direction };
}


double	to_rad(const double angle)
{
	return (angle * M_PI * 0.5);
}

Vector3d	set_vect_dir_cam(const Camera camera, int w, int h, const int i, const int j)
{
	Vector3d	vect_dir;
	double	a;
	double	b;
	double	c;
	int		max;

	a = j + 0.5 - (w) * 0.5;
	b = i + 0.5 - (h) * 0.5;
	max = w > h ? w : h;
	c = max / (2 * tan((to_rad(camera.fov)) / 180.0));
	vect_dir.x = 1 * a + 0 * b + 0 * c;
	vect_dir.y = 0 * a + 1 * b + 0 * c;
	vect_dir.z = 0 * a + 0 * b + 1 * c;
	// vect unitaires de la camera

	//vect_dir = rot_vect(vect_dir, camera->fov * 0.5, 'x');
	//vect_dir = rot_vect(vect_dir, camera->fov * 0.5, 'y');
	//vect_dir = rot_vect(vect_dir, camera->fov * 0.5, 'z');

	// g_data->ray.dir.x = coord[0] - (g_data->render[0] / 2) * tmp.x;
    // g_data->ray.dir.y = coord[1] - (g_data->render[1] / 2) * tmp.y;
    // g_data->ray.dir.z = tmp.z * (-g_data->render[1] / (2 * tan((to_rad(g_data->cam->fov) / 2))))
	return (normalize(vect_dir));
}

int main()
{



	     // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error: Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Create the window and renderer
    SDL_Window *window = SDL_CreateWindow("Ray Tracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,WINDOW_WIDTH,WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Error: Unable to create window: %s\n", SDL_GetError());
        return 1;
    }


	  SDL_Surface* surface = SDL_GetWindowSurface(window);

     bool done=false;
     bool calculate=true;
    int frameCount = 0;
    Uint32 startTime = SDL_GetTicks();

    Scene scene;

    Camera camera=
    {
        .position= { 0, 0, -2 },
        .orientation={ 0, 0, 1 },
        .fov=90
    };

     Cylinder cylinder =
 {
    .position   =    { 0, 0, 0 },   // position
    .orientation=    { 0, 1, 0.0 },   // orientation
    .color       =   { 1.0, 1.0, 0.0 },
    .diameter    =  0.5,             // diameter
    .height       =    1              // height
    };



    scene.camera=camera;

    Sphere sphere=
    {
        .position = {0, 0, 10},
        .color =  {1, 0, 0},
        .diameter = 1,
    };

    Plane plane=
    {
        .position = {0, -1, 0},
        .color    = {1.0, 1.0, 1.0},
        .normal   = {0,1,0},
    };

Light light =
{
    .position = { -2, 10, -2 },
    .color = { 1, 0, 1 },
    .ratio = 0.6
};
scene.light=light;

AmbientLight ambient =
{
    .color = { 0.2, 0.2, 0.2 },
    .ratio = 0.6
};
scene.ambient=ambient;

scene.objects=create_list();


scene_add_plane(&scene,0,-1,-100,    1,1,1, 0,1,0);
scene_add_sphere(&scene,-2,0,  10,   1,1,0,  1);
scene_add_sphere(&scene,-1.5,1,9, 1,0,1, 1);
scene_add_sphere(&scene,1.5,0,8,  0,1,1, 1);
/*
scene_add_cylinder(&scene, 1.5, 2,4,
                   //1, 0, 0.9,
                   0,-1,-0.2,
                   1,1,1,
                   1.5, 1);

scene_add_cylinder(&scene, -0.4, 1, 5,  -0.2, 0, -1,
                   1,0,0,
                   2.0, 0.8);
/*/
scene_add_cylinder(&scene, 0, 0, 0,  0.0, 1.0, 0.0,
                   1,1,1,
                   1.0, 1.0);

list_sort(scene.objects,compare);




    float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    float halfFOV = camera.fov * 0.5f;
    float halfWidth = tanf(halfFOV * (M_PI / 180.0f));
    float halfHeight = halfWidth / aspectRatio;


     while (!done)
    {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done=true;
                break;
            } else
             if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    done=true;
                } else
                  if (event.key.keysym.sym == SDLK_p)
                {
                    scene.light.position.y-=0.5;

                }else
                if (event.key.keysym.sym == SDLK_l)
                {
                    scene.light.position.y+=0.5;

                }
                else
                  if (event.key.keysym.sym == SDLK_m)
                {
                    scene.light.position.x-=0.5;

                }else
                if (event.key.keysym.sym == SDLK_n)
                {
                    scene.light.position.x-=0.5;

                }


                break;
            } else
             if (event.type == SDL_KEYUP)
            {
                   calculate=true;
                   break;
            }
        }


         frameCount++;
        if (SDL_GetTicks() - startTime >= 1000)
        {
            char title[150];

                snprintf(title, sizeof( title), "  FPS: %d", frameCount);

            SDL_SetWindowTitle(window, title);
            frameCount = 0;
            startTime = SDL_GetTicks();
        }




    if (calculate)
    {




    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {

            double pixelX = (x + 0.5) / WINDOW_WIDTH;
            double pixelY = (y + 0.5) / WINDOW_HEIGHT;
            Vector3d pixelPos = (Vector3d){(halfWidth * (2.0 * pixelX - 1.0), -halfHeight * (2.0 * pixelY - 1.0), -1.0)};
            camera.u =  (2 * (x + 0.5) / WINDOW_WIDTH - 1) * tan(camera.fov / 2 * M_PI / 180) * WINDOW_WIDTH / WINDOW_HEIGHT;
            camera.v = -(2 * (y + 0.5) / WINDOW_HEIGHT - 1) * tan(camera.fov / 2 * M_PI / 180);





            Vector3d rayDirection = normalize((Vector3d) { camera.u, camera.v, 1 });
            //Vector3d rayDirection =set_vect_dir_cam(camera,WINDOW_WIDTH, WINDOW_HEIGHT,x,y);

            Ray ray = { camera.position, rayDirection };




            Color finalColor= (Color){0,0,0};
            finalColor = scene_process(&scene,ray);
/*
            Intersection impact;
              if (intersect_ray_cylinder(ray, cylinder,&impact ))
               {
                //   printf("hit\n");
                    finalColor = lighting_cylinder(light,ray,impact);
                   // finalColor= (Color){0,1,1};

        *       }
*/
               if (finalColor.r<=0.0)
                    finalColor.r=0.0;
                if (finalColor.g<=0.0)
                    finalColor.g=0.0;
                if (finalColor.b<=0.0)
                    finalColor.b=0.0;

                if (finalColor.r>1.0)
                    finalColor.r=1.0;
                if (finalColor.g>1.0)
                    finalColor.g=1.0;
                if (finalColor.b>1.0)
                    finalColor.b=1.0;



                    setPixelColor(surface,x,y,finalColor.r*255,finalColor.g*255,finalColor.b*255);

        //    DrawPixel(x,y,(Color){finalColor.r*255,finalColor.g*255,finalColor.b*255,255});

        }
    }

	   calculate=false;
    }
    SDL_UpdateWindowSurface(window);



    }

    destroy_list(scene.objects);

    SDL_SaveBMP(surface,"trace.bmp");
    SDL_DestroyWindow(window);
    SDL_Quit();



}
