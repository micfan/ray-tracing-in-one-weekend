#pragma once

#include "vec3.hpp"
#include "ray.hpp"

class camera
{
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect)
	{
		double theta = vfov*M_PI/180;
		double half_height = tan(theta/2);
		double half_width = aspect * half_height;

		origin = lookfrom;
		vec3 w = unit_vector(lookfrom - lookat);
		vec3 u = unit_vector(cross(vup, w));
		vec3 v = cross(w, u);

		lower_left_corner = vec3{-half_width, -half_height, -1.0};
		lower_left_corner = origin - half_width*u - half_height*v - w;
		horizontal = 2*half_width*u;
		vertical = 2*half_height*v;
	}

    ray get_ray(double u, double v)
    {
        return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};

