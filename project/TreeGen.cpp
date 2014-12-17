#include "TreeGen.h"
#include "Util.h"

using namespace Util; // Unify all the RNG elements
                      // also, it cross-compiles nicely

string TreeGen::axiom = "IzXXtXT";
const double TreeGen::BRANCH_SCALE = 0.80;
const double TreeGen::MIN_RAD = 0.03;
const double TreeGen::TOP_SCALE = 0.94;
const double TreeGen::TOT_DEG = 360.0;
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
  angleVary = lenVary = 0;
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

// Initialize the rules
void TreeGen::initialize() {
  Util::seed(0); // Seed with the current time

  addRule('T',"tXT","","",0.25);
  addRule('T',"tXXT","","",0.25);
  addRule('T',"tXXXT","","",0.25);
  addRule('T',"tXTXX","","",0.25);
  /*
  addRule('T',"TXXz","","",0.30);
  addRule('T',"tXtXtzXT","","",0.15);
  addRule('T',"tXXtXtXTzX","","",0.15);
  addRule('T',"tXXtXXtTz","","",0.15);
  addRule('T',"tXtXXzT","","",0.15);
  */
  //addRule('T', "t","","",0.10);

  addRule('T',"fYF","","",0.25);
  addRule('T',"fYYF","","",0.25);
  addRule('T',"fYYYF","","",0.25);
  addRule('T',"fYFYY","","",0.25);

  /*
  addRule('F',"FFYYFLLzFY","","",0.30);
  addRule('F',"FFYFLYYFLL","","",0.20);
  addRule('F',"FFYLFYF","","",0.20);
  addRule('F',"FFYLLzF","","",0.50);
  addRule('F',"FFYFzLFY","","",0.30);
  addRule('F',"Lf","","",0.10);
  */

  addRule('I',"iI","","",1);
  addRule('i',"i","","",1);

  addRule('X',"[sssrFFYYF]","","",0.225);
  addRule('X',"[sssrFFYFY]","","",0.225);
  addRule('X',"[ssrFFYF]","","",0.15);
  addRule('X',"[ssrFFYY]","","",0.15);
  addRule('X',"[srFFYY]","","",0.125);
  addRule('X',"[srFFY]","","",0.125);
  
  addRule('Y',"[sssrfLYffYLFY]","","",0.225);
  addRule('Y',"[sssrfLfYfYLLFY]","","",0.225);
  addRule('Y',"[ssrfYfYYfYF]","","",0.15);
  addRule('Y',"[ssrfYfLYfLLF]","","",0.15);
  addRule('Y',"[srfLLYfLfYYfLYY]","","",0.125);
  addRule('Y',"[srfLYLYfLYLF]","","",0.125);

  addRule('t', "t","","",1);
  addRule('f', "f","","",1);
  /* Old T rules
  addRule('T', "TT","","",0.38);
  addRule('T',"TTz","","",0.07);
  addRule('T', "TTXz","","",0.15);
  */
  /* Old F rules
  addRule('F',"YFF","","",0.30);
  addRule('F',"FYF","","",0.50);
  addRule('F',"FF","","",0.20);
  */
  //addRule('F',"F","","",0.20);
  /* Old Branch rules
  addRule('X',"[srFFYL]T[ssrFFYYFL]TX","","",0.50);
  addRule('X',"[ssrFFYL][ssrFFYL]TXX","","",0.30);
  addRule('X',"[ssrFFYL]T[sssrFFYL]TX","","",0.20);
  */
  /* Old Y rules
  addRule('Y',"LL[srLFLLFLY]FLLLFY","","",0.40);
  addRule('Y',"L[srFLFYFLL][srFLLFLLYF]FY","","",0.40);
  addRule('Y',"L[srFLFYFLL]F[srFLLFLLYF]FY","","",0.20);
  */
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
  stack<Group *> nodeState;        // Node stack
  
  // Trunk nodes
  vector<MatrixTransform *> trunkNodes; // = new vector<MatrixTransform*>(); 
  
  // Direct branch nodes
  vector<MatrixTransform *> branchNodes; // = new vector<MatrixTransform*>();
  Group *curr, *root = curr = new Group();
  string treeStr = genString(n);
  int length = treeStr.length();
  double topRad, baseRad = topRad = r;
  int terminate = 0;

  for(int i = 0; i < length; ++i) {
    char c = treeStr.at(i);

    if( terminate ) {
      if( c == '[' ) terminate++;
      if( c == ']' ) terminate--;
      continue;
    }
    else if( baseRad < MIN_RAD ) {
      terminate = 1;
    }

    switch(c) {
      case 'I':
      case 'i':
      case 'T':
      case 't':
      case 'F':
      case 'f':
        {
        // Create tree branch/trunk
        Cylinder * cy = new Cylinder(q, baseRad,topRad,h,5,3);
        cy->setMat(woodMat);
        // Create translation for next branch
        MatrixTransform * trans = new MatrixTransform(Matrix4::translate(0,h,0));
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
        Sphere * leaf = new Sphere(0.5);
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
        MatrixTransform * rotZ = new MatrixTransform(Matrix4::rotZ(a));
        MatrixTransform * rotY = new MatrixTransform(Matrix4::rotY(TOT_DEG * drand()));
        rotY->addChild(rotZ);
        curr->addChild(rotY);
        curr = rotZ;
        branchNodes.push_back(rotZ);
        }
        break;
      case 's':
        baseRad *= BRANCH_SCALE;
        topRad *= BRANCH_SCALE;
        break;
      case 'z':
        topRad *= TOP_SCALE;
        break;
      case '[':
        if( terminate ) terminate++;
        nodeState.push(curr);
        rStack.push(pair<double,double>(baseRad,topRad));
        break;
      case ']':
        if( terminate ) terminate--;
        curr = nodeState.top();
        baseRad = rStack.top().first;
        topRad = rStack.top().second;
        nodeState.pop();
        rStack.pop();
        break;
      default:
        cerr << "ERROR! Letter: " << c << endl;
    }
  }

  // Create the tree object and initialize the trunk/branches
  Tree * tree = new Tree(root);
  tree->setTrunk(trunkNodes)->setBranches(branchNodes);

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
  angleVary = v;
  return this;
}

void TreeGen::destroyRules() {
  // TODO
}
