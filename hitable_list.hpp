#pragma once

#include <vector>
#include <memory>
#include "hitable.hpp"

using hitable_ptr = std::shared_ptr<hitable>;
using hitbale_vector = std::vector<hitable_ptr>;

class hitable_list : public hitable
{
public:
    hitable_list() = default;
    hitable_list(hitbale_vector& l, int n) : list(l), list_size(n) {}

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    hitbale_vector list = hitbale_vector();
    int list_size = 0;
};

bool hitable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size; i++) {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}