#ifndef VIEWER_H
#define VIEWER_H

#include "opengl.h"
#include "shader.h"
#include "camera.h"
#include "trackball.h"
#include "mesh.h"

#include <iostream>

class Viewer{
public:
    //! Constructor
    Viewer();
    virtual ~Viewer();

    // gl stuff
    void init(int w, int h);
    void drawScene();
    void updateAndDrawScene();
    void reshape(int w, int h);
    void loadShaders();

    // events
    void mousePressed(GLFWwindow* window, int button, int action);
    void mouseMoved(int x, int y);
    void mouseScroll(double x, double y);
    void keyPressed(int key, int action, int mods);
    void charPressed(int key);
    void drawScene2D();
    void drawScene3D();

private:
    int _winWidth, _winHeight;

    Camera _cam;
    Shader _shader;
    Mesh   _mesh;
    float _zoom;
    Eigen::Vector3f _offset;
    Eigen::Vector3f _viewAngle;
    float _theta;
    float iterations;
    float iterations_moon;
    // Mouse parameters for the trackball
    enum TrackMode
    {
      TM_NO_TRACK=0, TM_ROTATE_AROUND, TM_ZOOM,
      TM_LOCAL_ROTATE, TM_FLY_Z, TM_FLY_PAN
    };
    TrackMode _trackingMode = TM_ROTATE_AROUND;
    Trackball _trackball;
    Eigen::Vector2i _lastMousePos;
};

#endif
