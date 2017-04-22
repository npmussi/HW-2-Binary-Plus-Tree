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


LeafNode* LeafNode::insert(int value)
{
  /* this method will insert the value into the values array
     keeping it sorted.
	DOES NOT CHECK IF IT IS FULL, COME HERE ON CONDITION THAT
	VALUES IS NOT FULL
  */

   for(int i = 0; i < count; ++i){
      if(value < values[i]){
         //upon reaching this point, move everything down by one and place item in
			for(int j = count; j > i; ++j){
				values[j] = values[j-1];
			}
			values[i] = value;
      }
   }
  return this;
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


