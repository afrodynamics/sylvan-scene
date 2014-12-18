#pragma once

#include "Group.h"
#include "MatrixTransform.h"
#include "Matrix4.h"
#include <vector>

using namespace std;

class Tree : public Group {
private:
  vector<MatrixTransform *> trunkNodes;
  vector<MatrixTransform *> branchNodes;
  double t;

public:
  Tree(Node *);
  ~Tree();
  Tree* setTrunk(vector<MatrixTransform*>& t); // References are more memory safe
  Tree* setBranches(vector<MatrixTransform*>& t);

  void animate();
  const static double D_T;
};
