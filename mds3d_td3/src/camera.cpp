#include <iostream>
#include "camera.h"

using namespace Eigen;

Camera::Camera()
{
  mViewMatrix.setIdentity();
  setPerspective(M_PI/2,0.1,10000);
  setViewport(0,0);
}

void Camera::lookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up)
{
  mTarget = target;

  // TODO

  Vector3f ZC = (position - target).normalized();
  Vector3f XC = (up.cross(ZC)).normalized();
  Vector3f YC = ZC.cross(XC);

  Matrix3f R;
  R << XC,YC,ZC;


  mViewMatrix.topLeftCorner(3,3) = R.transpose();
  mViewMatrix.topRightCorner(3,1) = -R.transpose() * position;

}

void Camera::setPerspective(float fovY, float near, float far)
{
  m_fovY = fovY;
  m_near = near;
  m_far = far;
}

void Camera::setViewport(int width, int height)
{
  mVpWidth = width;
  mVpHeight = height;
}

void Camera::zoom(float x)
{
  Vector3f t = Affine3f(mViewMatrix) * mTarget;
  mViewMatrix = Affine3f(Translation3f(Vector3f(0,0,x*t.norm())).inverse()) * mViewMatrix;
}

void Camera::rotateAroundTarget(float angle, Vector3f axis)
{
  Vector3f w = axis; // rotation axis
  Vector3f c = mTarget; // center of rotation
  Affine3f A = Translation3f(c) * AngleAxisf(angle, w) * Translation3f(-c);
  mViewMatrix *= A.matrix();

}

Camera::~Camera()
{
}

const Matrix4f& Camera::viewMatrix() const
{
  return mViewMatrix;
}

Matrix4f Camera::projectionMatrix() const
{
  float aspect = float(mVpWidth)/float(mVpHeight);
  float theta = m_fovY*0.5;
  float range = m_far - m_near;
  float invtan = 1./std::tan(theta);

  Matrix4f projMat;
  projMat.setZero();
  projMat(0,0) = invtan / aspect;
  projMat(1,1) = invtan;
  projMat(2,2) = -(m_near + m_far) / range;
  projMat(2,3) = -2 * m_near * m_far / range;
  projMat(3,2) = -1;

  return projMat;
}
