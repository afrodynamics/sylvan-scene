#include "TreeGen.h"
#include "Util.h"

using namespace Util; // Unify all the RNG elements
                      // also, it cross-compiles nicely

string TreeGen::axiom = "IZXXtXT";
const int TreeGen::MAX_SLICES = 10;
const int TreeGen::MIN_SLICES = 5;
const double TreeGen::LEN_ALTER = -0.15;
const double TreeGen::MIN_RAD = 0.2;
const double TreeGen::BRANCH_SCALE = 0.80;
const double TreeGen::TOP_SCALE = 0.9;
const double TreeGen::TOP_T_SCALE = 0.95;
const double TreeGen::TOT_DEG = 360.0;
const double TreeGen::G_ANG = 137.508;

Material TreeGen::leafMat = Material().setColor(0.1,1,0.1,1)
                                     .setShine(5)
                                     .setAmbient(0.02352941176,0.450980392,0,1)
                                     .setDiffuse(0.8,0.8,0.8,1)
                                     .setSpecular(0.3,0.3,0.3,1);
Material TreeGen::woodMat = Material().setColor(0.89509803921,0.45098039215,1/3.0,1)
                                     .setShine(0)
                                     .setAmbient(0.2,0.2,0.2,1)
                                     .setDiffuse(1,1,1,1)
                                     .setSpecular({},0);
GLUquadricObj * TreeGen::q = gluNewQuadric();

TreeGen::TreeGen() :
  TreeGen(3,3,35,5)
{
}

TreeGen::TreeGen(double l, double r, double a, int n) {
  length = l;
  radius = r;
  angle = a;
  depth = n;
  aVary = lVary = 0;
}

TreeGen::~TreeGen() {
  // TODO
}




// Add a rule to the dictionary/linked list data structure
void TreeGen::addRule(char k,
                      string rep,
                      string pred,
                      string s,
                      double p) {
  map<char,rule *>::iterator it = rules.find(k);
  
  rule * r = new rule(rep,pred,s,p,NULL);

  if( it == rules.end() ) {
    // Insert new rule
    rules[k] = r;
  }
  else {
    // Add rule to linked list
    double totP = 0;      // Accumulate the probability so it adds up to 1
    rule * rlist = it->second;
    while( rlist->next ) {
      totP += rlist->p;
      rlist = rlist->next;
    }
    rlist->next = r;
    r->p += totP;
  }
}

// Calculate the number fo slices for a cylinder as a fn of radius
int TreeGen::calcSlices(double initR, double r) {
  double s = (r-MIN_RAD)/(initR-MIN_RAD);

  return (int)(s*((double)MAX_SLICES-MIN_SLICES)+MIN_SLICES);
}

// Initialize the rules
void TreeGen::initialize() {
  Util::seed(0); // Seed with the current time
  
  // Grow the trunk base
  addRule('I',"iI","","",1);
  addRule('i',"i","","",1);

  addRule('T',"ttXZT","","",0.25);
  addRule('T',"tttXXZT","","",0.30);
  addRule('T',"ttXtXZT","","",0.30);
  addRule('T',"ttXZTXX","","",0.15);
  /*
  addRule('T',"TXXz","","",0.30);
  addRule('T',"tXtXtzXT","","",0.15);
  addRule('T',"tXXtXtXTzX","","",0.15);
  addRule('T',"tXXtXXtTz","","",0.15);
  addRule('T',"tXtXXzT","","",0.15);
  */
  //addRule('T', "t","","",0.10);

  addRule('F',"fYF","","",0.25);
  addRule('F',"fYYzF","","",0.25);
  addRule('F',"fYYYzF","","",0.25);
  addRule('F',"fYFYY","","",0.25);

  /*
  addRule('F',"FFYYFLLzFY","","",0.30);
  addRule('F',"FFYFLYYFLL","","",0.20);
  addRule('F',"FFYLFYF","","",0.20);
  addRule('F',"FFYLLzF","","",0.50);
  addRule('F',"FFYFzLFY","","",0.30);
  addRule('F',"Lf","","",0.10);
  */
  /*
  addRule('X',"[sssrffYYF]","","",0.225);
  addRule('X',"[sssrffYFY]","","",0.225);
  addRule('X',"[ssrffYF]","","",0.15);
  addRule('X',"[ssrfffYY]","","",0.15);
  addRule('X',"[srfffffYY]","","",0.125);
  addRule('X',"[srffffY]","","",0.125);
  */
  
  addRule('X',"[ssrffYF]","","",0.20);
  addRule('X',"[ssrffFYY]","","",0.20);
  addRule('X',"[ssrfffYfYF]","","",0.20);
  addRule('X',"[srffffYYF]","","",0.133);
  addRule('X',"[srfffFYY]","","",0.133);
  addRule('X',"[srfffYF]","","",0.134);
  
  /*
  addRule('Y',"[sssrffLYffYLFY]","","",0.225);
  addRule('Y',"[sssrffLfYfYLLFY]","","",0.225);
  addRule('Y',"[ssrffYfYYfYF]","","",0.15);
  addRule('Y',"[ssrffYfLYfLLF]","","",0.15);
  addRule('Y',"[srffLLYfLfYYfLYY]","","",0.125);
  addRule('Y',"[srffLYLYfLYLF]","","",0.125);
  */
  addRule('Y',"[ssssrLLfLfLYfLLfLY]","","",0.25);
  addRule('Y',"[ssssrfLfLYfYfYfLLL]","","",0.25);
  addRule('Y',"[ssrffLLfYfYF]","","",0.15);
  addRule('Y',"[ssrffLfYfLLF]","","",0.15);
  addRule('Y',"[srffffYfF]","","",0.125);
  addRule('Y',"[srfffFYY]","","",0.125);

  addRule('t', "t","","",1);
  addRule('f', "Lf","","",0.35);
  addRule('f', "fL","","",0.35);
  addRule('f', "LfL","","",0.35);
  addRule('f', "f","","",0.3);

}

// Generate a string of order n
string TreeGen::genString(int n) {
  string str = axiom;

  // n interations
  for(int i = 0; i < n; ++i) {
    // iterate over the string
    for(int j = 0; j < str.length(); ++j) {
      char c = str.at(j);
      double r = drand();
      rule * charRule = rules[c];

      // Loop through rules
      while( charRule ) {
        if( r <= charRule->p ) {
          str.replace(j,1,charRule->rep);

          // Increase the index by length of replacement
          j += charRule->rep.length() - 1;
          break;
        }

        charRule = charRule->next;
      }
    }
  }

  return str;
}

/* Default generations
 */
Tree * TreeGen::generate() {
  return generate(length,radius,angle,depth);
}

Tree * TreeGen::generate(int n) {
  return generate(length,radius,angle,n);
}

/* Generate a new tree
 * h - the step of a forward movement
 * r - original radius of tree
 * a - angle of branches
 */
Tree * TreeGen::generate(double h, double r, double a, int n) {
  stack<pair<double,double>> rStack;    // Stack of radii, pair of base, top 
  stack<double> yaStack;    // Stack of y_angles.  Top is the one that's used
  stack<Group *> nodeState;        // Node stack
  
  // Trunk nodes
  vector<MatrixTransform *> trunkNodes; // = new vector<MatrixTransform*>(); 
  
  // Direct branch nodes
  vector<MatrixTransform *> branchNodes; // = new vector<MatrixTransform*>();
  Group *curr, *root = curr = new Group();
  string treeStr = genString(n);
  int length = treeStr.length();
  double topRad, baseRad = topRad = r;
  yaStack.push(drand()*TOT_DEG);        // Set the first y-angle
  int terminate = 0;                    // Terminate flag
  string stack = "";
  unsigned char stacks = 0;

  for(int i = 0; i < length; ++i) {
    char c = treeStr.at(i);

    /*
    if( terminate ) {
      if( c == '[' ) terminate++;
      if( c == ']' ) terminate--;
      cerr << "Terminate branch" << endl;
      continue;
    }
    else if( baseRad < MIN_RAD ) {
      terminate = 1;
    }
    */

    switch(c) {
      case 'I':
      case 'i':
      case 'T':
      case 't':
      case 'F':
      case 'f':
        {
        // Create tree branch/trunk
        double vary = lVary * drand() - lVary/2.0;
        Cylinder * cy = new Cylinder(q, baseRad,topRad,h+vary,calcSlices(r,baseRad),3);
        cy->setMat(woodMat);
        // Create translation for next branch
        MatrixTransform * trans = new MatrixTransform(Matrix4::translate(0,h+LEN_ALTER,0));
        // Create trunk transformation for possible wind
        MatrixTransform * trunkTrans = new MatrixTransform();

        // Add to current state
        curr->addChild(cy);
        trans->addChild(trunkTrans);
        curr->addChild(trans);
        curr = trunkTrans;      // Change current state
        baseRad = topRad;
        trunkNodes.push_back(trunkTrans);   // Add trunk node to vector
        }
        break;
      case 'X':   // Do nothing
        break;
      case 'Y':   // Use leaf where a branch would have been
      case 'L':
        {
        MatrixTransform * trans = new MatrixTransform(Matrix4::translate(0,0,baseRad+0.5));
        MatrixTransform * rotY = new MatrixTransform(Matrix4::rotY(TOT_DEG*Util::drand()));
        Sphere * leaf = new Sphere();
        leaf->setMat(leafMat);

        // Add leaf to curr position
        rotY->addChild(trans);
        trans->addChild(leaf);
        curr->addChild(rotY);
        // Do not update curr
        }
        break;
      case 'r':
        {
        double vary = aVary * drand() + aVary/2.0;
        double y_ang = yaStack.top();
        MatrixTransform * rotZ = new MatrixTransform(Matrix4::rotZ(a+vary));
        MatrixTransform * rotY = new MatrixTransform(Matrix4::rotY(y_ang));
        rotY->addChild(rotZ);
        curr->addChild(rotY);
        curr = rotZ;
        branchNodes.push_back(rotZ);
        y_ang += G_ANG;
        if(y_ang >= TOT_DEG) y_ang -= TOT_DEG;
        yaStack.pop();
        yaStack.push(y_ang);
        // Push an angle that goes away from the originating branch
        yaStack.push(3/4.0*TOT_DEG * drand() - 5/8.0*TOT_DEG);
        }
        break;
      case 's':
        baseRad *= BRANCH_SCALE;
        topRad *= BRANCH_SCALE;
        break;
      case 'Z':
        topRad *= TOP_T_SCALE;
        break;
      case 'z':
        topRad *= TOP_SCALE;
        break;
      case '[':
        {
        if( terminate ) terminate++;
        nodeState.push(curr);
        rStack.push(pair<double,double>(baseRad,topRad));
        // Push an angle that goes away from the originating branch
        //yaStack.push(3/4.0*TOT_DEG * drand() - 5/8.0*TOT_DEG);
        }
        break;
      case ']':
        if( terminate ) terminate--;
        curr = nodeState.top();
        baseRad = rStack.top().first;
        topRad = rStack.top().second;
        nodeState.pop();      // Pop off the stack
        rStack.pop();         // Pop off the stack
        yaStack.pop();        // Pop off the stack.  Note, no allocation necessary
        break;
      default:
        cerr << "ERROR! Letter: " << c << endl;
    }
  }

  // Create the tree object and initialize the trunk/branches
  Tree * tree = new Tree(root);
  tree->setTrunk(trunkNodes)->setBranches(branchNodes);
  cerr << endl;
  return tree;
}

/* Setter methods
 */
TreeGen* TreeGen::setLength(double l) {
  length = l;
  return this;
}
TreeGen* TreeGen::setRadius(double r) {
  radius = r;
  return this;
}

TreeGen* TreeGen::setAngle(double a) {
  angle = a;
  return this;
}
TreeGen* TreeGen::setDepth(int n) {
  depth = n;
  return this;
}
TreeGen* TreeGen::setAVary(double v) {
  aVary = v;
  return this;
}
TreeGen* TreeGen::setLVary(double v) {
  lVary = v;
  return this;
}

void TreeGen::destroyRules() {
  // TODO
}
