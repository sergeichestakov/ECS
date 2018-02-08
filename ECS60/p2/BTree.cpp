#include <iostream>
#include "BTree.h"
#include "BTreeNode.h"
#include "LeafNode.h"
#include "InternalNode.h"
using namespace std;

BTree::BTree(int ISize, int LSize):internalSize(ISize), leafSize(LSize)
{
  root = new LeafNode(LSize, NULL, NULL, NULL);
} // BTree::BTree()

// students must write this
void BTree::insert(const int value)
{
    BTreeNode *sibling = root->insert(value);
    if(sibling) {
        InternalNode *newRoot = new InternalNode(internalSize, leafSize, NULL, NULL, NULL);
        ((InternalNode*)newRoot)->insert(root, sibling);
        root = newRoot;
    }
} // BTree::insert()


void BTree::print()
{
  BTreeNode *BTreeNodePtr;
  Queue<BTreeNode*> queue(1000);

  queue.enqueue(root);
  while(!queue.isEmpty())
  {
    BTreeNodePtr = queue.dequeue();
    BTreeNodePtr->print(queue);
  } // while
} // BTree::print()
