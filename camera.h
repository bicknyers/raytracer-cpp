#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"

class camera {
public:
  camera() {
    
  }
  camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio,
    double aperture, double focus_dist) {
    // vfov is field-of-view in degrees
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta/2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;

    auto w = unit(lookfrom - lookat);
    auto u = unit(cross(vup, w));
    auto v = cross(w, u);

    origin = lookfrom;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist * w;

    lens_radius = aperture / 2;

  }
  ray get_ray(double s, double t) const {
    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u * rd.x() + v * rd.y();
    return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
  }
private:
  point3 origin;
  point3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
  double lens_radius;
};

#endif
