#include "Tree.h"

const double Tree::D_T = 0.2;

Tree::Tree(Node *n) {
  if (n) {
    addChild(n);
  }
  t = 0;
}

Tree::~Tree() {
  // All instance variables are created dynamically
}

Tree* Tree::setTrunk(vector<MatrixTransform*>& t) {
  trunkNodes = vector<MatrixTransform*>(t); // Make a deep copy
  return this;
}

Tree* Tree::setBranches(vector<MatrixTransform*>& b) {
  branchNodes = vector<MatrixTransform*>(b); // Make a deep copy
  return this;
}

double calcTrunkAngle(double t) {
  if( t < 0 ) return 0;
  if( t < 5 ) return Tree::D_T / 2.0;
  if( t < (5+2*M_PI*4) ) 2*sin(t-5);
  else return -0.1;
}

double calcBranchAngle(double t) {
  if( t < 0 ) return 0;
  if( t < 5 ) return Tree::D_T / 2.0;
  if( t < (5+2*M_PI*4) ) return 3*sin(t-5);
  else return -0.1;
}

void Tree::animate() {
  t += D_T;

  for( MatrixTransform * mt : trunkNodes ) {
    mt->setMatrix(mt->getMatrix() * Matrix4::rotZ(calcTrunkAngle(t)));
  }

  for( MatrixTransform * mt : branchNodes ) {
    mt->setMatrix(mt->getMatrix() * Matrix4::rotZ(calcBranchAngle(t)));
  }

  if( t > (10+2*M_PI*4) ) t=0;
}
