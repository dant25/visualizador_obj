#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include "math/Vec3.h"

class BoundingBox {
public:
    BoundingBox();
    BoundingBox(const Vec3& pMin, const Vec3& pMax);

    Vec3 pMin, pMax;
    Vec3 center;
};


#endif

