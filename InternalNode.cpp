#include <iostream>
#include "InternalNode.h"
using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
    /*this function will find the correct next node to go to
        and return the node gone to   
        if the internal node is empty,
        creates keys[0] and insert*/
    //FIXME What is this is an InternalNode with a bunch of LeafNodes in regards to the return?
    //^ it will be a recursive function, recursively call insert(int) until the child is a leaf

    if(count < internalSize){
        BTreeNode *returned_node = NULL;
        int& num_keys = count;
        for(unsigned int index_num = 0; index_num < num_keys-1; ++index_num){ //Compare the inserted value to the keys.
//            std::cout<<"hehe XD!! Value is "<<value<<". Keys at index "<< index_num << " is "<<keys[index_num]<<" Count is"<<count<<std::endl;
            if(value < keys[index_num+1]){//Is the value less than minimum of node to right?
                returned_node = children[index_num]->insert(value);//Insert value at left of key.                
		keys[index_num] = children[index_num]->getMinimum();//Update key of this index number.
		keys[index_num + 1] = children[index_num+1]->getMinimum(); //update the key of the next one in case of right shift
                if(returned_node != NULL){ //Did this node split?
                    unsigned int back_index = count - 1;//If so, we move every single node right.
                    while (back_index > index_num)
                    {
//std::cout << "back_index: " << back_index << "index_num: " << index_num<< std::endl;
                        children[back_index + 1] = children[back_index]; //Move right!
			keys[back_index + 1] = children[back_index]->getMinimum();
			back_index--; // we should decrement here because I have to change keys array
                        this->count++;
			// SET LEFT AND RIGHT SIBLI- wait, nevermind, they're all the same still.
                    }
                    children[index_num] = returned_node; 
                    keys[index_num] = returned_node->getMinimum();
		    children[index_num + 1] = returned_node->getRightSibling();
		    keys[index_num + 1] = returned_node->getRightSibling()->getMinimum();
                }
                return NULL;
            }
            
        }
        //Just have children[count] = returned_node and update key.
//        std::cout<<"Value is "<<value<<". Keys at index 0 is "<<keys[0]<<" Count is"<<count<<std::endl;
        returned_node = children[count-1]->insert(value);
	keys[count - 1] = children[count - 1]->getMinimum();
        if (returned_node != NULL)
        {
            keys[num_keys] = returned_node->getRightSibling()->getMinimum();
            children[count++] = returned_node->getRightSibling(); 
        }
        //if we got out here, then you need to insert into the last child
        //count++;
        return NULL;
    }
    else if (NULL != this->leftSibling && this->leftSibling->getCount() < internalSize)//leftSib exists, not full
	{ //insert into the left sibling
//	    insertLeft(value);
	}
	else if (NULL != this->rightSibling && this->rightSibling->getCount() < internalSize)//rightSib exists, not full
	{ //insert into the right sibling
	 //   insertRight(value);
	}
    else{
            //then count == internalSize, you must split 
            //BUT ONLY IF THE LEAF SPLITS TOO
//KILLME
    //V 2.0 code lul
    /*for(unsigned int index_num = 0; index_num < internalSize; ++index_num){
        if(value < keys[index_num]){
           //insert into children[i]
           BTreeNode* new_node = children[index_num]->insert(value);
            if(new_node == NULL){ 
                //then it means that insert was valid, no split
                return NULL;
            }
            else{
                //insert caused leaf to split, new_leaf is the new leaf
                //because it needed to split, I must split as well
                
                InternalNode* new_internal = new InternalNode(internalSize, leafSize, NULL, this, rightSibling);
                this->rightSibling = new_internal;
                //now, put my children / keys into new_internal
                int halfNum = count/2;
                for(int i = halfNum; i < internalSize; ++i){
                    new_node->keys[i - halfNum] = this->keys[i];
                    new_node->children[i - halfNum] = this->children[i];
                    count--; //killing my children
                }
                return new_internal;
            }
            
        }
    }*/
    return NULL;//PLACEHOLDER
    }
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
	//oldRoot is the root's BTreeNode pointer, old node2 has parent defined (new_internal) and oldRoot as left sibling.
	//oldRoot has the new internal node being made as parent
    //JAAAAAAAAAAAAAAMES! xd xd!!!!!!!!!!!!!!!!
    //This function is only called when we are inserting a new internal node
    //root. As a result, there's only two internal nodes we deal with.
    this->children[count++] = oldRoot;
    this->children[count++] = node2;
    this->keys[0] = oldRoot->getMinimum(); 
    this->keys[1] = node2->getMinimum();
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
    //inserting into internal node after splitting

    
} // InternalNode::insert()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl << count << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()

/*void InternalNode::insertLeft(int value){
    //this method will insert the value into the left sibling
	this->leftSibling->keys[this->leftSibling->count];
	
	insert(this->values[0]); //Insert minimum into left sibling.
	for (unsigned int elem_num = 0; elem_num < count - 1; elem_num++)
	{//Shift all elements to the left.
	    values[elem_num] = values[elem_num + 1];
	}
	count--; //We lost an element, basically.
	this->insert(value); //Insert element into now-non-empty leafNode.
}*/

