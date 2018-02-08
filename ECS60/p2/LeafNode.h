#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;
  void insertSorted(int values[], int value);
  void deleteMin(int values[]);
  void transferToSibling(LeafNode* sibling, int values[], int value);
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
  // else NULL
  void print(Queue <BTreeNode*> &queue);
  bool isFull();
}; //LeafNode class

#endif
