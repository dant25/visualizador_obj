#include "Object.h"
#include <cmath>
#include <iostream>


Object::Object()
{

}

Object::Object( ObjImporter importer )
{
    for( unsigned int i = 0; i < importer.vertices.size(); i++)
    {
        Vec3 v;
        v.x =  importer.vertices[i].x;
        v.y =  importer.vertices[i].y;
        v.z =  importer.vertices[i].z;
        vertexs.push_back(v);
    }

    for( unsigned int i = 0; i < importer.faces.size(); i++)
    {
        Triangle t;
        t.v1 =  importer.faces[i].x;
        t.v2 =  importer.faces[i].y;
        t.v3 =  importer.faces[i].z;
        triangles.push_back(t);
    }

    for( unsigned int i = 0; i < importer.normals.size(); i++)
    {
        Vec3 v;
        v.x =  importer.normals[i].x;
        v.y =  importer.normals[i].y;
        v.z =  importer.normals[i].z;
        normals.push_back(v);
    }

    for( unsigned int i = 0; i < importer.normals_faces.size(); i++)
    {
        Triangle ids;
        ids.v1 =  importer.normals_faces[i].x;
        ids.v2 =  importer.normals_faces[i].y;
        ids.v3 =  importer.normals_faces[i].z;
        normals_triangles.push_back(ids);
    }

    Vec3 vmin = Vec3(importer.minX, importer.minY, importer.minZ);
    Vec3 vmax = Vec3(importer.maxX, importer.maxY, importer.maxZ);

    boundingBox = BoundingBox(vmin, vmax);
}

Object::~Object()
{

}
