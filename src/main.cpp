#include "utility.h"
#include "hittable_list.h"
#include "sphere.h"
#include "color.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"
//#include "import_mesh/octree.h"
#include "import_mesh/read_mesh.h"

#include <sstream>
#include <iostream>
#include <pthread.h>

using namespace std;

/*
// Use current time as seed for random generator, needs high resolution so that threads spawned for the same pixel use different random streams (else they generate the same colors!)
high_resolution_clock::time_point time = high_resolution_clock::now();
auto nanos = duration_cast<nanoseconds>(time.time_since_epoch()).count();
unsigned int seed = int(nanos);
*/

struct thread_data {
  int image_width;
  int image_height;
  camera cam {};
  hittable_list world {};
  int max_depth;
  int i;
  int j;
  int samples_per_pixel;
  color tcolor;
  int t;
  int num_threads;
};

hittable_list random_scene() {
    hittable_list world;

    auto material2 = make_shared<diffuse>(color(0.4, 0.2, 0.1));
    auto mesh = read_mesh(world, material2, 1, point3(0,0,0));
    mesh.add_mesh();

    auto ground_material = make_shared<diffuse>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<diffuse>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));


    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return hittable_list(make_shared<bvh_node>(world, 0.0, 1.0));
}

color ray_color(const ray& r, const hittable& world, int depth) {
  hit_record rec;
  // If we have exceeded the ray bounce limit, no more light is gathered.
  if (depth <= 0) {
    return color(0,0,0);
  }
  if (world.hit(r, 0.001, inf, rec)) {
    ray scattered;
    color attenuation;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * ray_color(scattered, world, depth-1);}
    return color(0,0,0);
    }
  vec3 unit_direction = unit(r.direction());
  auto t = 0.5*(unit_direction.y() + 1.0);
  return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

void * thread_color(void *args) {
  thread_data *tdata=(thread_data *)args;
  int image_width = tdata->image_width;
  int image_height = tdata->image_height;
  int num_threads = tdata->num_threads;
  camera cam = tdata->cam;
  hittable_list world = tdata->world;
  int max_depth = tdata->max_depth;
  int i = tdata->i;
  int j = tdata->j;
  int t = tdata->t;
  int samples_per_pixel = tdata->samples_per_pixel;
  samples_per_pixel = samples_per_pixel/num_threads;
  //Use different random stream than other paralell threads, else, will calculate exact same rays
  srand(t);

  color thread_color(0, 0, 0);
  for (int s = 0; s < samples_per_pixel; ++s) {
    auto u = (i + random_double()) / (image_width-1);
    auto v = (j + random_double()) / (image_height-1);
    ray r = cam.get_ray(u, v);
    thread_color += ray_color(r, world, max_depth);
}
tdata->tcolor=thread_color;
pthread_exit(NULL);
return NULL;
}

int main(int argc, char ** argv) {
//.ppm file format requires everything to be done in UTF-8, on Windows, setup a PowerShell profile
//to ensure PS uses UTF-8 by adding this to a newline: $PSDefaultParameterValues['Out-File:Encoding'] = 'utf8'
std::setlocale(LC_ALL, "en_US.UTF-8");

int image_width = 384;
int samples_per_pixel = 100;
int max_depth = 50;
int num_threads = 4;

if (argc >= 2) {
  image_width = static_cast<int>(clamp(stoi(argv[1]), 384, 3840));
  if (argc >= 3) {
    samples_per_pixel = static_cast<int>(clamp(stoi(argv[2]), 100, 1000));
    if (argc >= 4) {
      max_depth = static_cast<int>(clamp(stoi(argv[3]), 50, 500));
      if (argc >= 5) {
        num_threads = static_cast<int>(clamp(stoi(argv[4]), 4, 100));
      }
    }
  }
}

samples_per_pixel = (samples_per_pixel/num_threads)*num_threads;
const auto aspect_ratio = 16.0/9.0;
const int image_height = static_cast<int>(image_width/aspect_ratio);
point3 lookfrom(13, 2, 3);
point3 lookat(0, 0, 0);
vec3 vup(0, 1, 0);
auto dist_to_focus = 10.0;
auto aperture = 0.1;
pthread_t threads[num_threads];
pthread_attr_t attr;
struct thread_data tdata[num_threads];

std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
auto world = random_scene();

camera cam( lookfrom, lookat, vup, 20, aspect_ratio, dist_to_focus, aperture );

for (int j = image_height-1; j>= 0; --j) {
  std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
  for (int i=0; i < image_width; ++i) {
    // Initialize and set thread joinable
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   for( int k = 0; k < num_threads; k++ ) {
     tdata[k].image_width=image_width;
     tdata[k].image_height=image_height;
     tdata[k].cam=cam;
     tdata[k].world=world;
     tdata[k].max_depth=max_depth;
     tdata[k].i=i;
     tdata[k].j=j;
     tdata[k].samples_per_pixel=samples_per_pixel;
     tdata[k].t=k;
     tdata[k].num_threads=num_threads;
     pthread_create(&threads[k], &attr, thread_color, (void *)&tdata[k]);
    }

    color pixel_color(0, 0, 0);
    for ( int m = 0; m < num_threads; m++ ) {
    pthread_join(threads[m], NULL);
    pixel_color+=tdata[m].tcolor;
    }
    write_color(std::cout, pixel_color, samples_per_pixel);
  }
}
std::cerr << "\nDone.\n";
}
