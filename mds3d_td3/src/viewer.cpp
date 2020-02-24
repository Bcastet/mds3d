#include "viewer.h"
#include "camera.h"

using namespace Eigen;

Viewer::Viewer()
  : _winWidth(0), _winHeight(0)
{
  _zoom  = 1.5;
  //offset = Vector2f(0,0);
  _offset = Vector3f(0,0,0);
  _theta = 0;
  iterations=0;

}

Viewer::~Viewer()
{
}

////////////////////////////////////////////////////////////////////////////////
// GL stuff

// initialize OpenGL context
void Viewer::init(int w, int h){
    loadShaders();
    glClearColor(0.5, 0.5, 0.5, 0.5);
    glEnable(GL_DEPTH_TEST);

    if(!_mesh.load(DATA_DIR"/models/sphere.obj")) exit(1);
    _mesh.initVBA();

    reshape(w,h);
    _trackball.setCamera(&_cam);

  Vector3f pos_cam = Vector3f(1,1,1);
  Vector3f target = Vector3f(0,0,0);
  Vector3f y_vect = Vector3f(0,1,0);
    _cam.lookAt(pos_cam,target,y_vect);
}

void Viewer::reshape(int w, int h){
    _winWidth = w;
    _winHeight = h;
    _cam.setViewport(w,h);
}


/*!
   callback to draw graphic primitiveszoomValue += 0.2;
 */
void Viewer::drawScene()
{
  // TODO

  glViewport(0, 0, _winWidth, _winHeight);
  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);


  _shader.activate();
  glUniform1f(_shader.getUniformLocation("zoom"), _zoom);
  _mesh.draw(_shader);
  _shader.deactivate();


}


void Viewer::drawScene2D()
{
    glViewport(0, 0, _winWidth, _winHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader.activate();

    Matrix4f M;
    M <<  0.5, 0, 0, -0.5,
          0, 0.5, 0, -1,
          0, 0, 0.5, 0,
          0, 0, 0, 1;
    glUniformMatrix4fv(_shader.getUniformLocation("mat_obj"),
    1, GL_FALSE, M.data());
    _mesh.draw(_shader);

    M <<  -0.5, 0, 0, 0.5,
          0, 0.5 , 0, -1,
          0, 0,0.5, 0,
          0, 0, 0, 1;
    glUniformMatrix4fv(_shader.getUniformLocation("mat_obj"),
    1, GL_FALSE, M.data());
    _mesh.draw(_shader);

    Matrix4f R;
    R <<  cos(_theta), -sin(_theta), 0, 0,
          sin(_theta), cos(_theta), 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1;

    Matrix4f T;
    T <<  1, 0, 0, 0,
          0, 1, 0, 0.5,
          0, 0, 1, 0,
          0, 0, 0, 1;

    M = T* R * T.inverse();


    Vector3f t(0,0.5,0);
    Affine3f A = Translation3f(t)
                *AngleAxisf(_theta,Vector3f::UnitZ())
                *Translation3f(-t);


    glUniformMatrix4fv(_shader.getUniformLocation("mat_obj"),
    1, GL_FALSE, A.matrix().data());
    _mesh.draw(_shader);
/*
    M <<  zoomValue, 0, 0, offset.x(),
          0, zoomValue, 0, offset.y(),
          0, 0, zoomValue, 0,
          0, 0, 0, 1;
    glUniformMatrix4fv(_shader.getUniformLocation("obj_mat"),
     1, GL_FALSE, M.data());


    _mesh.draw(_shader);*/
    _shader.deactivate();
}


void Viewer::drawScene3D()
{
  // TODO

  glViewport(0, 0, _winWidth, _winHeight);
  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);


  _shader.activate();
  //Soleil
  Matrix4f M;
  M.setIdentity();
  glUniformMatrix4fv(_shader.getUniformLocation("mat_obj"),
   1, GL_FALSE, M.data());
  glUniformMatrix4fv(_shader.getUniformLocation("mat_view"),
  1, GL_FALSE, _cam.viewMatrix().data());
  glUniformMatrix4fv(_shader.getUniformLocation("mat_perspective"),
  1, GL_FALSE, _cam.projectionMatrix().data());

  _mesh.draw(_shader);

  //Terre
  
  /*Vector3f t(0,0.5,0);
    Affine3f A = Translation3f(t)
                *AngleAxisf(_theta,Vector3f::UnitZ())
                *Translation3f(-t);*/
  Matrix4f T;
  T <<    1, 0, 0, 5,
          0, 1, 0, 0,
          0, 0, 1, 0,
          0, 0, 0, 1;
  Vector3f w; // rotation axis
  w<<0,1,0;
  Vector3f c;
  c<<0,0,0; // center of rotation
  Affine3f A = Translation3f(c) * AngleAxisf(iterations, w) * Translation3f(-c);
  T *= A.matrix();
  glUniformMatrix4fv(_shader.getUniformLocation("mat_obj"),
   1, GL_FALSE, T.data());
  glUniformMatrix4fv(_shader.getUniformLocation("mat_view"),
   1, GL_FALSE, _cam.viewMatrix().data());
   glUniformMatrix4fv(_shader.getUniformLocation("mat_perspective"),
   1, GL_FALSE, _cam.projectionMatrix().data());
  _mesh.draw(_shader);

  //Lune
  
  


  /*glUniformMatrix4fv(_shader.getUniformLocation("mat_view"),
   1, GL_FALSE, _cam.viewMatrix().data());
   glUniformMatrix4fv(_shader.getUniformLocation("mat_perspective"),
   1, GL_FALSE, _cam.projectionMatrix().data());*/

  _mesh.draw(_shader);
  _shader.deactivate();
  iterations+=M_PI/16;

}


void Viewer::updateAndDrawScene()
{
    drawScene3D();
}

void Viewer::loadShaders()
{
    // Here we can load as many shaders as we want, currently we have only one:
    _shader.loadFromFiles(DATA_DIR"/shaders/3dshader.vert", DATA_DIR"/shaders/3dshader.frag");
    checkError();
}

////////////////////////////////////////////////////////////////////////////////
// Events

/*!
   callback to manage keyboard interactions
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::keyPressed(int key, int action, int /*mods*/)
{
  if(key == GLFW_KEY_R && action == GLFW_PRESS)
  {
    loadShaders();
  }

  if(action == GLFW_PRESS || action == GLFW_REPEAT )
  {
    if (key==GLFW_KEY_UP)
    {
      _offset(1)   += 0.2;
    }
    else if (key==GLFW_KEY_DOWN)
    {
      _offset(1) -= 0.2;
    }
    else if (key==GLFW_KEY_LEFT)
    {
      _offset(0) += 0.2;
    }
    else if (key==GLFW_KEY_RIGHT)
    {
      _offset(0) -= 0.2;
    }
    else if (key==GLFW_KEY_PAGE_UP)
    {
      _zoom += 0.2;
      _theta += M_PI/8;
    }
    else if (key==GLFW_KEY_PAGE_DOWN)
    {
      _zoom -= 0.2;
      _theta -= M_PI/8;
    }
  }
}

/*!
   callback to manage mouse : called when user press or release mouse button
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::mousePressed(GLFWwindow */*window*/, int /*button*/, int action)
{
  if(action == GLFW_PRESS)
  {
      _trackingMode = TM_ROTATE_AROUND;
      _trackball.start();
      _trackball.track(_lastMousePos);
  }
  else if(action == GLFW_RELEASE)
  {
      _trackingMode = TM_NO_TRACK;
  }
}


/*!
   callback to manage mouse : called when user move mouse with button pressed
   You can change in this function the way the user
   interact with the application.
 */
void Viewer::mouseMoved(int x, int y)
{
    if(_trackingMode == TM_ROTATE_AROUND)
    {
        _trackball.track(Vector2i(x,y));
    }

    _lastMousePos = Vector2i(x,y);
}

void Viewer::mouseScroll(double /*x*/, double y)
{
  _cam.zoom(-0.1*y);
}

void Viewer::charPressed(int /*key*/)
{
}
