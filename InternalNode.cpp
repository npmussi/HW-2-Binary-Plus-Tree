#include <iostream>
#include "InternalNode.h"
using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
	InternalNode *p, BTreeNode *left, BTreeNode *right) :
	BTreeNode(LSize, p, left, right), internalSize(ISize)
{
	keys = new int[internalSize]; // keys[i] is the minimum of children[i]
	children = new BTreeNode* [ISize];
}//InternalNode::InternalNode()

int InternalNode::getMinimum()const
{
	if(count > 0)   // should always be the case
		return children[0]->getMinimum();
	else
		return 0;
} // InternalNode::getMinimum()


//FIXME: 1. Look for updating left and right sibling keys (in case of putting values into left and right siblings)
//    ^^ NEED TO MAKE SURE IT UPDATES FOR ALL CASES, ex. BTree12.dat 2 2 will break because it goes sends child right, and doesn't update
//FIXME: 2. somehow there was an empty internal node in the final answer, the rest of the internals were also screwed up
// 	 ^^ make sure that parent / child pointers are right

InternalNode* InternalNode::insert(int value)
{
    /*this function will find the correct next node to go to
        and return the node gone to   
        if the internal node is empty,
        creates keys[0] and insert*/

	if(count < internalSize)
	{//No split occurs.
		regularInsert(value);
    }
    else
	{
	//in this case, count == internalSize, but this doesn't mean that we need to look to siblings yet
	//we only should start looking to siblings when the leaf splits
	//go through the children array, check children near the value being inserted
		BTreeNode* returned_node = NULL;
		unsigned int index_num = 0;
		while (index_num < (count-1) && value < keys[index_num++]);//Increments index number until we find the correct position.
		returned_node = children[index_num]->insert(value);//Recursive call.
		updateKeys(index_num);
		return pickInsert(returned_node);
	}
    return NULL;
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{
    this->children[count++] = oldRoot;
    this->children[count++] = node2;
    this->keys[0] = oldRoot->getMinimum(); 
    this->keys[1] = node2->getMinimum();
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
//this method will insert the BTreeNode into the internal node
//must check for order, where does it go?
	int elem_num = count;
	while(0 < elem_num && newNode->getMinimum() < keys[elem_num - 1])
	{
		//move all elements over to the right
		//cout << "kms before keys " << endl;
		keys[elem_num] = keys[elem_num - 1];
		//cout << "kms after keys " << endl;
		children[elem_num] = children[elem_num - 1];
		elem_num--;
	}
	keys[elem_num] = newNode->getMinimum();
	children[elem_num] = newNode;
	children[elem_num]->setParent(this);//I'm its new parent
	count++;
} // InternalNode::insert()


void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;


  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;


  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()


InternalNode* InternalNode::insertSplit(BTreeNode* returned_node)
        /*Creates a new internal node to be made as the right sibling of the calling internal node.
        *This function determines how many elements to insert into the new right sibling
	*/
{
	InternalNode *new_internal = new InternalNode(internalSize, leafSize, this->parent, this, this->getRightSibling());
	int starting_index;
	if(this->rightSibling != NULL)
	{
		//I must make the old right sibling know who his new left is
		this->rightSibling->setLeftSibling(new_internal);
	}
	this->setRightSibling(new_internal);
	
	//Compare value to middle. If less than middle (insert left), shift more elems right. Vice versa otherwise.
	starting_index = (returned_node->getMinimum() < this->keys[internalSize/2]) ? (internalSize-1)/2 : (internalSize+1)/2;
	
	for (int elem_num = starting_index; elem_num < internalSize; elem_num++)//The +1 guarantees that if count is odd, one more element is in the left sibling.
	{
		new_internal->insert(this->children[elem_num]); //Put our elements in the new internal node.
		count--;//We now have less.
	}

	if(returned_node->getRightSibling()->getMinimum() < new_internal->children[0]->getMinimum())
	{
		this->insert(returned_node->getRightSibling());//WATCH OUT FOR SIBLING POINTER
	}
	else
	{
		new_internal->insert(returned_node->getRightSibling());//because the internal node only splits when there is one too many leaves
	}
	
	if (new_internal->children[0]->getLeftSibling() != NULL) //Leftmost nodes shouldn't have siblings.
	{
		new_internal->children[0]->getLeftSibling()->setRightSibling(NULL);
	}
	
	new_internal->children[0]->setLeftSibling(NULL);
	//THIS WILL ALWAYS APPLY: there will always be a leaf that is past the old "max"  
	return this; //If we return a pointer to an internal node, that means we need a new parent in every case.
}

void InternalNode::regularInsert(int value)
{
	BTreeNode *returned_node = NULL;
	int& num_keys = count;
	for(unsigned int index_num = 0; index_num < num_keys-1; ++index_num)
	{ //Compare the inserted value to the keys.
		//std::cout<<"Value is "<<value<<". Keys at index "<< index_num << " is "<<keys[index_num]<<" Count is"<<count<<std::endl;
		if(value < keys[index_num+1])
		{//Is the value less than minimum of node to right?
			returned_node = children[index_num]->insert(value);//Insert value at left of key.                
			keys[index_num] = children[index_num]->getMinimum();//Update key of this index number.
			keys[index_num + 1] = children[index_num+1]->getMinimum(); //update the key of the next one in case of right shift
			if(index_num != 0)
			{
			//then we have to update the left
			keys[index_num - 1] = children[index_num - 1]->getMinimum();
			}
			
			if(returned_node != NULL)
			{//Did this node split?
				unsigned int back_index = count - 1;//If so, we move every single node right.
				while (back_index > index_num)
				{
					//std::cout << "back_index: " << back_index << "index_num: " << index_num<< std::endl;
					children[back_index + 1] = children[back_index]; //Move right starting from back.
					keys[back_index + 1] = children[back_index]->getMinimum();
					back_index--;                   
					// SET LEFT AND RIGHT SIBLI- wait, nevermind, they're all the same still.
				}
				this->count++;
				children[index_num] = returned_node; 
				keys[index_num] = returned_node->getMinimum();
				children[index_num + 1] = returned_node->getRightSibling();
				keys[index_num + 1] = returned_node->getRightSibling()->getMinimum();
			}
			return;
		}
	}
	returned_node = children[count - 1]->insert(value);
	keys[count - 1] = children[count - 1]->getMinimum();
	if(returned_node != NULL)
	{
		keys[num_keys] = returned_node->getRightSibling()->getMinimum();
		children[count++] = returned_node->getRightSibling();
	} 
}

void InternalNode::insertLeft(BTreeNode* returned_node)
{
	//this method will insert the value into the left sibling
	((InternalNode*)(this->leftSibling))->insert(this->children[0]); //Insert first into left sibling.
	for (unsigned int elem_num = 0; elem_num < count - 1; elem_num++)
	{//Shift all elements to the left.
		children[elem_num] = children[elem_num + 1];
		keys[elem_num] = keys[elem_num + 1];
	}
	count--; //We lost an element, basically.
	this->insert(returned_node->getRightSibling()); //Insert element into now-non-empty leafNode.
}

void InternalNode::insertRight(BTreeNode* returned_node)
{
	//this method will insert the child into the right sibling
	((InternalNode*)(this->rightSibling))->insert(this->children[--count]);  //Well, we just don't care about the last element.
	this->insert(returned_node->getRightSibling());
}

void InternalNode::updateKeys(int index_num)
{
	/*Updates the keys of a node that are at
	 *or around a certain index value.
	 */
	 
	keys[index_num] = children[index_num]->getMinimum();
	std::cout << "right before" <<endl;
	
	if(children[index_num]->getLeftSibling() != NULL)
	{ // Shouldn't be able to seg fault.
		keys[index_num - 1] = children[index_num]->getLeftSibling()->getMinimum(); //THIS WILL SEG FAULT if you have a left sibling and are the first item in children
	}
	
	if(children[index_num]->getRightSibling() != NULL)
	{ // Shouldn't be able to seg fault.
		keys[index_num + 1] = children[index_num]->getRightSibling()->getMinimum();
	}
}

InternalNode* InternalNode::pickInsert(BTreeNode* returned_node)
{
	/*Decides where to put a node after
	 *it has split. Looks left first or
	 *ultimately splits. This assumes that
	 *the calling internal node is full.
	*/
	if(returned_node != NULL)
	{
		//if the leaf split, I must start looking to my siblings
		if (NULL != this->leftSibling && this->leftSibling->getCount() < internalSize)//leftSib exists, not full
		{ //insert into the left sibling
			insertLeft(returned_node);
		}
		else if (NULL != this->rightSibling && this->rightSibling->getCount() < internalSize)//rightSib exists, not full
		{ //insert into the right sibling
			insertRight(returned_node);
		}
		else
		{ //I must split
			return insertSplit(returned_node); //Returns itself because the parent needs to know that it has split.
		}
	}
	return NULL;
}
