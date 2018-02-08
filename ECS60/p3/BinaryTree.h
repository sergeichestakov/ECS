#ifndef BinaryTreeH
    #define BinaryTreeH
    #include <iostream>
    using namespace std;
    
    template <class Comparable>
    class BinaryTree {
        public:
            BinaryTree<Comparable>(){}
            BinaryTree<Comparable>(Comparable* newRoot) {
                root = newRoot;
                leftTree = NULL;
                rightTree = NULL;
            }
            void insert(BinaryTree<Comparable> *first, BinaryTree<Comparable> *second){ //Insert
                if(*first <= *second){
                    leftTree = first;
                    rightTree = second;
                } else {
                    leftTree = second;
                    rightTree = first;
                }  
            }    
            void printTree(){
                printEncode("");
            }
            bool operator<(const BinaryTree<Comparable>& rhs) const {
                return *root < *rhs.root;
            }
            bool operator<=(const BinaryTree<Comparable>& rhs) const {
                return *root <= *rhs.root;
            }
            BinaryTree<Comparable> *leftTree;
            BinaryTree<Comparable> *rightTree;
            Comparable *root;
        private:
            void printEncode(string encoding){
                if(!leftTree && !rightTree){ //Leaf Node
                    cout << *root << " " << encoding << endl;
                    return;
                }
                leftTree->printEncode(encoding + '0');
                rightTree->printEncode(encoding + '1');
            }
    };
    
#endif