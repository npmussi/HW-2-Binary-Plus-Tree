#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
private:
  int internalSize;
  BTreeNode **children;
  int *keys;
  
  void insertLeft(BTreeNode* returned_node);
  void insertRight(BTreeNode* returned_node);
  InternalNode* insertSplit(BTreeNode* returned_node);
  bool isFull();
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  int getMinimum()const;
  InternalNode* insert(int value); // returns pointer to new InternalNode
    // if it splits else NULL
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);
}; // InternalNode class

#endif
