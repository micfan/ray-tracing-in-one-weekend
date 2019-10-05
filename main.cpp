#include <iostream>
#include <string>
#include <fstream>
#include "vec3.hpp"
#include "ray.hpp"

using namespace std;

bool hit_sphere(const vec3& center, double radius, const ray& r)
{
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = 2.0 * dot(oc, r.direction());
    double c = dot(oc, oc) - radius*radius;
    double discriminant = b*b -4*a*c;
    return discriminant > 0;
}

vec3 color(const ray& r)
{
    if (hit_sphere(vec3{0, 0, -1}, 0.5, r))
    {
        return vec3(1, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3{1.0, 1.0, 1.0} + t * vec3{0.5, 0.7, 1.0};
}

int main() {
    auto filename = "ch4.ppm";
    std::ofstream out(filename);
    cout << "output image: " << filename << endl;

    vec3 lower_left_corner{-2.0, -1.0, -1.0};
    vec3 horizontal{4.0, 0.0, 0.0};
    vec3 vertical{0.0, 2.0, 0.0};
    vec3 origin{0.0, 0.0, 0.0};

    int nx = 200;
    int ny = 100;
    out << "P3\n" << nx << " " << ny << "\n255\n";

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            double u = double(i) / double(nx);
            double v = double(j) / double(ny);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            vec3 col = color(r);

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            out << ir << " " << ig << " " << ib << "\n";
        }
    }
    out.close();

    return EXIT_SUCCESS;
}