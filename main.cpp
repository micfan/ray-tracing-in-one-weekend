#include <iostream>
#include <string>
#include <fstream>
#include <cfloat>
#include <random>
#include <chrono>
#include "vec3.hpp"
#include "ray.hpp"
#include "hitable.hpp"
#include "hitable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "dielectric.hpp"

using namespace std;

vec3 random_in_unit_sphere()
{
    vec3 p;

    long long seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    std::default_random_engine rand_gen(static_cast<unsigned int>(seed));
    std::uniform_real_distribution<> rand_dist(0.0, 1.0);
    do {
        p = 2.0 * vec3{
            rand_dist(rand_gen),
            rand_dist(rand_gen),
            rand_dist(rand_gen)
        } - vec3{1.0, 1.0, 1.0};
    } while (p.squared_length() >= 1.0);

    return p;
}

vec3 color(const ray& r, std::shared_ptr<hitable> world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, DBL_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 &&
            rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return vec3();
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        double t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3{1.0, 1.0, 1.0} + t * vec3{0.5, 0.7, 1.0};
    }
}

int main() {
    auto filename = "ch9.part2.ppm";
    std::ofstream out(filename);
    cout << "output image: " << filename << endl;

    camera cam;

    auto list = std::vector<hitable_ptr>();

    list.push_back(std::make_shared<sphere>(vec3{0, 0, -1}, 0.5, std::make_shared<lambertian>(vec3{0.1, 0.2, 0.5})));
    list.push_back(std::make_shared<sphere>(vec3{0, -100.5, -1}, 100, std::make_shared<lambertian>(vec3{0.8, 0.8, 0.0})));
    list.push_back(std::make_shared<sphere>(vec3{1, 0, -1}, 0.5, std::make_shared<metal>(vec3{0.8, 0.6, 0.2})));
    list.push_back(std::make_shared<sphere>(vec3{-1, 0, -1}, 0.5, std::make_shared<dielectric>(1.5)));

    auto world = std::make_shared<hitable_list>(list, 4);

    int nx = 200;
    int ny = 100;
    int ns = 100;
    out << "P3\n" << nx << " " << ny << "\n255\n";

    std::default_random_engine rand_gen(1);
    std::uniform_real_distribution<> rand_dist(0, 1.0);

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {

            vec3 col{0, 0, 0};

            for (int s = 0; s < ns; s++) {
                double u = double(i + rand_dist(rand_gen)) / double(nx);
                double v = double(j + rand_dist(rand_gen)) / double(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);
            }
            col /= double(ns);
            col = vec3{sqrt(col[0]), sqrt(col[1]), sqrt(col[2])};
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            out << ir << " " << ig << " " << ib << "\n";
        }
    }
    out.close();

    return EXIT_SUCCESS;
}