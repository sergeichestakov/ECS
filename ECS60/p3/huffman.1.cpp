#include <fstream>
#include "BinaryHeap.h"
#include "BinaryTree.1.h"

//BNACKUP FILE DO NOT DELETE/CHANGE

class Encoder {
    public:
        void encode(BinaryTree &tree, int (&counts)[256]) {
            const Character EMPTY(0, -1);
            BinaryHeap<Character> heap(256);

            //Throw the used characters in a heap
            for(int i = 0; i < 256; i++) {
                if(counts[i] > 0) {
                    Character c(counts[i], i);
                    heap.insert(c);
                }
            }
            
            //next
            while(!heap.isEmpty()) {
                Character first = EMPTY;
                Character second = EMPTY;
                heap.deleteMin(first);
                heap.deleteMin(second);
                
                Character parent = EMPTY;
                parent.count += first.count + second.count;
                
                if(first <= second) {
                    parent.leftChild = &first;
                    parent.rightChild = &second;
                } else {
                    parent.leftChild = &second;
                    parent.rightChild = &first;
                }
                
                if(heap.isEmpty()) {
                    cout << parent.count << "--";
                    tree.setRoot(&parent);
                } else {
                    heap.insert(parent);
                }
            }
        };
};

int main(int argc, char** argv) {
    //ifstream input(argv[1]);
    ifstream input("weissHuffman.txt");
    
    //Read input and update array
    int asciiCount[256];
    for(int i=0;i<256;i++) {
        asciiCount[i] = 0;
    }
    char currChar;
    while(input.get(currChar)) {
        asciiCount[(int)currChar]++;
    }
    
    //Call function
    BinaryTree tree;
    Encoder encoder;
    encoder.encode(tree, asciiCount);
    cout << tree.getRoot()->count << "--";
    tree.printTree(tree.getRoot(), "");
    //tree.printTree(tree.getRoot(), "");
}