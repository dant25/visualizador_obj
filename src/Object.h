#ifndef _Object_H_
#define _Object_H_

#include <vector>
#include "math/Point.h"
#include "math/Vec3.h"
#include "importer.h"
#include "BoundingBox.h"

struct Triangle {
   unsigned int v1;
   unsigned int v2;
   unsigned int v3;
};

class Object{
    public:
        Object();
        Object( ObjImporter importer );
        ~Object();

        std::vector<Triangle> triangles;
        std::vector<Vec3> vertexs;
        std::vector<Vec3> normals;
        std::vector<Triangle> normals_triangles;
        BoundingBox boundingBox;
};

#endif
