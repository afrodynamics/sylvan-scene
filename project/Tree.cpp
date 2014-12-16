#include "Tree.h"

Tree::Tree(Node * n) {
  if(n) {
    addChild(n);
  }
}