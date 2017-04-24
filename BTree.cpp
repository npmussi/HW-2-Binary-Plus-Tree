#include <iostream>
#include "BTree.h"
#include "BTreeNode.h"
#include "LeafNode.h"
#include "InternalNode.h"
using namespace std;

BTree::BTree(int ISize, int LSize):internalSize(ISize), leafSize(LSize)
{
	root = new LeafNode(LSize, NULL, NULL, NULL);
} // BTree::BTree()


void BTree::insert(const int value)
{
  // students must write this
	BTreeNode *b_tree_node_ptr = root->insert(value); 
	if (!(b_tree_node_ptr)){return;} //If we don't have anything returned, don't do anything.
	
	InternalNode *new_internal = new InternalNode(internalSize, leafSize, NULL, NULL, NULL); //Make an internal node; doesn't have a parent since it's the top.
	if (root != b_tree_node_ptr)//Is this not the node from root?
	{
			
	}
	else //This is the leafNode from root requesting a parent.
	{
	    b_tree_node_ptr->setParent(new_internal); //We can't set the parent any other way.
		new_internal->insert(b_tree_node_ptr, b_tree_node_ptr->getRightSibling());
		root = new_internal;
	}
} // BTree::insert()


void BTree::print()
{
  BTreeNode *BTreeNodePtr;
  Queue<BTreeNode*> queue(1000);

  queue.enqueue(root);
  while(!queue.isEmpty())
  {
    BTreeNodePtr = queue.dequeue();
    BTreeNodePtr->print(queue);
  } // while
} // BTree::print()