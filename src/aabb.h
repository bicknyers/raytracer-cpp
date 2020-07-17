#ifndef AABB_H
#define AABB_H

#include "utility.h"

class aabb {
public:
  aabb() {}
  aabb( const point3& a, const point3& b ) { _min = a; _max = b;}

  point3 min() const {return _min;}
  point3 max() const {return _max;}

  bool hit(const ray& r, double tmin, double tmax) const{
    for (int i = 0; i < 3; i++) {
      auto t0 = fmin( (_min[i] - r.origin()[i]) / r.direction()[i],
                      (_max[i] - r.origin()[i]) / r.direction()[i] );
      auto t1 = fmax( (_min[i] - r.origin()[i]) / r.direction()[i],
                      (_max[i] - r.origin()[i]) / r.direction()[i] );
      tmin = fmax(t0, tmin);
      tmax = fmin(t1, tmax);
      if (tmax <= tmin) {return false;}
    }
    return true;
  }

  bool hit(point3 center) {
    for (int i = 0; i < 3; i++) {
      if ( _min[i] > center[i] || center[i] > _max[i] ) {return false;}
    }
    return true;
  }

public:
  point3 _min;
  point3 _max;
};

aabb surrounding_box(aabb box0, aabb box1) {
  vec3 small(fmin(box0.min().x(), box1.min().x()),
               fmin(box0.min().y(), box1.min().y()),
               fmin(box0.min().z(), box1.min().z()));

  vec3 big(fmax(box0.max().x(), box1.max().x()),
             fmax(box0.max().y(), box1.max().y()),
             fmax(box0.max().z(), box1.max().z()));

  return aabb(small,big);
}

#endif
