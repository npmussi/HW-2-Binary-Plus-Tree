Okay, so here we are trying to make a B+ Tree.
The only modifiable files are BTree.cpp, InternalNode.cpp, InernalNode.h, LeafNode.cpp, LeafNode.h, authors.csv.

BTreeDriver.cpp
  Driver program. Makes a tree, inserts stuff into it, and prints it.

BTree.cpp/BTree.h
  Contains the actual tree itself. It has a pointer to its root, the size of the internal nodes (m; how many numbers + 1 can fit into the internal), and the size of each leaf (L; how many elements can be stored in a leaf). It's here where we define the insert() function, which will be the most difficult part. It also has print() and its constructor predefined. Ignore those, really.
  
insert()
  
BTreeNode.cpp/BTreeNode.h (NOT MODIFIABLE!)
  Contains the various things that we define a node by; this is the parent class of InternalNode and LeafNode. Contains the number of elements currently in the node (count), the maximum number of elements (leafSize), a pointer to its parent InternalNode, and two pointers to its left and right BTreeNode siblings.
    Its non-virtual functions are trivial to describe. It has three virtual functions that are redefined in InternalNode and LeafNode:   
      getMinimum(), which returns the first element of the nodes (for InternalNode, it returns the lowest/leftmost "key", or just the value, while for LeafNode it returns the lowest "value") and should primarily be used for comparing items with "<".
      BTreeNode::insert(int value), which attempts to insert a value. We have to implement it such that it returns a new node if it splits or else it returns NULL. Keep in mind that the one in InternalNode is overloaded.
      print(), which is already defined for us.
  
InternalNode.cpp/InternalNode.h
  The internal nodes that point to either other internal nodes or to 


The testhw and the Testing directory are now available for use.
Copy testhw to one of your own directories.
There is no need to copy the Testing directory.
testhw takes only one argument--the directory that holds your BTree source code.
testhw will copy from the Testing directory those files that you were not suppose to change.
It will also copy a special Makefile, and some .dat files.
It then compiles your program, and runs 10 tests on it.  
The results are placed in the results.txt file in the directory with the source code in it.

Sean
