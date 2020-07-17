#ifndef OCTREE_H
#define OCTREE_H

#include "../utility.h"
#include "../hittable.h"

#include <algorithm>



class octree : public hittable {
public:
  octree();

  octree(hittable_list& list, double time0, double time1) : octree(list.objects, 0, list.objects.size(), time0, time1) {}

  octree(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end, double time0, double time1);

  octree(aabb node_box) {box = node_box;}

  virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
  virtual bool bounding_box(double t0, double t1, aabb& output_box) const;

public:
  // We want a vector of 8 children to encode the octree structure, then a variable length vector for storing objects at the leaves of the tree
  std::vector<shared_ptr<hittable>> children;
  aabb box;
};



/*
inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(0,0, box_a) || !b->bounding_box(0,0, box_b))
        std::cerr << "No bounding box in octree constructor.\n";

    return box_a.min().e[axis] < box_b.min().e[axis];
}
*/




/*
bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}
*/



octree::octree(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end, double time0, double time1) {
  // Creates top level
  box = aabb( point3(time0,time0,time0) , point3(time1,time1,time1) )
  // Layer 2
  children = octree_node(time0, time1);
  double child_time0; child_time1;

  // Final layer (3), children of these boxes contain the objects themselves
  for (int i = 0; i < 7; i++) {
    // Grab the points used from earlier for further subdivision
    child_time0 = children[i].box.min();
    child_time1 = children[i].box.max();
    children[i].children = octree_node(child_time0, child_time1);
  }

  // Now we want to organize objects into boxes
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      for (int k = start; k < end; k++) {
        // If an object intersects with a bounding box, it gets stored as a leaf. Multiple copies of objects are OK
        if (children[i].children[j].box.hit(objects[k].center) { children[i].children[j].children.insert(objects[k]); }
      }
    }
  }
  // End
}


std::vector<shared_ptr<hittable>> octree_node(double time0, double time1) {
  // Make root node from 0-1 on all axis (covers entire scene)
  double mid = (time1-time0)/2.0;

  // Initialize 8 for encoding structure
  std::vector<shared_ptr<hittable>> children(8);

  // Obviously this is not the elegant way to do this, but it will work for now.
    // 3 choose 0 = 1
  children[0] = octree( aabb( point3(time0,time0,time0) , point3(mid,mid,mid) ) );
    // 3 choose 1 = 3
  children[1] = octree( aabb( point3(mid,time0,time0) , point3(time1,mid,mid) ) );
  children[2] = octree( aabb( point3(time0,mid,time0) , point3(mid,time1,mid) ) );
  children[3] = octree( aabb( point3(time0,time0,mid) , point3(mid,mid,time1) ) );
    // 3 choose 2 = 3
  children[4] = octree( aabb( point3(mid,mid,time0) , point3(time1,time1,mid) ) );
  children[5] = octree( aabb( point3(mid,time0,mid) , point3(time1,mid,time1) ) );
  children[6] = octree( aabb( point3(time0,mid,mid) , point3(mid,time1,time1) ) );
    // 3 choose 3 = 1
  children[7] = octree( aabb( point3(mid,mid,mid) , point3(time1,time1,time1) ) );

  return children;
}


/*
int axis = random_int(0,2);
auto comparator = (axis == 0) ? box_x_compare
                : (axis == 1) ? box_y_compare
                : box_z_compare;
size_t object_span = end - start;

if (object_span == 1) {
  left = right = objects[start];
} else if (object_span == 2) {
  if (comparator(objects[start],objects[start+1])) {
    left = objects[start];
    right = objects[start+1];
  } else {
    left = objects[start+1];
    right = objects[start];
  }
} else {
  std::sort(objects.begin() + start, objects.begin() + end, comparator);

  auto mid = start + object_span/2;
  left = make_shared<octree_node>(objects, start, mid, time0, time1);
  right = make_shared<octree_node>(objects, mid, end, time0, time1);
}

aabb box_left, box_right;
if ( !left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right) ) {
  std::cerr << "No bounding box in octree constructor.\n";
}
box = surrounding_box(box_left, box_right);
}

*/


bool octree::bounding_box(double t0, double t1, aabb& output_box) const {
    output_box = box;
    return true;
}


//TODO rewrite
bool octree::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if (!box.hit(r, t_min, t_max)) {return false;}

    int k_max;
    // Loop through child arrays
    for (int i = 0; i < 7; i++) {
      // Does ray hit any boxes in layer 1?
      if ( static_cast<octree>(children[i]).box.hit(r, t_min, t_max) ) {
        for (int j = 0; j < 7; j++) {
          // Does ray hit any boxes in layer 2?
          if ( static_cast<octree>(children[i]).(static_cast<octree>(children[j])).box.hit(r, t_min, t_max) ) {
            k_max = static_cast<octree>(children[i]).(static_cast<octree>(children[j])).children.size();
            // Non-empty?
            if (k_max > 0) {
              for (int k = 0; k < k_max; k++) {
                // Does ray hit any objects at the bottom of the octree?
                if ( static_cast<octree>children[i].static_cast<octree>children[j].children[k].hit(r, t_min, t_max) ) {return true;}
              }
            }
          }
        }
      }
    }
    // Worst-case performance on complete miss, 49*(average_objects_per_bucket)
    return false;
}



#endif
