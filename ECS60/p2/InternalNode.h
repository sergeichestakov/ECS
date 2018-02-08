#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  int getMinimum()const;
  bool isFull();
  void deleteMin();
  void updateKey(BTreeNode *child, int newMin);
  void updateChild(BTreeNode *child, BTreeNode *newChild);
  void insertSorted(BTreeNode *newNode, int min);
  void transferToSibling(InternalNode* sibling, BTreeNode *newNode);
  InternalNode* insert(int value); // returns pointer to new InternalNode
    // if it splits else NULL
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);
}; // InternalNode class

#endif
