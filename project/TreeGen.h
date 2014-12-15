#pragma once

#include <string>
#include <map>
#include "node.h"
#include "Group.h"
#include "MatrixTransform.h"
#include "Tree.h"

class TreeGen {
private:
  struct rule {             // Defines a rule
    std::string rep;
    std::string pred;
    std::string succ;
    double p;
  };
  std::map<char, rule> rules;  // Map rules to variables

  std::string genString(int n);      // Generate a string of order n

public:
  TreeGen();            // No argument constructor
  void initialize();    // Initialize the rules
  Tree generate();      // Generate a new tree
};