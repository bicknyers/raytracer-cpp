#ifndef RGB_TO_YCBCR_H
#define RGB_TO_YCBCR_H

#include "vec3.h"


color rgb_to_ycbcr (color rgb) {
  // From JPEG conversion on YCbCr wikipedia page
  double y = 0 + (0.299 * rgb.x()) + (0.587 * rgb.y()) + (0.114 * rgb.z());
  double cb = 128 - (0.168736 * rgb.x()) - (0.331264 * rgb.y()) + (0.5 * rgb.z());
  double cr = 128 + (0.5 * rgb.x()) - (0.418688 * rgb.y()) - (0.081312 * rgb.z());;

  color ycbcr = color( static_cast<int>(256 * clamp(y, 0.0, 0.999)),
                       static_cast<int>(256 * clamp(cb, 0.0, 0.999)),
                       static_cast<int>(256 * clamp(cr, 0.0, 0.999)) );

  return ycbcr;
}

#endif
