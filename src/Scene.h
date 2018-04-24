#ifndef SCENE_H
#define SCENE_H

#include "Object.h"
#include <list>
#include "BoundingBox.h"

class Scene {
    public:
        Scene();
        ~Scene();

        void adaptBoundingBox();

        BoundingBox boundingBox;

        std::list<Object *> objects;
};

#endif
