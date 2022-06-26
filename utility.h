#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <cmath>
#include <m3d.h>

/*!
 *  THE SP..
 */
void printGLErrors(const char* const fn, const char* const file, const int line)
{
    bool success = true;
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        const char* errorString = reinterpret_cast<const char*>(gluErrorString(error));
        if(errorString)
            std::cerr <<  ("OpenGL error in %s at line %d calling function %s: '%s'..", file, line, fn, errorString) << std::endl;
        else
            std::cerr << ("OpenGL error in %s at line %d calling function %s: '%d 0x%X'", file, line, fn, error, error) << std::endl;
    }
}

#ifdef _DEBUG
    #define checkGLCall(x) do {printGLErrors(">>BEFORE<< " #x, __FILE__, __LINE__); (x); printGLErrors(#x, __FILE__, __LINE__);}
    #define checkGLResult(x) (x); printGLErrors(#x, __FILE__, __LINE__);
    #define checkExistingError(x) printGLErrors(">>BEFORE<< " #x, __FILE__, __LINE__);
#else
    #define checkGLCall(x)(x)
    #define checkExistingErrors(x) (x)
    #define checkGLResult(x) (x)
#endif // _DEBUG

void PrintShaderInfoLog(const GLint shader)
{
    int infoLength = 0;
    int charsWritten = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
    if(infoLength > 0)
    {
        GLchar* infoLog = new GLchar[infoLength];
        glGetShaderInfoLog(shader, infoLength, &charsWritten, infoLog);
        std::cout << "Shader Info Log: " << std::endl << infoLog << std::endl;
        delete [] infoLog;
    }
}
/*!
    1.f, .0f,  .0f,  .0f,
    0.f, 1.f,  .0f,  .0f,
    0.f, .0f, 1.0f,  .0f,
    0.f, .0f,  .0f, 1.0f
*/
template<typename T>
float* buildTranslateMat4(T x, T y, T z)
{
    float mat[16] = {
        1.f, .0f, .0f, .0f,
        .0f, 1.f, .0f, .0f,
        .0f, .0f, 1.f, .0f,
        x, y, z, 1.f
    };

    return mat;
}

template<typename T>
float* buildRotateXMat4(T r)
{
    float mat[16] =
    {
        1.f, .0f,  .0f,  .0f,
        0.f, cos(r),  -sin(r),  .0f,
        0.f, sin(r),   cos(r),  .0f,
        0.f, .0f,  .0f, 1.0f
    };

    return mat;
}

template<typename T>
float* buildRotateYMat4(T r)
{
    float mat[16] =
    {
        cos(r), .0f,  sin(r),  .0f,
        0.f, 1.f,  .0f,  .0f,
        -sin(r), .0f, cos(r),  .0f,
        0.f, .0f,  .0f, 1.0f
    };
    return mat;
}

template<typename T>
float* buildRotateZMat4(T r)
{
    float mat[16] =
    {
        cos(r), -sin(r),  .0f,  .0f,
        sin(r),  cos(r),  .0f,  .0f,
        0.f, .0f, 1.0f,  .0f,
        0.f, .0f,  .0f, 1.0f
    };
    return mat;
}

void createLookAt(M3DMatrix44f& target, M3DVector3f eye, M3DVector3f center, M3DVector3f up)
{
    M3DVector3f zaxis;
    M3DVector3f xaxis;
    M3DVector3f yaxis;

    M3DVector3f tmpVecX;
    M3DVector3f tmpVecY;
    M3DVector3f tmpVecZ;

    m3dSubtractVectors3(tmpVecZ, eye, center);
    m3dNormalizeVector(tmpVecZ);
    m3dCopyVector3(zaxis, tmpVecZ);

    m3dCrossProduct(tmpVecX, up, zaxis);
    m3dNormalizeVector(tmpVecX);
    m3dCopyVector3(xaxis, tmpVecX);

    m3dCrossProduct(tmpVecY, zaxis, xaxis);

    float dotX = (float) m3dDotProduct(xaxis, eye);
    float dotY = (float) m3dDotProduct(yaxis, eye);
    float dotZ = (float) m3dDotProduct(zaxis, eye);

    float mat[16] = {
        xaxis[0], yaxis[0], zaxis[0], .0f,
        xaxis[1], yaxis[1], zaxis[1], .0f,
        xaxis[2], yaxis[2], zaxis[2], .0f,
        dotX,     dotY,     dotZ,     1.f
    };

    m3dCopyMatrix44(target, mat);
}

void createPerspectiveMat(M3DMatrix44f& target, float fov, float aspect, float near, float far)
{
    float mat[16] =
    {
        fov/aspect, .0f, .0f, .0f,
        .0f, fov, .0f, .0f,
        .0f, .0f, (far + near)/(near - far), (2 * far * near) / (near - far),
        .0f, .0f, -1.f, .0f
    };
    m3dCopyMatrix44(target, mat);
}


#endif // UTILITY_H_INCLUDED
