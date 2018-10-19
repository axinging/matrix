///////////////////////////////////////////////////////////////////////////////
// main.cpp
// ========
// Example of understanding OpenGL transform matrix(GL_MODELVIEW)
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-03-17
// UPDATED: 2017-06-27
///////////////////////////////////////////////////////////////////////////////

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Matrices.h"
#include "Vectors.h"

using std::stringstream;
using std::cout;
using std::endl;
using std::ends;

Matrix4 setFrustum(float l, float r, float b, float t, float n, float f);
Matrix4 setFrustum(float fovY, float aspectRatio, float front, float back);
Matrix4 setOrthoFrustum(float l, float r, float b, float t, float n, float f);

// constants
const float DEG2RAD = 3.141593f / 180;

float GetZn(float n, float f, float Ze) {
  float f_plus_n = f + n;
  float f_sub_n = f - n;
  float f_mul_n = f * n;
  float A = -1 * f_plus_n / f_sub_n;
  float B = -1 * 2 * f_mul_n / f_sub_n;

  float Zn = -1 * (A * Ze + B) / Ze;
  return Zn;
}

void printMatrix4(const Matrix4& mat4) {
  std::cout << " " << mat4[0] << ", " << mat4[1] << ", " << mat4[2] << ", "
            << mat4[3] << std::endl;
  std::cout << " " << mat4[4] << ", " << mat4[5] << ", " << mat4[6] << ", "
            << mat4[7] << std::endl;
  std::cout << " " << mat4[8] << ", " << mat4[9] << ", " << mat4[10] << ", "
            << mat4[11] << std::endl;
  std::cout << " " << mat4[12] << ", " << mat4[13] << ", " << mat4[14] << ", "
            << mat4[15] << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
  {
    Matrix4 matProject = setFrustum(-1, 1, -1, 1, 1, 100);
    Vector4 point(-1, 1, -2, 1);
    Vector4 projectedPoint = matProject * point;
    /*projectedPoint (-1, 1, -4.06061, -2)*/
    std::cout << "projectedPoint: " << projectedPoint << std::endl;
  }

  {
    Matrix4 matProject = setFrustum(-100, 100, -100, 100, 1, 100);
    printMatrix4(matProject);
    Vector4 point(-200, 200, -2, 1);
    Vector4 projectedPoint = matProject * point;
    /*projectedPoint (-2, 2, 0.0202022, 2), not normalized*/
    std::cout << "projectedPoint: " << projectedPoint << std::endl;
    std::cout << "GetZn: " << GetZn(1, 100, -2) << std::endl;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
// set a perspective frustum with 6 params similar to glFrustum()
// (left, right, bottom, top, near, far)
///////////////////////////////////////////////////////////////////////////////
Matrix4 setFrustum(float l, float r, float b, float t, float n, float f) {
  Matrix4 mat;
  mat[0] = 2 * n / (r - l);
  mat[5] = 2 * n / (t - b);
  mat[8] = (r + l) / (r - l);
  mat[9] = (t + b) / (t - b);
  mat[10] = -(f + n) / (f - n);
  mat[11] = -1;
  mat[14] = -(2 * f * n) / (f - n);
  mat[15] = 0;
  return mat;
}

///////////////////////////////////////////////////////////////////////////////
// set a symmetric perspective frustum with 4 params similar to gluPerspective
// (vertical field of view, aspect ratio, near, far)
///////////////////////////////////////////////////////////////////////////////
Matrix4 setFrustum(float fovY, float aspectRatio, float front, float back) {
  float tangent = tanf(fovY / 2 * DEG2RAD);  // tangent of half fovY
  float height = front * tangent;            // half height of near plane
  float width = height * aspectRatio;        // half width of near plane

  // params: left, right, bottom, top, near, far
  return setFrustum(-width, width, -height, height, front, back);
}

///////////////////////////////////////////////////////////////////////////////
// set a orthographic frustum with 6 params similar to glOrtho()
// (left, right, bottom, top, near, far)
///////////////////////////////////////////////////////////////////////////////
Matrix4 setOrthoFrustum(float l, float r, float b, float t, float n, float f) {
  Matrix4 mat;
  mat[0] = 2 / (r - l);
  mat[5] = 2 / (t - b);
  mat[10] = -2 / (f - n);
  mat[12] = -(r + l) / (r - l);
  mat[13] = -(t + b) / (t - b);
  mat[14] = -(f + n) / (f - n);
  return mat;
}
