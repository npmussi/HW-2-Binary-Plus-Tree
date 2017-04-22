#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value) //Well, at least we're guaranteed to try to insert stuff...
{
	if (count != leafSize)
	{
		unsigned int elem_num = count;
		while (0 < elem_num && value < values[elem_num-1])
		{ //While our inserted value is less than every number.
		values[elem_num] = values[--elem_num]; //Set next element in array to be previous value.
		}
		values[elem_num] = value;
		++count;
		return NULL; // to avoid warnings for now.
	}
	else if (NULL == this->leftSibling || NULL == this->rightSibling) 
	{
		return this;
	}

}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout <<"Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


