#include "BoundingBox.h"

BoundingBox::BoundingBox(){}

BoundingBox::BoundingBox(const Vec3& pMin, const Vec3& pMax) {
    this->pMin = pMin;
    this->pMax = pMax;
    center = (pMin + pMax)*0.5;
}
