#ifndef HITTABLELIST_H
#define HITTABLELIST_H
#include "hittable.h"

class hit_table_list : public hit_table
{
public:
    hit_table_list() {}
    hit_table_list(hit_table **l, int n)
    {
        list = l;
        list_size = n;
    }
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
    hit_table **list;
    int list_size;
};

bool hit_table_list::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size; i++)
    {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

#endif //HITTABLE_LIST_H_