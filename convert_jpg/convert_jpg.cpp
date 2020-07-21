#include "read_ppm.h"
#include "rgb_to_ycbcr.h"
#include "vec3.h"

#include <iostream>


int main(int argc, char ** argv) {

  // .ppm file format requires everything to be done in UTF-8, on Windows, setup a PowerShell profile
  //to ensure PS uses UTF-8 by adding this to a newline: $PSDefaultParameterValues['Out-File:Encoding'] = 'utf8'
  std::setlocale(LC_ALL, "en_US.UTF-8");

  // Read image
  color ** image = read_ppm("");

  int image_width;
  int image_height;

  // Convert from RGB to YCbCr
  for (int j=image_height-1; j>= 0; --j) {
    for (int i=0; i < image_width; ++i) {
      image[i][j] = rgb_to_ycbcr(image[i][j]);
    }
  }

  // Testing:
  write_ppm(image);

  // Discrete Cosine Transform

  // Quantization

  // Huffman Encoding

  // Write to jpg

}
