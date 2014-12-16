#pragma once

#include "Group.h"
#include "MatrixTransform.h"
#include <vector>

using namespace std;

class Tree : public Group {
private:
  vector<MatrixTransform *> * trunkNodes;
  vector<MatrixTransform *> * branchNodes;

public:
  Tree(Node *);
  Tree* setTrunk(vector<MatrixTransform*> * t);
  Tree* setBranches(vector<MatrixTransform*> * t);
  ~Tree();
};