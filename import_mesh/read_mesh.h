#ifndef READ_MESH_H
#define READ_MESH_H

#include "../hittable_list.h"
#include "../vec3.h"
#include "triangle.h"

#include <numeric>
#include <vector>
#include <algorithm>
#include <cstring>

class read_mesh {
  public:
    read_mesh(std::string filepath, hittable_list * world, shared_ptr<material> m, double scale, point3 origin) {path=filepath; scene=world; scale_factor=scale; base=origin; mat_ptr=m;}

  void add_mesh () {
    // Start read from .obj ASCII file
    FILE * file = fopen(path.c_str(), "r");
    if ( file == NULL) {std::cerr << "File not found!\n"; return;}

    // Hold the triangles and faces defined by .obj
    std::vector<point3> mesh_tri;
    std::vector<std::vector<int>> mesh_face;

    // Temporary variables for scanning process
    char lineHeader[128];
    int result;
    std::vector<float> temp_vertex(3);
    std::vector<int> temp_face(3);
    int temp;

    // Scan file
    while (1) {
      // Check if End-Of-File
      result = fscanf(file, "%s", lineHeader);
      if (result == EOF) {break;}

      if ( strcmp(lineHeader, "v") == 0 ) {
        // Scan vertices
        fscanf(file, "%f %f %f\n", &temp_vertex[0], &temp_vertex[1], &temp_vertex[2] );
        mesh_tri.push_back( point3(temp_vertex[0], temp_vertex[1], temp_vertex[2]) );
      } else if ( strcmp(lineHeader, "f") == 0 ) {
        // Scan faces
        fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &temp_face[0], &temp, &temp, &temp_face[1], &temp, &temp, &temp_face[2], &temp, &temp);
        mesh_face.push_back(temp_face);
      }
    }

    // Close file
    fclose(file);

    // Normalize to range (-scale, scale), then add origin
    for (int i=0; i < mesh_tri.size(); i++) {
      mesh_tri[i] = mesh_tri[i]*scale_factor;
      mesh_tri[i] = mesh_tri[i]+base;
    }

    // Add triangles to the scene as defined by the faces
    for (int j=0; j < mesh_face.size(); j++) {
    scene->add( make_shared<triangle>( mesh_tri[ mesh_face[j][0]-1 ], mesh_tri[ mesh_face[j][1]-1 ], mesh_tri[ mesh_face[j][2]-1 ], mat_ptr ) );
    }
    return;
  }

public:
  std::string path;
  hittable_list * scene;
  shared_ptr<material> mat_ptr;
  double scale_factor;
  point3 base;
};


#endif
