#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../utility.h"
#include "../hittable.h"
#include "../vec3.h"


class triangle: public hittable {
public:
  triangle() {}
  triangle(point3 p1, point3 p2, point3 p3, shared_ptr<material> m) { v1=p1; v2=p2; v3=p3; mat_ptr=m; };

  virtual bool hit (const ray& r, double tmin, double tmax, hit_record& rec) const;
  virtual bool bounding_box(double t0, double t1, aabb& output_box) const;

public:
  point3 v1;
  point3 v2;
  point3 v3;
  shared_ptr<material> mat_ptr;
};

bool triangle::bounding_box(double t0, double t1, aabb& output_box) const {
point3 center = (v1 + v2 + v3)/3;
// Find furthest vertex
double radius = (center-v1).length();
double temp = (center-v2).length();
if (temp > radius) {radius = temp;}
temp = (center-v3).length();
if (temp > radius) {radius = temp;}
// Create bounding box
output_box = aabb( center - vec3(radius, radius, radius),
                   center + vec3(radius, radius, radius) );
return true;
}

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
// Implements the Moller-Trumbore intersection algorithm
double tolerance = 0.0001;
vec3 head = r.origin() + (r.direction() * t_max);
vec3 tail = r.origin() + (r.direction() * t_min);

vec3 e1 = v2-v1;
vec3 e2 = v3-v1;

vec3 h = cross(head, e2);
double a = dot(e1, h);
// Check if ray is paralell to triangle
if (a > -tolerance && a < tolerance) {
      return false;
}

double f = 1.0/a;
vec3 s = tail-v1;
double u = f * dot(s, h);
if (u < 0.0 || u > 1.0) {
  return false;
}

vec3 q = cross(s, e1);
float v = f * dot(head, q);
if (v < 0.0 || u + v > 1.0) {
  return false;
}

double t = f * dot(e2, q);
if (t > tolerance) {
  // Ray intersection
  return true;
}   else  {
      // Path intersection but not a ray intersection
      return false;
    }
}


#endif
