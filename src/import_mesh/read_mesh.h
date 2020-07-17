#ifndef READ_MESH_H
#define READ_MESH_H

#include "../hittable_list.h"
#include "../vec3.h"
#include "triangle.h"

#include <vector>
#include <algorithm>

class read_mesh {
public:
  //read_mesh() {}
  read_mesh(hittable_list& world, shared_ptr<material> m, double scale, point3 origin) {scene = world; scale_factor = scale; base = origin; mat_ptr=m;}

  void add_mesh () {
    std::vector<point3> mesh(17);
    // While reading this vertex data from an ASCII encoded .obj would be simple enough, we are doing a quick implementation for now
    // since we want to eventually read faces and normals from the .obj as well so we are not taking (vertex choose 3) triangles in the next step
    mesh[0] = point3(0.707107, 4.274188, 0.707107);
    mesh[1] = point3(0.000000, 4.274188, 1.000000);
    mesh[2] = point3(-0.707107, 4.274188, 0.707107);
    mesh[3] = point3(-1.000000, 4.274188, -0.000000);
    mesh[4] = point3(-0.707107, 4.274188, -0.707107);
    mesh[5] = point3(0.000000, 4.274188, -1.000000);
    mesh[6] = point3(0.707107, 4.274188, -0.707107);
    mesh[7] = point3(1.000000, 4.274188, -0.000000);
    mesh[8] = point3(0.000000, 2.891626, 0.000000);
    mesh[9] = point3(0.206897, 4.560777, 0.491162);
    mesh[10] = point3(0.494082, 4.560777, 0.199848);
    mesh[11] = point3(0.493688, 4.560777, -0.200798);
    mesh[12] = point3(0.205299, 4.560777, -0.491824);
    mesh[13] = point3(-0.203041, 4.560777, -0.492759);
    mesh[14] = point3(-0.492516, 4.560777, -0.203627);
    mesh[15] = point3(-0.490883, 4.560777, 0.207570);
    mesh[16] = point3(-0.202511, 4.560777, 0.492978);

    // Normalize to range (-scale, scale), then add origin
    for (int i=0; i < 17; i++) {
      mesh[i] = (mesh[i]/5)*scale_factor;
      mesh[i] = mesh[i]+base;
    }

    int V[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

    // Add triangles to the scene for every combination of vertices. Not performant for large meshes, but in this case we get 17choose3=680 triangles which is OK
    do {
    scene.add(make_shared<triangle>(mesh[V[0]], mesh[V[1]], mesh[V[2]], mat_ptr));
  } while ( std::next_permutation(V, V+3) );
    return;
  }

public:
  hittable_list scene;
  shared_ptr<material> mat_ptr;
  double scale_factor;
  point3 base;
};


#endif
