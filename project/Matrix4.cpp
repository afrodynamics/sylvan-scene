/***************************************************
 +-------------------------------------------------+
 |                     Matrix4                     |
 +-------------------------------------------------+
 ***************************************************/

#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
#include <exception>
#include "Matrix4.h"

/**
 * Matrix4 standard constructor
 */
Matrix4::Matrix4()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = 0;
    }
  }
} 

/**
 * Constructs a camera matrix, given the vectors x_c, y_c, z_v, and e
 * with the bottom row being the identity. The variable names are xv(ector)
 * yv(ector) zv(ector) and wv(ector) so this can be used for cases that do
 * not include the camera matrix.
 */
Matrix4::Matrix4( Vector3& xv, Vector3& yv, Vector3& zv, Vector3& wv )
{
  
  identity();
  m[0][0] = xv.x; m[0][1] = yv.x; m[0][2] = zv.x; m[0][3] = wv.x;
  m[1][0] = xv.y; m[1][1] = yv.y; m[1][2] = zv.y; m[1][3] = wv.y;
  m[2][0] = xv.z; m[2][1] = yv.z; m[2][2] = zv.z; m[2][3] = wv.z;

} 

/**
 * Matrix-Matrix Multiplication
 */
Matrix4 Matrix4::operator*(const Matrix4& m2) {

  Matrix4 temp;
  double sum; 

  // Using mathematical formula from: 
  // http://en.wikipedia.org/wiki/Matrix_multiplication
  
  // Iterate over all the entries in temp
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {

      // Reset the sum
      sum = 0;

      // Sigma from k=1 to m of A_ik * B_kj
      // (Let underscores denote subscripts)
      for (int k = 0; k < 4; ++k) {
        sum += m[i][k] * 
               m2.m[k][j];
      }

      // Set the entry
      temp.m[i][j] = sum;
    }
  }

  return temp;
};

/**
 * Matrix-Vector Multiplication
 *   Note that you may need to call dehomogenizeon the resulting Vector4
 *   if the parameter v is a point.
 */
Vector4 Matrix4::operator*(const Vector4& v) {
  return Vector4(
    m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
    m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
    m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
    m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
  ); 
  // Accessing the fields in the Vector4 is why we declared 
  // Matrix4 as a friend class of Vector4
};

/**
 * Copy Assignment Operator
 */
Matrix4& Matrix4::operator=(const Matrix4& m2)
{
  if (this != &m2)
  {
    for (int i=0; i<4; ++i)
    {
      for (int j=0; j<4; ++j)
      {
        m[i][j] = m2.m[i][j];
      }
    }
  }
  return *this;
}

// return pointer to matrix elements
double* Matrix4::getPointer()
{
  return &m[0][0];
}

// get column as vec4
Vector4 Matrix4::getColumn4(int col) {
	if (col < 0 | col > 4) {
		throw new std::runtime_error("Matrix4: out of bounds call to get column");
	}
	return Vector4( m[0][col], m[1][col], m[2][col], m[3][col] );
}
// get column as vec3
Vector3 Matrix4::getColumn3(int col) {
	if (col < 0 | col > 4) {
		throw new std::runtime_error("Matrix4: out of bounds call to get column");
	}
	return Vector3(m[0][col], m[1][col], m[2][col]);
}

// set matrix to identity matrix
void Matrix4::identity()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      if (i==j) 
        m[i][j] = 1.0; 
      else 
        m[i][j] = 0.0;
    }
  }
}

// transpose the matrix (mirror at diagonal)
void Matrix4::transpose()
{
  Matrix4 temp;
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      temp.m[j][i] = m[i][j];
    }
  }
  *this = temp;  // copy temporary values to this matrix
}

/**
 * makeRotateX
 *   Creates a rotation matrix around the x axis with specified angle
 */
void Matrix4::makeRotateX(double angle) {
  angle = angle / 180.0 * M_PI;  // convert from degrees to radians
  identity();
  m[1][1] = cos(angle);
  m[1][2] = -sin(angle);
  m[2][1] = sin(angle);
  m[2][2] = cos(angle);
};

// Creates a rotation matrix which rotates about the y axis.
// angle is expected in degrees
void Matrix4::makeRotateY(double angle)
{
  angle = angle / 180.0 * M_PI;  // convert from degrees to radians
  identity();
	m[0][0] = cos(angle);
	m[0][2] = sin(angle);
	m[2][0] = -sin(angle);
	m[2][2] = cos(angle);
}

// Creates a rotation matrix which rotates about the z axis.
// angle is expected in degrees
void Matrix4::makeRotateZ(double angle) {
  angle = angle / 180.0 * M_PI;  // convert from degrees to radians
  identity();
  m[0][0] = cos(angle);
  m[0][1] = -sin(angle);
  m[1][0] = sin(angle);
  m[1][1] = cos(angle);
};

// makes a scale matrix 
void Matrix4::makeScale(double sx, double sy, double sz) {
  identity();
  m[0][0] = sx;
  m[1][1] = sy;
  m[2][2] = sz;
};

// makes a rotation matrix that rotates about an arbitrary axis
// that is passed in
// angle is expected in degrees
void Matrix4::makeRotate(double angle, Vector3& axis) {
	
	angle = angle / 180.0 * M_PI; // convert from degrees to radians
	double ax = axis.x;
	double ay = axis.y;
	double az = axis.z;

	// http://ivl.calit2.net/wiki/images/3/3a/02_TransformationsF13.pdf

    identity();
	
	// Row 1 (updated)

	m[0][0] = 1.0 + (1.0 - cos(angle)) * (ax * ax - 1);
	m[0][1] = -az * sin(angle) + (1 - cos(angle)) * ax * ay;
	m[0][2] = ay * sin(angle) + (1 - cos(angle)) * ax * az;

	// Row 2 (updated)

	m[1][0] = az * sin(angle) + (1 - cos(angle)) * ay * ax;
	m[1][1] = 1 + (1 - cos(angle)) * (ay*ay - 1);
	m[1][2] = -ax * sin(angle) + (1 - cos(angle)) * ay * az;

	// Row 3

	m[2][0] = -ay * sin(angle) + (1 - cos(angle)) * az * ax;
	m[2][1] = ax * sin(angle) + (1 - cos(angle)) * az * ay;
	m[2][2] = 1 + (1 - cos(angle)) * (az * az - 1);

	// Bottom Row (4)

}

// translates the cube's current position by the distance 
// of the parameters passed in
void Matrix4::makeTranslate(double tx, double ty, double tz) {
  // make identity
  identity(); 
  m[0][3] = tx; // Translation entries go in the fourth column
  m[1][3] = ty;
  m[2][3] = tz;
};

void Matrix4::createMatrix(const Vector3& x, const Vector3& y, const Vector3& z, const Vector3& w) {
    identity();
    m[0][0] = x.x;
    m[0][1] = y.x;
    m[0][2] = z.x;
    m[0][3] = w.x;
    
    m[1][0] = x.y;
    m[1][1] = y.y;
    m[1][2] = z.y;
    m[1][3] = w.y;
    
    m[2][0] = x.z;
    m[2][1] = y.z;
    m[2][2] = z.z;
    m[2][3] = w.z;
}

/**
 * Multiplies the supplied transformation matrix on the
 * left hand side, thereby making the transformation apply
 * in world coordinates. This function changes the state of
 * the current matrix.
 * @param transform transformation matrix
 */
void Matrix4::transformWorld(Matrix4& transform) {
  (*this) = transform * (*this);
};

/**
 * Multiplies the supplied transformation matrix on the
 * right hand side, thereby making the transformation apply
 * in local (object/camera etc.) coordinates. This function
 * changes the state of the current matrix.
 * @param transform transformation matrix
 */
void Matrix4::transformLocal(Matrix4& transform) {
  (*this) = (*this) * transform;
};

/**
 * print:
 *   Prints the matrix to stderr, prefixed with some comment string.
 */
void Matrix4::print(std::string comment) {
  std::ostringstream matrixString;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      matrixString << m[i][j];
      if ( j < 3 )
        matrixString << " ";
    }
    if ( i < 3 )
      matrixString << "\n";
  }
  std::cerr << comment << "\n[ " << matrixString.str() << " ]\n";
};

/**
 * printAsMatlab:
 *   Prints the matrix to stderr in one line in MATLAB format
 */
void Matrix4::printAsMatlab() {
  std::ostringstream matrixString;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      matrixString << m[i][j];
      if ( j < 3 )
        matrixString << " ";
      else
        matrixString << "; ";
    }
  }
  std::cerr << "[ " << matrixString.str() << " ]\n";
};

/**
 * Static Method Refactoring
 */
Matrix4 Matrix4::rotX(double angle) {
  Matrix4 transform;
  transform.identity();
  transform.makeRotateX(angle);
  return transform;
};
Matrix4 Matrix4::rotY(double angle) {
  Matrix4 transform;
  transform.identity();
  transform.makeRotateY(angle);
  return transform;
};
Matrix4 Matrix4::rotZ(double angle) {
  Matrix4 transform;
  transform.identity();
  transform.makeRotateZ(angle);
  return transform;
};
Matrix4 Matrix4::rotate(double angle, Vector3& axis) {
  Matrix4 transform;
  transform.identity();
  transform.makeRotate(angle, axis);
  return transform;
};
Matrix4 Matrix4::scale(double sx, double sy, double sz) {
  Matrix4 transform;
  transform.identity();
  transform.makeScale(sx,sy,sz);
  return transform;
};
Matrix4 Matrix4::translate(double tx, double ty, double tz) {
  Matrix4 transform;
  transform.identity();
  transform.makeTranslate(tx,ty,tz);
  return transform;
};
Matrix4 Matrix4::translate(Vector3 pos) {
  return translate(pos.getX(),pos.getY(),pos.getZ());
}
Matrix4 Matrix4::transpose( const Matrix4& other ) {
  Matrix4 transpose = other;
  transpose.transpose();
  return transpose;
}
Matrix4 Matrix4::create(const Vector3& x, const Vector3& y, const Vector3& z, const Vector3& w) {
  Matrix4 creation;
  creation.createMatrix(x, y, z, w);
  return creation;
}

