#ifndef READ_PPM_H
#define READ_PPM_H

#include "vec3.h"

#include <iostream>


color ** read_ppm (string filepath) {

// Do Stuff

return;
}

void write_ppm (color ** image) {
int image_width;
int image_height;

std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

for (int j=image_height-1; j>= 0; --j) {
  for (int i=0; i < image_width; ++i) {
  out << image[i][j].x() << ' '
      << image[i][j].y() << ' '
      << image[i][j].z() << '\n';
    }
}

return;
}

#endif
