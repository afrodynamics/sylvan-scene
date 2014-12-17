#include "TreeGen.h"

string TreeGen::axiom = "tTX";
const double TreeGen::BRANCH_SCALE = 0.75;
const double TreeGen::TOP_SCALE = 0.9;
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

TreeGen::TreeGen() {
  // TODO
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
  addRule('T',"TXXz","","",0.30);
  addRule('T',"XTzXT","","",0.15);
  addRule('T',"XTTzX","","",15);
  addRule('T',"XTTz","","",15);
  addRule('T',"TXzT","","",15);
  addRule('T', "t","","",0.10);

  addRule('F',"YYFzFY","","",0.30);
  addRule('F',"YFYYF","","",0.20);
  addRule('F',"YFYF","","",0.20);
  addRule('F',"FYzF","","",0.50);
  addRule('F',"YFzFY","","",0.30);
  addRule('F',"f","","",0.10);

  addRule('X',"[sssrFYF]","","",0.225);
  addRule('X',"[sssrFFY]","","",0.225);
  addRule('X',"[ssrFYF]","","",0.15);
  addRule('X',"[ssrFFYY]","","",0.15);
  addRule('X',"[srFYY]","","",0.125);
  addRule('X',"[srFY]","","",0.125);

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
  addRule('Y',"LL[srLFLLFLY]FLLLFY","","",0.40);
  addRule('Y',"L[srFLFYFLL][srFLLFLLYF]FY","","",0.40);
  addRule('Y',"L[srFLFYFLL]F[srFLLFLLYF]FY","","",0.20);
}

// Generate random double between 0-1
double TreeGen::rnd() {
  return ((double)rand()) / RAND_MAX;
}

// Generate a string of order n
string TreeGen::genString(int n) {
  string str = axiom;
  srand(time(NULL));

  // n interations
  for(int i = 0; i < n; ++i) {
    // iterate over the string
    for(int j = 0; j < str.length(); ++j) {
      char c = str.at(j);
      double r = rnd();
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

/* Generate a new tree
 * h - the step of a forward movement
 * r - original radius of tree
 * a - angle of branches
 */
Tree * TreeGen::generate(double h, double r, double a, int n) {
  // TODO
  stack<pair<double,double>> rStack;    // Stack of radii, pair of base, top 
  stack<Group *> nodeState;        // Node stack
  vector<MatrixTransform *> * trunkNodes = new vector<MatrixTransform*>();     // Trunk nodes
  vector<MatrixTransform *> * branchNodes = new vector<MatrixTransform*>();     // Direct branch nodes
  Group * curr, *root = curr = new Group();
  string treeStr = genString(n);
  int length = treeStr.length();
  double topRad, baseRad = topRad = r;

  for(int i = 0; i < length; ++i) {
    char c = treeStr.at(i);
    switch(c) {
      case 'T':
      case 't':
      case 'F':
        {
        // Create tree branch/trunk
        Cylinder * cy = new Cylinder(q, baseRad,topRad,h);
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
        trunkNodes->push_back(trunkTrans);   // Add trunk node to vector
        }
        break;
      case 'X':   // Do nothing
        break;
      case 'Y':   // Use leaf where a branch would have been
      case 'L':
        {
        MatrixTransform * trans = new MatrixTransform(Matrix4::translate(0,0,baseRad+0.5));
        MatrixTransform * rotY = new MatrixTransform(Matrix4::rotY(TOT_DEG*rnd()));
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
        MatrixTransform * rotY = new MatrixTransform(Matrix4::rotY(TOT_DEG*rnd()));
        rotY->addChild(rotZ);
        curr->addChild(rotY);
        curr = rotZ;
        branchNodes->push_back(rotZ);
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
        nodeState.push(curr);
        rStack.push(pair<double,double>(baseRad,topRad));
        break;
      case ']':
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

void TreeGen::destroyRules() {
  // TODO
}
