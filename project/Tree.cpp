#include "Tree.h"

Tree::Tree(Node * n) {
  if(n) {
    addChild(n);
  }
  trunkNodes = NULL;
}

Tree* Tree::setTrunk(vector<MatrixTransform*> * t) {
  trunkNodes = t;
  return this;
}

Tree* Tree::setBranches(vector<MatrixTransform*> * b) {
  branchNodes = b;
  return this;
}

Tree::~Tree() {
  if(trunkNodes) delete trunkNodes;
  if(branchNodes) delete branchNodes;
}