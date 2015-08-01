#ifndef MAKEUP_DIFFERENCE
#define MAKEUP_DIFFERENCE
#include "makeup.h"
#include "face.h"

class differenceMakeup : public makeup, public face {
  public:
    /**
     *
     */
    differenceMakeup(face, face);

    /**
     * applies this makeup to the model with the specified weight
     */
    face applyTo(face, double);

    /**
     * applies this makeup to the model with full weight
     */
    face applyTo(face);
};

#endif
