#ifndef BinaryTreeH
    #define BinaryTreeH
    #include <iostream>
    using namespace std;
    
    //BNACKUP FILE DO NOT DELETE/CHANGE
    
    struct Character {
        int count;
        int ascii;
        Character *leftChild;
        Character *rightChild;
            
        Character() {};
        Character(int c, int val) {
            count = c;
            ascii = val;
            leftChild = NULL;
            rightChild = NULL;
        }
        Character& operator=(const Character& rhs) {
            count = rhs.count;
            ascii = rhs.ascii;
            leftChild = rhs.leftChild;
            rightChild = rhs.rightChild;
            return *this;
        }
        bool operator<(const Character& rhs) const {
            return count < rhs.count;
        }
        bool operator<=(const Character& rhs) const {
            return count <= rhs.count;
        }
        friend ostream& operator<<(ostream& out, const Character& ch) {
            out << (char)ch.ascii << "    " << ch.count;
            return out;
        }
    };
    
    class BinaryTree {
        public:
            BinaryTree() {
                root = NULL;
            }
            void printTree(Character *curr, string encoding) {
                if(curr && curr->ascii != -1) {
                    cout << (char)(curr->ascii) << "   " << curr->count << "    " << encoding << endl;
                    return;
                }
                if(curr->leftChild) {
                    encoding += '0';
                    printTree(curr->leftChild, encoding);
                }
                if(curr->rightChild) {
                    encoding += '1';
                    printTree(curr->rightChild, encoding);
                }
            }
            void setRoot(Character *newRoot) {
                root = newRoot;
            }
            Character* getRoot() {
                return root;
            }
        private:
            Character *root;
    };
    
#endif