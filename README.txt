Okay, so here we are trying to make a B+ Tree.
The only modifiable files are BTree.cpp, InternalNode.cpp, InernalNode.h, LeafNode.cpp, LeafNode.h, authors.csv.

BTreeDriver.cpp
  Driver program. Makes a tree, inserts stuff into it, and prints it.

BTree.cpp/BTree.h
  Contains the actual tree itself. It has a pointer to its root, the size of the internal nodes (m; how many numbers + 1 can fit into the internal), and the size of each leaf (L; how many elements can be stored in a leaf). It's here where we define the insert() function, which will be the most difficult part. It also has print() and its constructor predefined. Ignore those, really.
  
BTreeNode.cpp/BTreeNode.h (NOT MODIFIABLE!)
  Contains the various things that we define a node by; this is the parent class of InternalNode and LeafNode. Contains the number of elements currently in the node (count), the maximum number of elements in a leaf (leafSize), a pointer to its parent InternalNode, and two pointers to its left and right BTreeNode siblings.
    Its non-virtual functions are trivial to describe. It has three virtual functions that are redefined in InternalNode and LeafNode:   
      getMinimum(), which returns the first element of the nodes (for InternalNode, it returns the lowest/leftmost "key", or just the value, while for LeafNode it returns the lowest "value") and should primarily be used for comparing items with "<".
      BTreeNode::insert(int value), which attempts to insert a value. We have to implement it such that it returns a new node if it splits or else it returns NULL. Keep in mind that the one in InternalNode is overloaded.
      print(), which is already defined for us.
  
InternalNode.cpp/InternalNode.h
  The internal nodes, derived from class BTreeNodes, that point to either other internal nodes or to leaf nodes; specifically, these contain an array of pointers to BTreeNodes and an array of "keys" (the minimum values inside each of the BTreeNodes) that we use to decide where to move in the tree. Also contains an integer initialized at construction that tells us its max possible size. Note that it still contains the pointers from BTreeNode; we'd use "leafSize" from BTreeNodes to tell us the max size of a leaf and "count" to determine how many elements are currently in the internal node. Insertion still needs to be defined by us.
  
LeafNode.cpp/LeafNode.h
  The leaf nodes, derived from class BTreeNodes, that contain an array of integers. Note that it has the members from BTreeNode. Use "leafSize" for the maximum number of elements in the leaf.
  
Insertion
  This is the only part of the assignment we are really assigned to do, but we need to do all of it.
  The lower valued the element is, the farther left it is put, and the lower its index number is on a leafNode's "values" array. 
  
Other Rules and Notes
  1. No node excluding the root can be less than half full. We are only concerned with insertions for this assignment, so the only condition where a node may even get close to half full is on insertion into a node that is already full. For InternalNodes, this number at which a split is triggered is "internalSize" from BTree, and for LeafNodes, it is "leafSize". When a node gets one more element (be it an integer or another node) than it can handle, it splits into two. If the nodes may only hold an even number of elements, then the left node gets half of the elements and the right node gets half plus one; otherwise, it's evenly distributed.
    i. If the above internal node (call it node #1 and its parent node #0) already is full, then we create another new internal node (#2) now pointed to by #1's previous parent node #0, have #2 point to #1, then have a new node #3 pointed to by node #2. We make it such that the leftmost node pointed to by node #2 is node #1.
  2. When inserting values into a tree and there are siblings to the current leaf we are checking, check the left sibling first, then the right one.
    


The testhw and the Testing directory are now available for use.
Copy testhw to one of your own directories.
There is no need to copy the Testing directory.
testhw takes only one argument--the directory that holds your BTree source code.
testhw will copy from the Testing directory those files that you were not suppose to change.
It will also copy a special Makefile, and some .dat files.
It then compiles your program, and runs 10 tests on it.  
The results are placed in the results.txt file in the directory with the source code in it.

Sean
