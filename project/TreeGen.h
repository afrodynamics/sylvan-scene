#pragma once

#include <string>
#include <iostream>
#include <map>
#include <time.h>
#include <vector>
#include <stack>

#include "node.h"
#include "Group.h"
#include "MatrixTransform.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Material.h"
#include "Tree.h"

/* Generates a stochastic L-system tree.
 * 
 * Alphabet:
 *    T - forward (trunk)
 *    t - forward (non-replacing)
 *    F - forward (branches)
 *    L - rotate + Leaf
 *    X - Branching (trunk)
 *    Y - Branching (branches)
 *    A - Apex
 *    r - branch + rotate random angle
 *    s - shrink width by 75%
 *    z - shrink top by 90%
 *    [] - push/pop stack
 *
 * Axiom = "tTX"
 * 
 * Rules:
 *    T -> TT (38%)
 *    T -> TTz (7%)
 *    T -> TXzT (15%)
 *    T -> t (40%)
 *    t -> t (100%)
 *    F -> FF (10%)
 *    F -> FYF (10%)
 *    F -> F (60%)
 *    X -> [srFYL]T[ssrFYYFL]TX (50%)
 *    X -> [ssrFYL][ssrFYL]TXX (30%)
 *    X -> [ssrFYL]T[sssrFYL]TX (20%)
 *    Y -> LL[rLFLLFLY]FLLLFY (40%)
 *    Y -> L[rFLFYFLL][rFLLFLLYF]FY (40%)
 *    Y -> L[srFLFYFLL]F[srFLLFLLYF]FY (20%)
 */


using namespace std;

class TreeGen {
private:
  struct rule {             // Defines a rule
    string rep;           // String to use to replace
    string pred;          // Used for predecessor strings.  Unimplemented
    string succ;          // Used for successor strings.  Unimplemented
    double p;             // Probability of applying rule
    rule * next;          // Next rule for char

    rule(string r, string pr, string suc, double p, rule* n) {
      this->rep = r;
      this->pred = pr;
      this->succ = suc;
      this->p = p;
      this->next = n;
    }
  };
  map<char, rule*> rules;  // Map rules to variables

  void addRule(char, string, string, string, double);
  double rnd();      // Generate random double between 0-1

  static string axiom;
  const static double BRANCH_SCALE;
  const static double TOP_SCALE;
  const static double TOT_DEG;
  static Material leafMat;
  static Material woodMat;
  static GLUquadricObj * q;

public:
  TreeGen();
  ~TreeGen();
  void initialize();    // Initialize the rules

  /* Generate a new tree with branch length len and initial radius rad
   * n - the number of iterations
   */
  Tree * generate(double h, double r, double ang, int n);     
  string genString(int n);      // Generate a string of order n
  void destroyRules();
};