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

bool LeafNode::isFull() { //Return if Node is full or not
    return count == leafSize;
}

//Insert value into the correct position in unfilled LeafNode
void LeafNode::insertSorted(int values[], int value) {
    if(count == 0) {
        values[0] = value;
        count++;
        if(parent) {
           parent->updateKey(this, values[0]);
        }
        return;
    }
    for(int pos = count - 1; pos >= 0; pos--){
        if(values[pos] > value) {
            values[pos + 1] = values[pos]; //Move up one index
            if(pos == 0){ //Inserting smallest element
                values[pos] = value;
                //update min key
                if(parent) {
                    parent->updateKey(this, values[0]);
                }
            }
        } else {
            values[pos + 1] = value; //Found correct index
            break;
        }
    }
    count++;
}

void LeafNode::deleteMin(int values[]) {
    for(int pos = 0; pos < count - 1; pos++) {
        values[pos] = values[pos + 1];
    }
    if(parent) {
        parent->updateKey(this, values[0]);
    }
    count--;
}

void LeafNode::transferToSibling(LeafNode* sibling, int values[], int value) {
    //Handle if size 1 exception
    if(leafSize == 1) {
        if(value > values[0]) {
            sibling->insert(value);
        } else {
            sibling->insertSorted(sibling->values, values[0]);
            values[0] = value;
        }
        return;
    }
    
    //Calculating stopping spot
    int indexMoveStop = leafSize/2;
    if(leafSize == 2) {
        indexMoveStop--;
    } else if(leafSize % 2 == 0){
        indexMoveStop--;
    }
    
    //Put in half or majority if odd into new sibling
    for(int pos = leafSize - 1; pos > indexMoveStop; pos--) {
        sibling->insert(values[pos]);
        count--;
    } //Compare the new value to the next greatest value
    if(value > values[indexMoveStop]) {
        sibling->insertSorted(sibling->values, value);
    } else {
        sibling->insertSorted(sibling->values, values[indexMoveStop]);
        count--;
        insertSorted(values, value);
    }
}

// students must write this
LeafNode* LeafNode::insert(int value)
{
    if(!isFull()) { //Insert into current Node because it will fit
        insertSorted(values, value);
    } else {
        if(leftSibling && !((LeafNode*)leftSibling)->isFull() ) {
            if(value < values[0]){ //Value is the minimum so give it to the left
                leftSibling->insert(value); 
            } else { //Give left sibling minimum of values and update values
                leftSibling->insert(values[0]);
                deleteMin(values);
                insertSorted(values, value);
            }
        } else if(rightSibling && !((LeafNode*)rightSibling)->isFull() ) { //left is full so check right
            if(value > values[count - 1]) { //Value is largest so pass it off to right 
                rightSibling->insert(value);
            } else {
                rightSibling->insert(values[count-1]);
                count--; //delete last item without actually changing array
                insertSorted(values, value);
            }
        } else { //Split Node!!
            LeafNode* newNode = new LeafNode(leafSize, parent, this, rightSibling);
            
            //Update siblings
            if(rightSibling) {
                rightSibling->setLeftSibling(newNode);
            }
            this->setRightSibling(newNode);
            
            //Move greater values into new node
            transferToSibling(newNode, values, value);

            //return newNode;
            return newNode;
      }
  }
  return NULL;
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


