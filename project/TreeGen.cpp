#include "TreeGen.h"

string TreeGen::axiom = "TX";

TreeGen::TreeGen() {
  // TODO
}

TreeGen::~TreeGen() {
  // TODO
}

// Initialize the rules
void TreeGen::initialize() {
  addRule('T', "TT","","",0.38);
  addRule('T',"TTz","","",0.07);
  addRule('T', "TTXz","","",0.15);
  addRule('T', "T","","",0.40);
  addRule('F',"FF","","",0.30);
  addRule('F',"FYF","","",0.10);
  addRule('F',"F","","",0.60);
  addRule('X',"[srFYL]T[ssrFYYFL]TX","","",0.50);
  addRule('X',"[ssrFYL][ssrFYL]TXX","","",0.30);
  addRule('X',"[ssrFYL]T[sssrFYL]TX","","",0.20);
  addRule('Y',"LL[rLFLLFLY]FLLLFY","","",0.40);
  addRule('Y',"L[rFLFYFLL][rFLLFLLYF]FY","","",0.40);
  addRule('Y',"L[srFLFYFLL]F[srFLLFLLYF]FY","","",0.20);
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
    while( rlist-> next ) {
      totP += rlist->p;
      rlist = rlist->next;
    }
    rlist->next = r;
    r->p += totP;
  }
}

// Generate random double between 0-1
double TreeGen::prob() {
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
      double r = prob();
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

// Generate a new tree
Tree TreeGen::generate(double len, double rad, double ang, int n) {
  // TODO
  return Tree(NULL);
} 

void TreeGen::destroyRules() {
  // TODO
}