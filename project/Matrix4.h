#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "Vector3.h"
#include "Vector4.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Matrix4
{
  private:
    /**
     * Original, potentially ambiguous API moved to private, so
     * I don't have to recode these functions entirely. I replaced
     * these with static methods so that all member functions that
     * apply to instances are specifically for changing state. This
     * is to prevent accidentally "clearing" a matrix of stored
     * transformation info.
     */
    void makeRotateX(double); // make rotation matrix about X axis with angle in degrees
    void makeRotateY(double); // make rotation matrix about Y axis with angle in degrees
    void makeRotateZ(double); // make rotation matrix about Z axis with angle in degrees
    void makeRotate(double angle, Vector3& axis);        // make a rotation matrix about an arbitrary axis
    void makeScale(double sx, double sy, double sz);     // make a nonuniform scaling matrix
    void makeTranslate(double tx, double ty, double tz); // make a translation matrix
    void createMatrix(const Vector3& x, const Vector3& y, const Vector3& z, const Vector3& w);

  protected:
    double m[4][4];   // matrix elements; first index is for rows, second for columns (row-major)

  public:
    // Standard Constructor
    Matrix4();   // constructor, initializing all matrix elements to 0 

    // Camera Constructor (last row is the identity 0, 0, 0, 1)
    Matrix4( Vector3& xv, Vector3& yv, Vector3& zv, Vector3& wv );   
    Matrix4& operator=(const Matrix4&);   // overloads assignment operator (from starter code)
    Matrix4 operator*(const Matrix4& m2); // multiply matrix with a matrix, return result
    Vector4 operator*(const Vector4& v);  // multiply matrix with a vector

    /**
     * Static Factory Methods
     */
    static Matrix4 rotX(double); // make rotation matrix about X axis with angle in degrees
    static Matrix4 rotY(double); // make rotation matrix about Y axis with angle in degrees
    static Matrix4 rotZ(double); // make rotation matrix about Z axis with angle in degrees
    static Matrix4 rotate(double angle, Vector3& axis);        // make a rotation matrix about an arbitrary axis
    static Matrix4 scale(double sx, double sy, double sz);     // make a nonuniform scaling matrix
    static Matrix4 translate(double tx, double ty, double tz); // make a translation matrix
    static Matrix4 transpose(const Matrix4&); // returns a copy of the given matrix, transposed
    static Matrix4 create(const Vector3& x, const Vector3& y, const Vector3& z, const Vector3& w); //creates matrix from 4 Vector3s
    static Matrix4 viewport(const double xMin, const double xMax, const double yMin, const double yMax); // Creates a viewport matrix given window width and height in pixels
    static Matrix4 perspective(const double fov, const double aspect, const double znear, const double zfar); // Creates a perspective projection (constructs viewing frustum)
    //static Matrix4 ortho();  // Creates an orthographic projection matrix

    /**
     * State-changing functions (affect the current matrix)
     */
    void transformWorld(Matrix4 & transform); // multiplies transform matrix on left hand side (world coords)
    void transformLocal(Matrix4 & transform); // multiplies transform matrix on right hand side (local coords)
    void transpose();         // transposes the matrix
    void identity();          // make this matrix the identity matrix
    Vector3 getColumn3(int);
    Vector4 getColumn4(int);

    /**
     * Debug printing functions/utilities
     */
    void print(std::string comment); // Prints the matrix (displays all 16 matrix components numerically on the screen as a 4x4 array)
    void printAsMatlab();  // Prints the matrix in one line, in MATLAB format
    double* getPointer();     // useful to passing the matrix to OpenGL

};

#endif
