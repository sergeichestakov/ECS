#include <fstream>
#include <iomanip>
#include "BinaryHeap.h"
#include "BinaryTree.h"

class Character {
    public:
    int count;
    int ascii;
        
    Character() {};
    Character(int c, int val) {
        count = c;
        ascii = val;
    }
    Character& operator=(const Character& rhs) {
        count = rhs.count;
        ascii = rhs.ascii;
        return *this;
    }
    bool operator<(const Character& rhs) const {
        return count < rhs.count;
    }
    bool operator<=(const Character& rhs) const {
        return count <= rhs.count;
    }
    friend ostream& operator<<(ostream& out, const Character& ch){
        out << (char)ch.ascii << setw(5) << ch.count;
        return out;
    }
};

class BinaryTreePtr {
    public:
    BinaryTree<Character> *tree;
    
    BinaryTreePtr(){}
    BinaryTreePtr(BinaryTree<Character> *t) {
        tree = t;
    }
    BinaryTreePtr operator=(const BinaryTreePtr& rhs) {
        tree = rhs.tree;
        return *this;
    }
    bool operator<(const BinaryTreePtr rhs) const {
        return *tree < *rhs.tree;
    }
    bool operator<=(const BinaryTreePtr rhs) const {
        return *tree <= *rhs.tree;
    }
};

class Huffman {
    public:
        static BinaryTree<Character> makeTree(int (&counts)[256]) {
            BinaryHeap< BinaryTreePtr > heap(256);
            BinaryTree<Character> root;
            //Throw the used characters in a heap
            for(int i = 0; i < 256; i++) {
                if(counts[i] > 0) {
                    Character* c = new Character(counts[i], i);
                    BinaryTree<Character> *newTree = new BinaryTree<Character>(c);
                    BinaryTreePtr newPtr(newTree);
                    heap.insert(newPtr);
                }
            }
            
            //next
            while(!heap.isEmpty()) {
                BinaryTreePtr first;
                BinaryTreePtr second;
                heap.deleteMin(first);
                heap.deleteMin(second); //Remove next two minimums of heap
                
                Character *p = new Character(first.tree->root->count + second.tree->root->count, 0); //Keeps track of the sum of the two children
                BinaryTree<Character> *parent = new BinaryTree<Character>(p); //Create a new Binary Tree with sum of children
                parent->insert(first.tree, second.tree); //Insert the two mins into the parent, which is a root
                BinaryTreePtr parentPtr(parent);
                if(!heap.isEmpty()){ //Reinsert new tree back into heap if there are more characters
                    heap.insert(parentPtr);
                } else {
                    root = *(parentPtr.tree); //Otherwise tree is complete
                }
            }
            return root;
        };
};

int main(int argc, char* argv[]) {
    ifstream input(argv[1]);
    
    //Read input and update array
    int asciiCount[256];
    for(int i=0;i<256;i++) {
        asciiCount[i] = 0;
    }
    char currChar;
    while(input.get(currChar)) {
        asciiCount[(int)currChar]++;
    }
    
    //Create and print tree
    BinaryTree<Character> tree = Huffman::makeTree(asciiCount);
    tree.printTree();
}