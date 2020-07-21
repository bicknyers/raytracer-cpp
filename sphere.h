#ifndef SPHERE_H
#define SPHERE_H

#include "utility.h"
#include "hittable.h"

class sphere: public hittable {
public:
  sphere() {}
  sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};

  virtual bool hit (const ray& r, double tmin, double tmax, hit_record& rec) const;
  virtual bool bounding_box(double t0, double t1, aabb& output_box) const;

public:
  point3 center;
  double radius;
  shared_ptr<material> mat_ptr;
};

bool sphere::bounding_box(double t0, double t1, aabb& output_box) const {
output_box = aabb( center - vec3(radius, radius, radius),
                   center + vec3(radius, radius, radius) );
return true;
}

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
  vec3 oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius*radius;
  auto discriminant = half_b*half_b - a*c;

  if (discriminant > 0) {
    auto root = sqrt(discriminant);
    auto temp = (-half_b - root)/a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      vec3 outward_normal = (rec.p - center) / radius;
      rec.set_face_normal(r, outward_normal);
      rec.mat_ptr = mat_ptr;
      return true;
    }
    temp = (-half_b + root)/a;
    if (temp < t_max && temp > t_min ) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      vec3 outward_normal = (rec.p - center) / radius;
      rec.set_face_normal(r, outward_normal);
      rec.mat_ptr = mat_ptr;
      return true;
    }
  }

return false;
}



#endif
