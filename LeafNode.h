#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
private:
  int *values;
  
  void regularInsert(int value); //inserts when this != full
  void insertLeft(int value); // inserts to left sibling
  void insertRight(int value); //inserts to right sibling
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
  // else NULL
  void print(Queue <BTreeNode*> &queue);
}; //LeafNode class

#endif
