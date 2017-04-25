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
	if (count != leafSize) 
	{ //inserts into the leaf regularly
//cout << "leaf reg insert" << endl;
		regularInsert(value);
	}
	else if (NULL != this->leftSibling && this->leftSibling->getCount() < leafSize)//leftSib exists, not full
	{ //insert into the left sibling
		insertLeft(value);
	}
	else if (NULL != this->rightSibling && this->rightSibling->getCount() < leafSize)//rightSib exists, not full
	{ //insert into the right sibling
		insertRight(value);
	}
	else //Left and right full/don't exist, so we make a new parent node.
	{
		return insertSplit(value);
	}
	return NULL; //We default to returning NULL unless a split occurs.

}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout <<"Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()

void LeafNode::regularInsert(int value)
{
    //this method will insert the value into the values array
    //it will only be called when under the "perfect" conditions
    unsigned int elem_num = count++; //Element num is of last element, update count to show that we are adding another.
	while (0 < elem_num && value < values[elem_num-1])
	{ //While our inserted value is less than every number.
	    values[elem_num] = values[--elem_num]; //Set next element in array to be previous value.
	}
	values[elem_num] = value;
}

void LeafNode::insertLeft(int value)
{
    //this method will insert the value into the left sibling
	this->leftSibling->insert(this->values[0]); //Insert minimum into left sibling.
	for (unsigned int elem_num = 0; elem_num < count - 1; elem_num++)
	{//Shift all elements to the left.
	    values[elem_num] = values[elem_num + 1];
	}
	count--; //We lost an element, basically.
	this->insert(value); //Insert element into now-non-empty leafNode.
}

void LeafNode::insertRight(int value)
{
    //this method will insert the value into the right sibling
	this->rightSibling->insert(this->values[--count]);  //Well, we just don't care about the last element.
	this->insert(value);
	
}

LeafNode* LeafNode::insertSplit(int value)
{
//cout <<  "leaf split " << endl;
	    LeafNode *new_leaf = new LeafNode(leafSize, this->parent, this, this->getRightSibling());
	    unsigned int starting_index;
	    if(this->rightSibling != NULL){
	        //I must make the old right sibling know who his new left is
		this->rightSibling->setLeftSibling(new_leaf);
	    }
	    this->setRightSibling(new_leaf);
	    if (value < this->values[leafSize/2])//Compare value to middle.
	    {
	        starting_index = (leafSize-1)/2;//If less than middle (insert left), shift more elems right.
	    }
	    else
	    {
	        starting_index = (leafSize+1)/2;//If more than middle (insert right), shift less elem right
	    }
	    for (unsigned int elem_num = starting_index; elem_num < leafSize; elem_num++)//The +1 guarantees that if count is odd, one more element is in the left sibling.
	    {
	        new_leaf->insert(this->values[elem_num]); //Put our elements in the new leaf node.
	        count--;//We now have less.
	    }
	    if (value < this->values[(leafSize - 1)/2])
	    {
	        this->insert(value);
	    }
	    else
	    {
	        new_leaf->insert(value);
	    }
	    return this; //If we return a leaf pointer, that means we need a new parent in every case.
}

bool LeafNode::isFull(){
	//this method will check if the leaf is full or not
	if(this->count == leafSize){
		return true;
	}
	return false;
}
