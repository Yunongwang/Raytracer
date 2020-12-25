#include <iostream>
#include "sphere.h"
#include "cube.h"
#include "hittable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"


vec3 color(const ray& r, hit_table *world, int n, vec3 light) {
    hit_record rec;
    if(n < 0){
        return vec3(0,0,0);
    }

    if (world->hit(r,0.001,FLT_MAX,rec)){
        ray scattered;
        vec3 attenuation;


        if ( rec.mat_ptr->scatter(r,rec,attenuation,scattered))
        {   
            hit_record reclight;
            light = light +random_in_unit_sphere() * 5.0;
            vec3 direction = unit_vector(light - rec.p);
            ray lr = ray(rec.p,direction);
            vec3 scatteredr  =color(scattered,world,n-1,light);
            if(world->hit(lr,0.001,FLT_MAX,reclight)){
                return 0.12*attenuation *scatteredr;
            }
            return attenuation*scatteredr;
        }
        return vec3(0,0,0);

    } else {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0,1.0,1.0) +t*vec3(0.5,0.7,1.0);
    }
}

int main()
{   vec3 pointlight = vec3(20 ,25,5);
    vec3 lightColor = vec3(1 ,1,1);

    int nx = 400;
    int ny = 200;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    hit_table *list[4];
    list[0] = new sphere(vec3(-1,0,-1.5),0.5, new blinn(vec3(0.8,0.0,0.0),pointlight,lightColor));
//    list[0] = new cube(vec3(-1,1,-1.5),vec3(-1,1,-0.5),vec3(0,1,-0.5), 
//                vec3(0,1,-1.5),vec3(-1,0,-1.5),vec3(-1,0,-0.5),vec3(0,0,-0.5),vec3(0,0,-1.5),new metal(vec3(0.1,0.2,0.8), 0.2));
    list[1] = new sphere(vec3(0,-100.5,-1),100, new lambertian(vec3(0.8,0.8,0.8)));
    // list[2] = new cube(vec3(0,1,0),vec3(0,1,1),vec3(1,1,1), 
    //             vec3(1,1,0),vec3(0,0,0),vec3(0,0,1),vec3(1,0,1),vec3(1,0,0),new dielectric (1.2));
//    list[3] = new cube(vec3(1,1,2),vec3(1,1,3),vec3(2,1,3), 
//            vec3(2,1,2),vec3(1,0,2),vec3(1,0,3),vec3(2,0,3),vec3(2,0,2),new lambertian (vec3(0,0,0.8)));
    list[2] = new sphere(vec3(1,0.5,3),1, new blinn(vec3(0.0,0.8,0.0),pointlight,lightColor));//metal (vec3(0.1,0.5,0.7), 0));
    list[3] = new sphere(vec3(0,0.5,0.5),0.75, new blinn(vec3(0.0,0.0,0.8),pointlight,lightColor));//metal (vec3(0.1,0.5,0.7), 0));


    hit_table *world = new hit_table_list(list,4);
    vec3 lookfrom(10,5,-2);
    vec3 lookat(-20,-10,5);
    float dist_to_focus = (lookfrom - lookat).length();
    float aperture = 0.0;
    camera cam(lookfrom,lookat,vec3(0,1,0),20, static_cast<float>(nx)/static_cast<float>(ny),aperture,dist_to_focus);
    for (int j = ny-1; j >=0; j--) {
        for (int i =0; i < nx; i++) {
            vec3 col(0,0,0);
            for (int s = 0; s < ns; s++)
            {
                float u = static_cast<float>(i + (rand() / (RAND_MAX + 1.0))) / static_cast<float>(nx);
                float v = static_cast<float>(j + (rand() / (RAND_MAX + 1.0))) / static_cast<float>(ny);
                ray r = cam.get_ray(u,v);
                //vec3 p = r.point_at_paramter(2.0);
                col += color(r,world,3,pointlight);
            }
            col /= static_cast<float>(ns);
            col = vec3(sqrt(col[0]),sqrt(col[1]),sqrt(col[2]));
            int ir = static_cast<int>(255.99*col.r());
            int ig = static_cast<int>(255.99*col.g());
            int ib = static_cast<int>(255.99*col.b());
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}