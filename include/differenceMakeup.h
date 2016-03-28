#ifndef MAKEUP_DIFFERENCE
#define MAKEUP_DIFFERENCE
#include "makeup.h"
#include "face.h"

class differenceMakeup : public makeup, public face {
  public:
    /**
     * creates the makeup using a before and an after image
     */
    differenceMakeup(face, face);

    /**
     * applies this makeup to the model with the specified weight
     */
    face applyTo(face, double = 1.0);

};
#endif
