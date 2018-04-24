#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <GL/glu.h>
#include "src/Scene.h"
#include "src/math/Vec3.h"

inline float degrees(float radians) {
    return 180.0 * radians * M_1_PI;
}

inline float radians(float degrees) {
    return degrees * M_PI/180.0;
}

enum CameraMode { ROTATE, PAN };

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    void cameraSetMode(int mode);
    void cameraFit();
    void cameraZoom(float v);

    Scene *s;

private:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    //funcoes que eram do renderer
    void drawBackground();
    void drawAxis(float *m);

    //Camera
    bool spinning, moving;
    int beginx, beginy;
    float clipClose;
    bool perspective;
    float zoomFat;
    float pivot[3];
    float zoomDepth;
    float currquat[4];
    float lastquat[4];
    float fovy;
    float panFactor;
    void loadProjection();
    int cameraMode;
};

#endif // GLWIDGET_H
