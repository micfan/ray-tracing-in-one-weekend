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

std::vector<hitable_ptr> random_scene() {
	auto list = std::vector<hitable_ptr>();
//	hitable **list = new hitable *[n+1];
/*定义一个包含n+1个元素的数组，数组的每个元素是指向hitable对象的指针。然后将数组的指针赋值给list。所以，list是指针的指针。*/
	list.push_back(std::make_shared<sphere>(vec3(0,-1000,0), 1000, std::make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));
/*先创建一个中心在（0，-1000，0）半径为1000的超大漫射球，将其指针保存在list的第一个元素中。*/
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
/*两个for循环中会产生（11+11）*(11+11)=484个随机小球*/
			float choose_mat = (rand()%(100)/(float)(100));
/*产生一个（0，1）的随机数，作为设置小球材料的阀值*/
			vec3 center(a+0.9*(rand()%(100)/(float)(100)), 0.2,
						b+0.9*(rand()%(100)/(float)(100)));
/*” a+0.9*(rand()%(100)/(float)(100))”配合[-11,11]产生（-11，11）之间的随机数，而不是[-11,11)之间的22个整数。使得球心的x,z坐标是（-11，11）之间的随机数*/
			if ((center-vec3(4,0.2,0)).length() > 0.9) {
/*避免小球的位置和最前面的大球的位置太靠近*/
				if (choose_mat < 0.8) {     //diffuse
/*材料阀值小于0.8，则设置为漫反射球，漫反射球的衰减系数x,y,z都是（0，1）之间的随机数的平方*/
					list.push_back( std::make_shared<sphere>(center, 0.2,
															 std::make_shared<lambertian>(vec3(
																 (rand()%(100)/(float)(100))*(rand()%(100)/(float)(100)),
																 (rand()%(100)/(float)(100))*(rand()%(100)/(float)(100)),
																 (rand()%(100)/(float)(100))*(rand()%(100)/(float)(100))))));
				}
				else if (choose_mat < 0.95) {
/*材料阀值大于等于0.8小于0.95，则设置为镜面反射球，镜面反射球的衰减系数x,y,z及模糊系数都是（0，1）之间的随机数加一再除以2*/
					list.push_back( std::make_shared<sphere>(center, 0.2,
															 std::make_shared<metal>(vec3(0.5*(1+(rand()%(100)/(float)(100))),
																						  0.5*(1+(rand()%(100)/(float)(100))),
																						  0.5*(1+(rand()%(100)/(float)(100)))),
																					 0.5*(1+(rand()%(100)/(float)(100))))));
				}
				else {
/*材料阀值大于等于0.95，则设置为介质球*/
					list.push_back( std::make_shared<sphere>(center, 0.2, std::make_shared<dielectric>(1.5)));
				}
			}
		}
	}

	list.push_back( std::make_shared<sphere>(vec3(0, 1, 0), 1.0, std::make_shared<dielectric>(1.5)));
	list.push_back( std::make_shared<sphere>(vec3(-4, 1, 0), 1.0, std::make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));
	list.push_back( std::make_shared<sphere>(vec3(4, 1, 0), 1.0, std::make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));


	return list;
}

int main() {
    auto filename = "ch12.ppm";
    std::ofstream out(filename);
    cout << "output image: " << filename << endl;

	vec3 lookfrom(13,2,3);
	vec3 lookat(0, 0, 0);
    double aperture = 0.0;
    double dist_to_focus = (lookfrom - lookat).length();
    camera cam{
    	lookfrom,
    	lookat,
		vec3(0, 1, 0),
		20,
		double(16)/double(9),
		aperture,
		0.7*dist_to_focus
    };

//    auto list = std::vector<hitable_ptr>();
//
//    list.push_back(std::make_shared<sphere>(vec3{0, 0, -1}, 0.5, std::make_shared<lambertian>(vec3{0.1, 0.2, 0.5})));
//    list.push_back(std::make_shared<sphere>(vec3{0, -100.5, -1}, 100, std::make_shared<lambertian>(vec3{0.8, 0.8, 0.0})));
//    list.push_back(std::make_shared<sphere>(vec3{1, 0, -1}, 0.5, std::make_shared<metal>(vec3{0.8, 0.6, 0.2})));
//    list.push_back(std::make_shared<sphere>(vec3{-1, 0, -1}, 0.5, std::make_shared<dielectric>(1.5)));
//    list.push_back(std::make_shared<sphere>(vec3{-1, 0, -1}, -0.45, std::make_shared<dielectric>(1.5)));

	auto list = random_scene();

    auto world = std::make_shared<hitable_list>(list, 484);

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