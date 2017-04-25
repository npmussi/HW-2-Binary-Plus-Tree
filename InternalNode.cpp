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


//FIXME: 1. Look for updating left and right sibling keys (in case of putting values into left and right siblings)
//FIXME: 2. Inserts 12 at the end, 12 eats 11 instead of splitting

InternalNode* InternalNode::insert(int value)
{
    /*this function will find the correct next node to go to
        and return the node gone to   
        if the internal node is empty,
        creates keys[0] and insert*/
    //What is this is an InternalNode with a bunch of LeafNodes in regards to the return?
    //^ it will be a recursive function, recursively call insert(int) until the child is a leaf


    if(count < internalSize){
        BTreeNode *returned_node = NULL;
        int& num_keys = count;
        for(unsigned int index_num = 0; index_num < num_keys-1; ++index_num){ //Compare the inserted value to the keys.
            std::cout<<"hehe XD!! Value is "<<value<<". Keys at index "<< index_num << " is "<<keys[index_num]<<" Count is"<<count<<std::endl;
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
                        // SET LEFT AND RIGHT SIBLI- wait, nevermind, they're all the same still.
                    }
  		    		this->count++;
                    children[index_num] = returned_node; 
                    keys[index_num] = returned_node->getMinimum();
        	    	children[index_num + 1] = returned_node->getRightSibling();
                    keys[index_num + 1] = returned_node->getRightSibling()->getMinimum();
                }
                return NULL;
            }
            
        }
		returned_node = children[count - 1]->insert(value);
cout << "I just made a returned" << count <<endl;
		keys[count - 1] = children[count - 1]->getMinimum();
		if(returned_node != NULL){
cout << "im in the if" << endl;
			keys[num_keys] = returned_node->getRightSibling()->getMinimum();
			children[count++] = returned_node->getRightSibling();
		}  
		return NULL;
    }
    else{
	//in this case, count == intermalSize, but this doesn't mean that we need to look to siblings yet
	//we only should start looking to siblings when the leaf splits
	//go through the children array, check children near the value being inserted
		BTreeNode* returned_node = NULL;
		for(int i = 0; i < count - 1; i++){
			if(value < keys[i + 1]){
				returned_node = children[i]->insert(value);
				keys[i] = children[i]->getMinimum();
	if(children[i]->getLeftSibling() != NULL){ // FIXME: this doesn't always work, you still have a left sibling at children[0] (if you're a right sibling of someone else)
		keys[i - 1] = children[i - 1]->getMinimum();
	}
	if(children[i]->getRightSibling() != NULL){ //FIXME: SAME AS ABOVE
		keys[i + 1] = children[i + 1]->getMinimum();
	}
				if(returned_node != NULL){
					//if the leaf split, I must start looking to my siblings
					
					if (NULL != this->leftSibling && this->leftSibling->getCount() < internalSize)//leftSib exists, not full
	    			{ //insert into the left sibling
	        			    insertLeft(value);
	    			}
	    			else if (NULL != this->rightSibling && this->rightSibling->getCount() < internalSize)//rightSib exists, not full
	    			{ //insert into the right sibling
	    			        insertRight(value);
	    			}
	    			else{ //I must split
cout<< "im in split in for"<<endl;
	    			     return insertSplit(returned_node); //Returns itself because the parent needs to know that it has split.
	    			}
				}
				return NULL;
			}
		}
		//if you got out here, that means that we need to insert into the last child
cout << "countAAAAA: " << count << endl;
		returned_node = children[count - 1]->insert(value);
		keys[count - 1] = children[count - 1]->getMinimum();
		if(returned_node != NULL){
			//I must look to my siblings
			
			if (NULL != this->leftSibling && this->leftSibling->getCount() < internalSize)//leftSib exists, not full
	    	{ //insert into the left sibling
	        	    insertLeft(value);
	    	}
	    	else if (NULL != this->rightSibling && this->rightSibling->getCount() < internalSize)//rightSib exists, not full
	    	{ //insert into the right sibling
	    	        insertRight(value);
	    	}
	    	else{ //I must split
cout<< "im in split out for"<<endl;
	    	     return insertSplit(returned_node); //Returns itself because the parent needs to know that it has split.
	    	}
		}
		return NULL;
    }
    return NULL;//PLACEHOLDER
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
cout << "count: " << count << endl;
newNode->getMinimum();
cout << "lul test" << endl;
//this method will insert the BTreeNode into the internal node
//must check for order, where does it go?
	int elem_num = count;
	while(0 < elem_num && newNode->getMinimum() < keys[elem_num - 1]){
		//move all elements over to the right
cout << "kms before keys " << endl;		
		keys[elem_num] = keys[elem_num - 1];
cout << "kms after keys " << endl;		
		children[elem_num] = children[elem_num - 1];
		elem_num--;
	}
	keys[elem_num] = newNode->getMinimum();
	children[elem_num] = newNode;
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
            unsigned int starting_index;
            if(this->rightSibling != NULL){
                //I must make the old right sibling know who his new left is
                this->rightSibling->setLeftSibling(new_internal);
            }
            this->setRightSibling(new_internal);
            if (returned_node->getMinimum() < this->keys[internalSize/2])//Compare value to middle.
            {
                starting_index = (internalSize-1)/2;//If less than middle (insert left), shift more elems right.
            }
            else
            {
                starting_index = (internalSize+1)/2;//If more than middle (insert right), shift less elem right
            }
            for (unsigned int elem_num = starting_index; elem_num < internalSize; elem_num++)//The +1 guarantees that if count is odd, one more element is in the left sibling.
            {
cout << "moving time, start: " << starting_index << "elem_num: " << elem_num << endl;
                new_internal->insert(this->children[elem_num]); //Put our elements in the new internal node
                count--;//We now have less.
            }
			new_internal->insert(returned_node->getRightSibling());//because the internal node only splits when there is one too many leaves
			//THIS WILL ALWAYS APPLY: there will always be a leaf that is past the old "max"  
			//VERY SKEPTICAL ABOUT THIS LINE STILL
            return this; //If we return a pointer to an internal node, that means we need a new parent in every case.
}


void InternalNode::insertLeft(int value)
{
   //this method will insert the value into the left sibling
        ((InternalNode*)(this->leftSibling))->insert(this->children[0]); //Insert minimum into left sibling.
        for (unsigned int elem_num = 0; elem_num < count - 1; elem_num++)
        {//Shift all elements to the left.
            children[elem_num] = children[elem_num + 1];
	    keys[elem_num] = keys[elem_num + 1];
        }
        count--; //We lost an element, basically.
        this->insert(value); //Insert element into now-non-empty leafNode.
}

void InternalNode::insertRight(int value)
{
   //this method will insert the child into the right sibling
        ((InternalNode*)(this->rightSibling))->insert(this->children[--count]);  //Well, we just don't care about the last element.
        this->insert(value);
}

bool InternalNode::isFull(){
	//this method will check if this internal node is full
	return this->isFull();
}
