#include "Scene.h"
#include <iostream>

Scene::Scene() {

}

Scene::~Scene() {

}

void Scene::adaptBoundingBox()
{
    Vec3 pMin = objects.front()->boundingBox.pMin;
    Vec3 pMax = objects.front()->boundingBox.pMax;


    for (std::list<Object *>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        Object *o = (*it);

        if( o->boundingBox.pMin.x < pMin.x ) pMin.x = o->boundingBox.pMin.x;
        if( o->boundingBox.pMin.y < pMin.z ) pMin.y = o->boundingBox.pMin.y;
        if( o->boundingBox.pMin.y < pMin.z ) pMin.z = o->boundingBox.pMin.z;

        if( o->boundingBox.pMax.x < pMin.x ) pMax.x = o->boundingBox.pMax.x;
        if( o->boundingBox.pMax.y < pMin.z ) pMax.y = o->boundingBox.pMax.y;
        if( o->boundingBox.pMax.y < pMin.z ) pMax.z = o->boundingBox.pMax.z;
    }
    this->boundingBox = BoundingBox(pMin, pMax);
}
