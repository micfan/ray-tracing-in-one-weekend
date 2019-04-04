#include <iostream>
#include <string>
#include <fstream>
#include <cfloat>
#include "vec3.hpp"
#include "ray.hpp"
#include "hitable.hpp"
#include "hitable_list.hpp"
#include "sphere.hpp"


using namespace std;

vec3 color(const ray& r, std::shared_ptr<hitable> world)
{
    hit_record rec;
    if (world->hit(r, 0.0, DBL_MAX, rec)) {
        return 0.5 * vec3{
            rec.normal.x() + 1,
            rec.normal.y() + 1,
            rec.normal.z() + 1
        };
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        double t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3{1.0, 1.0, 1.0} + t * vec3{0.5, 0.7, 1.0};
    }
}

int main() {
    auto filename = "ch5.part2.ppm";
    std::ofstream out(filename);
    cout << "output image: " << filename << endl;

    vec3 lower_left_corner{-2.0, -1.0, -1.0};
    vec3 horizontal{4.0, 0.0, 0.0};
    vec3 vertical{0.0, 2.0, 0.0};
    vec3 origin{0.0, 0.0, 0.0};

    auto list = std::vector<hitable_ptr>();
    list.push_back(std::make_shared<sphere>(vec3{0, 0, -1}, 0.5));
    list.push_back(std::make_shared<sphere>(vec3{0, -100.5, -1}, 100));

    auto world = std::make_shared<hitable_list>(list, 2);

    int nx = 200;
    int ny = 100;
    out << "P3\n" << nx << " " << ny << "\n255\n";

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            double u = double(i) / double(nx);
            double v = double(j) / double(ny);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);

            vec3 p = r.point_at_parameter(2.0);
            vec3 col = color(r, world);

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            out << ir << " " << ig << " " << ib << "\n";
        }
    }
    out.close();

    return EXIT_SUCCESS;
}