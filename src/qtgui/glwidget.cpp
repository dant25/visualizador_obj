#include "glwidget.h"
#include <iostream>
#include "src/utility.h"
#include "trackball.h"
#include <stdio.h>

inline void glVertexVector( const Vec3& v )
{
#if (GALILEU_PRECISION == 1)
    glVertex3f( v.x, v.y, v.z );
#else
    glVertex3d( v.x, v.y, v.z );
#endif
}




GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    cameraMode = ROTATE;
    spinning = moving = false;
    zoomFat = 1.0;
    zoomDepth = 50.0;
    pivot[0] = pivot[1] = pivot[2] = 0.0;
    trackball(currquat, 0.0, 0.0, 0.0, 0.0);
    clipClose = 0.01;
    fovy = 60.0;
    panFactor = tan(radians(fovy) * 0.5);
    perspective = true;

    s = NULL;
}

void GLWidget::initializeGL()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable( GL_DEPTH_TEST );
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable( GL_POLYGON_SMOOTH );
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(2.0);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    //Inicialização da luz
    float lambient_model[4] = { 0.4, 0.4, 0.4, 1.0};
    //float a[4] = { 0.5, 0.5, 0.5, 1.0};
    //float d[4] = { 0.8, 0.8, 0.8, 0.80};
    //float s[4] = { 1.0, 1.0, 1.0, 1.0};
    //float p[4] = { 100.0, 100.0, 120.0, 0.0};

    //glLightfv(GL_LIGHT0, GL_AMBIENT, a);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, d);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, s);
    //glLightfv(GL_LIGHT0, GL_POSITION, p);
    glEnable(GL_LIGHT0);

    glEnable(GL_LIGHT_MODEL_AMBIENT);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lambient_model);

    //Inicialização do viewport
    int w = width(), h = height();
    glViewport(0, 0, (GLint) w , (GLint) h );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    loadProjection();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawBackground();

    if(perspective)
        glTranslatef(0.0, 0.0, -zoomDepth);
    float m[16];
    build_rotmatrix(m, currquat);
    glMultMatrixf(m);
    glTranslatef(-pivot[0], -pivot[1], -pivot[2]);


    ///AQUI É RENDEZIDO OS OBJETOS DA CENA
    for (std::list<Object *>::iterator it = s->objects.begin(); it != s->objects.end(); it++)
    {
        Object *o = (*it);

        glColor3f(0.7, 0.7, 0.2);

        glBegin(GL_TRIANGLES );

        for( unsigned int i = 0; i < o->triangles.size(); i++)
        {
            glNormal3d( o->normals[ o->normals_triangles[ i ].v1 ].x,
                        o->normals[ o->normals_triangles[ i ].v1 ].y,
                        o->normals[ o->normals_triangles[ i ].v1 ].z );

            glVertex3d( o->vertexs[ o->triangles[i].v1 ].x,
                    o->vertexs[ o->triangles[i].v1 ].y,
                    o->vertexs[ o->triangles[i].v1 ].z );

            glNormal3d( o->normals[ o->normals_triangles[ i ].v2 ].x,
                        o->normals[ o->normals_triangles[ i ].v2 ].y,
                        o->normals[ o->normals_triangles[ i ].v2 ].z );

            glVertex3d( o->vertexs[ o->triangles[i].v2 ].x,
                    o->vertexs[ o->triangles[i].v2 ].y,
                    o->vertexs[ o->triangles[i].v2 ].z );

            glNormal3d( o->normals[ o->normals_triangles[ i ].v3 ].x,
                        o->normals[ o->normals_triangles[ i ].v3 ].y,
                        o->normals[ o->normals_triangles[ i ].v3 ].z );

            glVertex3d( o->vertexs[ o->triangles[i].v3 ].x,
                    o->vertexs[ o->triangles[i].v3 ].y,
                    o->vertexs[ o->triangles[i].v3 ].z );
        }

        glEnd();
    }


    drawAxis(m);
}

//TODO manter aspect ratio
void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    loadProjection();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        beginx = event->x();
        beginy = event->y();

        if(cameraMode == PAN)
            moving = true;
        else
            spinning = true;
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event){
    float width = size().width(), height = size().height();
    if(spinning) {
        trackball(lastquat,
                  (2.0 * beginx - width) / width,
                  (height - 2.0 * beginy) / height,
                  (2.0 * event->x() - width) / width,
                  (height - 2.0 * event->y()) / height);
        beginx = event->x();
        beginy = event->y();
        add_quats(lastquat, currquat, currquat);
        updateGL();
    }
    else if(moving) {
        float modelview[16];
        glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

        float dx, dy;
        if(perspective) {
            dx = (((2.0 * beginx - width) / width) - ((2.0 * event->x() - width) / width)) * zoomDepth * width/height * panFactor;
            dy = (((height - 2.0 * beginy) / height) - ((height - 2.0 * event->y()) / height)) * zoomDepth * panFactor;
        }
        else {
            dx = (((2.0 * beginx - width) / width) - ((2.0 * event->x() - width) / width)) * zoomFat * width/height;
            dy = (((height - 2.0 * beginy) / height) - ((height - 2.0 * event->y()) / height)) * zoomFat;
        }

        beginx = event->x();
        beginy = event->y();

        modelview[0] *= dx;
        modelview[4] *= dx;
        modelview[8] *= dx;

        modelview[1] *= dy;
        modelview[5] *= dy;
        modelview[9] *= dy;

        pivot[0] += modelview[0];
        pivot[1] += modelview[4];
        pivot[2] += modelview[8];

        pivot[0] += modelview[1];
        pivot[1] += modelview[5];
        pivot[2] += modelview[9];

        updateGL();
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
        moving = spinning = false;
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier) {
        float ds = event->delta()*0.01;
        clipClose += ds;
        clipClose =  clipClose < 0.01 ? 0.01 : (clipClose > 1000.0 ? 1000.0 : clipClose);
    }
    else {
        //event->delta() no meu computador tá sempre com um valor de 120, assim multiplicamos por 0.001 para ter 0.12
        //o zoomDepth vai sempre diminuir 12%
        float ds = event->delta()*0.001;
        zoomDepth -= ds*zoomDepth;
        //FIXME proibir que zoom depth fique pequeno a ponto de ver o interior da malha?
        zoomDepth = zoomDepth < 0.0 ? 0.0 : zoomDepth;

        zoomFat -= ds;
        zoomFat = zoomFat < 0.0 ? 0.0 : zoomFat;
    }
    loadProjection();
    updateGL();
}

//FIXME: ajeitar essa função
void GLWidget::cameraSetMode(int mode){
    cameraMode = mode;
}

void GLWidget::drawBackground() {
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
    glColor3f(0.7,0.7,0.7);
    glVertex2f(-1.0,-1.0);
    glVertex2f(1.0,-1.0);
    glColor3f(0.4,0.4,0.4);
    glVertex2f(1.0,1.0);
    glVertex2f(-1.0,1.0);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::drawAxis(float *m) {
    static struct Axis {
        Axis() {
            id = glGenLists(1);
            glNewList(id, GL_COMPILE);

            glLineWidth(2.0);
            glBegin(GL_LINES);
            glColor3f(1.0, 0.9, 0.9);
            glVertex3f(0.0, 0.0, 0.0);
            glColor3f(0.8, 0.0, 0.0);
            glVertex3f(1.0, 0.0, 0.0);

            glColor3f(0.9, 1.0, 0.9);
            glVertex3f(0.0, 0.0, 0.0);
            glColor3f(0.0, 0.8, 0.0);
            glVertex3f(0.0, 1.0, 0.0);

            glColor3f(0.9, 0.9, 1.0);
            glVertex3f(0.0, 0.0, 0.0);
            glColor3f(0.0, 0.0, 0.8);
            glVertex3f(0.0, 0.0, 1.0);
            glEnd();

            glEndList();
        }
        int id;
    } axis;

    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT | GL_LINE_BIT);
    glDisable(GL_LIGHTING);
    glDisable( GL_DEPTH_TEST );
    glViewport(0, 0, 80.0, 80.0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho (-1.5, 1.5, -1.5, 1.5, -1.199, 1.199);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(m);

    glCallList(axis.id);

    glColor3f(1.0, 0.0, 0.0);
    renderText(1.2, 0.0, 0.0, "x");
    glColor3f(0.0, 1.0, 0.0);
    renderText(0.0, 1.2, 0.0, "y");
    glColor3f(0.0, 0.0, 1.0);
    renderText(0.0, 0.0, 1.2, "z");

    glEnable(GL_MULTISAMPLE); //rendertText desabilita o multisample.

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::cameraFit() {
    //reset na camera
    pivot[0] = pivot[1] = pivot[2] = 0.0;
    trackball(currquat, 0.0, 0.0, 0.0, 0.0);
    clipClose = 0.01;

    float dy = s->boundingBox.pMax.y - s->boundingBox.pMin.y;
    float dx = s->boundingBox.pMax.x - s->boundingBox.pMin.x;
    float dz = s->boundingBox.pMax.z - s->boundingBox.pMin.z;
    float length = dy > dx ? dy : dx;
    length = dz > length ? dz : length;
    zoomDepth = length/tan(radians(fovy*0.5));

    pivot[0] = s->boundingBox.center.x;
    pivot[1] = s->boundingBox.center.y;
    pivot[2] = s->boundingBox.center.z;
    loadProjection();
    updateGL();
}

void GLWidget::loadProjection()
{
    float w = width();
    float h = height();
    float ratio = w/h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(perspective)
        gluPerspective(fovy, ratio, clipClose, 20000);
    else
        //    glOrtho(-zoom_fat*ratio, zoom_fat*ratio, -zoom_fat, zoom_fat, -20000.0, 20000.0);
        //else
        //    glOrtho(-zoom_fat, zoom_fat, -zoom_fat/ratio, zoom_fat/ratio, -20000.0, 20000.0);
        glOrtho(0, w, 0, h, -1, 1);
}

void GLWidget::cameraZoom(float v) {
    zoomDepth -= v*zoomDepth;
    //FIXME proibir que zoom depth fique pequeno a ponto de ver o interior da malha?
    zoomDepth = zoomDepth < 0.0 ? 0.0 : zoomDepth;

    zoomFat -= v;
    zoomFat = zoomFat < 0.0 ? 0.0 : zoomFat;
    updateGL();
}
