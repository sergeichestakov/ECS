#include <iostream>
#include "InternalNode.h"
#include "LeafNode.h"

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


bool InternalNode::isFull(){
  return count == internalSize;
}

void InternalNode::updateKey(BTreeNode *child, int newMin) {
  for(int i=0;i<count;i++) {
    if(children[i] == child) {
      keys[i] = newMin;
      if(i == 0 && parent) { //the smallest value in keys
        parent->updateKey(this, newMin);
      }
    }
  }
}

void InternalNode::updateChild(BTreeNode *child, BTreeNode *newChild) {
  for(int i=0;i<count;i++) {
    if(children[i] == child) {
      children[i] = newChild;
      //keys[i] = newChild->getMinimum();
      updateKey(newChild, newChild->getMinimum());
    }
  }
}

void InternalNode::deleteMin() {
    for(int pos = 0; pos < count - 1; pos++) {
        children[pos] = children[pos + 1];
        //keys[pos] = keys[pos + 1];
        updateKey(children[pos], children[pos]->getMinimum());
    }
    /*if(parent) {
        parent->updateKey(this, keys[0]);
    }*/
    count--;
}

InternalNode* InternalNode::insert(int value)
{
  BTreeNode *newLeaf = NULL;
  InternalNode *newInternal = NULL;
  bool childIns = false;
  for(int pos = 0; pos < count-1; pos++) {
    if(value < keys[pos+1]) {
      newLeaf = children[pos]->insert(value);
      childIns = true;
      break;
    }
  }
  if(!childIns) {
    newLeaf = children[count-1]->insert(value);
  }
  
  if(newLeaf) {
    if(!isFull()) {
      insert(newLeaf);
    } else { //is full
      if(leftSibling && !((InternalNode*)leftSibling)->isFull()) { //left has space so hand off smallest child
        ((InternalNode*)leftSibling)->insert(children[0]);
        deleteMin();
        insert(newLeaf);
      } else if(rightSibling && !((InternalNode*)rightSibling)->isFull()) { //right has space so hand off largest child
      
        if(newLeaf->getMinimum() > children[count - 1]->getMinimum()){ //New Leaf is greater
          ((InternalNode*)rightSibling)->insert(newLeaf);
        } else { //Give right largest child and insert new one
          ((InternalNode*)rightSibling)->insert(children[count - 1]);
          count--;
          insert(newLeaf);
        }
      } else { //Split
        newInternal = new InternalNode(internalSize, leafSize, parent, this, rightSibling);
        transferToSibling(newInternal, newLeaf);
        //Update the sibling pointer for this instance
        this->setRightSibling(newInternal);
      }
      //parent->insert(newInternal);
    }
  }
  return newInternal;
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  oldRoot->setParent(this);
  insert(oldRoot);
  node2->setParent(this);
  insert(node2);
} // InternalNode::insert()

//Insert node into the correct position of children
void InternalNode::insertSorted(BTreeNode *newNode, int min) {
  newNode->setParent(this);
  for(int pos = count - 1; pos >= 0; pos--) {
      if(keys[pos] > min) {
          children[pos + 1] = children[pos]; //Move up one index
          keys[pos + 1] = keys[pos]; //Move up one index
          //updateKey(children[pos+1], children[pos+1]->getMinimum());
          if(pos == 0){ //Inserting smallest element
              count++;
              children[pos] = newNode;
              updateKey(newNode, newNode->getMinimum());
          }
      } else {
          count++;
          children[pos + 1] = newNode;
          //keys[pos + 1] = min; //Found correct index
          updateKey(newNode, newNode->getMinimum());
          break;
      }
  }
}

//
void InternalNode::transferToSibling(InternalNode* sibling, BTreeNode *newNode) {
    //Handle if size 1 exception
    if(internalSize == 1) {
      newNode->setParent(sibling);
      sibling->insert(newNode);
      return;
    }
    
    //Calculating stopping spot
    int indexMoveStop = internalSize/2;
    if(internalSize == 2) {
      indexMoveStop--;
    } else if(internalSize % 2 == 0){
      indexMoveStop--;
    }
    
    //Put in half or majority if odd into new sibling
    for(int pos = internalSize - 1; pos > indexMoveStop; pos--) {
      children[pos]->setParent(sibling);
      sibling->insert(children[pos]);
      count--;
    } //Compare the new value to the next greatest value
    if(newNode->getMinimum() > keys[indexMoveStop]) {
        sibling->insertSorted(newNode, newNode->getMinimum());
    } else {
        children[indexMoveStop]->setParent(sibling);
        sibling->insertSorted(children[indexMoveStop], keys[indexMoveStop]);
        count--;
        insertSorted(newNode, newNode->getMinimum());
    }
}

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  //Insert a leaf node
  if(isFull()) { //if full of internal nodes and have no space for another
    InternalNode *newInternalRoot = new InternalNode(internalSize, leafSize, parent, leftSibling, rightSibling);
    newInternalRoot->insert(this, newNode);
    parent->updateChild(this, newInternalRoot);
  } else if(count == 0) {
    count++;
    newNode->setParent(this);
    children[0] = newNode;
    //keys[0] = newNode->getMinimum();
    updateKey(newNode, newNode->getMinimum());
  } else {
    insertSorted(newNode, newNode->getMinimum());
  }
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


