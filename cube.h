#ifndef CUBE_H
#define CUBE_H
#include <memory>
#include "hittable.h"
#include <iostream>
#include "triangle.h"
#include "hittable_list.h"


class cube: public hit_table {
    public:
        cube() {}
        cube(vec3 _a, vec3 _b, vec3 _c, vec3 _d, vec3 _e, vec3 _f, vec3 _g, vec3 _h,  material* m);
        cube(vec3 _a, vec3 _g, material* m);
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
        vec3 a, b, c, d ,e ,f, g,h;
        material* mat;
        hit_table_list * list_ptr;
};

cube::cube(vec3 _a, vec3 _b, vec3 _c, vec3 _d, vec3 _e, vec3 _f, vec3 _g, vec3 _h,  material* m){
    hit_table **list= new hit_table*[12];
    a = _a; b= _b; c = _c; d = _d; e = _e; f = _f; g = _g; h = _h;
    // top
    list[0] = new triangle(a,b,d, m);
    list[1] = new triangle(c,d,b, m);
    // bot
    list[2] = new triangle(e,h,f, m);
    list[3] = new triangle(g,f,h, m);
    // left
    list[4] = new triangle(a,e,b, m);
    list[5] = new triangle(f,b,e, m);
    // right
    list[6] = new triangle(d,c,h, m);
    list[7] = new triangle(g,h,c, m);
    // close
    list[8] = new triangle(b,f,c, m);
    list[9] = new triangle(g,c,f, m);
    // far
    list[10] = new triangle(a,d,e, m);
    list[11] = new triangle(h,e,d, m);
    list_ptr = new hit_table_list(list,12);

}
// cube::cube(vec3 _a, vec3 _g,  material* m){
//     hit_table **list= new hit_table*[12];
//     a = _a; b= _b; c = _c; d = _d; e = _e; f = _f; g = _g; h = _h;
//     // top
//     list[0] = new triangle(a,b,d, m);
//     list[1] = new triangle(c,d,b, m);
//     // bot
//     list[2] = new triangle(e,h,f, m);
//     list[3] = new triangle(g,f,h, m);
//     // left
//     list[4] = new triangle(a,e,b, m);
//     list[5] = new triangle(f,b,e, m);
//     // right
//     list[6] = new triangle(d,c,h, m);
//     list[7] = new triangle(g,h,c, m);
//     // close
//     list[8] = new triangle(g,c,f, m);
//     list[9] = new triangle(g,f,c, m);
//     // far
//     list[10] = new triangle(a,d,e, m);
//     list[11] = new triangle(h,e,d, m);
//     list_ptr = new hit_table_list(list,12);

// }

bool cube::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    return list_ptr->hit(r,t_min,t_max, rec);
}

#endif //CUBE_H_