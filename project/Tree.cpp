#include "Tree.h"

Tree::Tree(Node *n) {
  if (n) {
    addChild(n);
  }
}

Tree* Tree::setTrunk(vector<MatrixTransform*>& t) {
  trunkNodes = vector<MatrixTransform*>(t); // Make a deep copy
  return this;
}

Tree* Tree::setBranches(vector<MatrixTransform*>& b) {
  branchNodes = vector<MatrixTransform*>(b); // Make a deep copy
  return this;
}

Tree::~Tree() {
  // if( trunkNodes) delete trunkNodes;
}