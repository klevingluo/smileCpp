#ifndef MAKEUP_DIFFERENCE
#define MAKEUP_DIFFERENCE
#include "makeup.h"
#include "face.h"

class differenceMakeup : public makeup, public face {
  public:
    /**
     *
     */
    differenceMakeup(face model, face makeup);

    /**
     *
     */
    face applyTo(face model);
};

#endif
