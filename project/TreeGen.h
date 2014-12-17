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
 *    f - forward (non-replacing)
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
 *    T -> TXXz (30%)
 *    T -> XTzXT (15%)
 *    T -> XTTzX (15%)
 *    T -> XTTz (15%)
 *    T -> TXzT (15%)
 *    T -> t (10%)
 *
 *    F -> FYzF (50%)
 *    F -> YFzFY (30%)
 *    F -> YYFzFY (30%)
 *    F -> YFYYF (20%)
 *    F -> YFYF (20%)
 *    F -> f (10%)
 *
 *    t -> t (100%)
 *    f -> f (100%)
 *
 *    X -> [sssrFYF] (22.5%)
 *    X -> [sssrFFY] (22.5%)
 *    X -> [ssrFYF] (15%)
 *    X -> [ssrFFYY] (15%)
 *    X -> [srFYY] (12.5%)
 *    X -> [srFY] (12.5%)
 *
 *
 *    X -> [ssrFYL]T[sssrFYYFL]TX (50%)
 *    X -> [ssrFYL][sssrFYL]TXX (30%)
 *    X -> [ssrFYL]T[sssrFYL]TX (20%)
 *    Y -> LL[srLFLLFLY]FLLLFY (40%)
 *    Y -> L[srFLFYFLL][srFLLFLLYF]FY (40%)
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