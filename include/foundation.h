#ifndef MAKEUP_FOUNDATION
#define MAKEUP_FOUNDATION

#include "makeup.h"
#include "face.h"

class foundation : public makeup {
  public:
    foundation(int R, int B, int G);
                      
    face applyTo(face, double = 1.0);
};

#endif
