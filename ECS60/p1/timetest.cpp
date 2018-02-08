#include <iostream>
#include <fstream>
#include "CPUTimer.h"
#include "dsexceptions.h"
#include "vector.h"
#include "LinkedList.h"
#include "CursorList.h"
#include "StackAr.h"
#include "StackLi.h"
#include "QueueAr.h"
#include "SkipList.h"

using namespace std;

vector<CursorNode <int> > cursorSpace(500001);

void RunList(string filename) {
    ifstream in(filename.c_str());
    string description;
    getline(in, description);
    
    List<int> vals;
    char command;
    int data;
    while(in >> command >> data) {
        if(command == 'i') {
            vals.insert(data, vals.zeroth());
        } else {
            vals.remove(data);
        }
    }
    in.close();
}

void RunCursorList(string filename) {
    ifstream in(filename.c_str());
    string description;
    getline(in, description);
    
    CursorList<int> vals(cursorSpace);
    char command;
    int data;
    while(in >> command >> data) {
        if(command == 'i') {
            vals.insert(data, vals.zeroth());
        } else {
            vals.remove(data);
        }
    }
    in.close();
}

void RunStackAr(string filename) {
    ifstream in(filename.c_str());
    string description;
    getline(in, description);
    
    StackAr<int> vals(500001);
    char command;
    int data;
    while(in >> command >> data) {
        if(command == 'i') {
            vals.push(data);
        } else {
            vals.pop();
        }
    }
    in.close();
}

void RunStackLi(string filename){
    ifstream in(filename.c_str());
    string description;
    getline(in, description);
    
    StackLi<int> s;
    char command;
    int data;
    while(in >> command >> data) {
        if(command == 'i') {
            s.push(data);
        } else {
            s.pop();   
        }
    }
    in.close();
}

void RunQueueAr(string filename){
    ifstream in(filename.c_str());
    string description;
    getline(in, description);
    
    Queue<int> q(500001);
    char command;
    int data;
    while(in >> command >> data) {
        if(command == 'i') {
            q.enqueue(data);
        } else {
            q.dequeue();   
        }
    }
    in.close();
}

void RunSkipList(string filename){
    ifstream in(filename.c_str());
    string description;
    getline(in, description);
    
    SkipList<int> vals(-1, 500001);
    char command;
    int data;
    while(in >> command >> data) {
        if(command == 'i') {
            vals.insert(data);
        } else {
            vals.deleteNode(data);   
        }
    }
    in.close();
}

int getChoice() {
    //Setup variables
    int choice;
    
    //Print menu
    cout << endl;
    cout << "      ADT Menu" << endl; //check how many spaces needed
    cout << "0. Quit" << endl;
    cout << "1. LinkedList" << endl;
    cout << "2. CursorList" << endl;
    cout << "3. StackAr" << endl;
    cout << "4. StackLi" << endl;
    cout << "5. QueueAr" << endl;
    cout << "6. SkipList" << endl;
    
    //Get choice from stdin
    cout << "Your choice >> ";
    cin >> choice;
    return choice;
}

int main() {
    //Setup 3 variables
    CPUTimer ct;
    string filename;
    int choice;
    
    //Get filename from stdin
    cout << "Filename >> ";
    cin >> filename;
    
    //Run main loop
    do {
        choice = getChoice();
        ct.reset();
        switch(choice) {
            case 1: RunList(filename); break;
            case 2: RunCursorList(filename); break;
            case 3: RunStackAr(filename); break;
            case 4: RunStackLi(filename); break;
            case 5: RunQueueAr(filename); break;
            case 6: RunSkipList(filename); break;
        }
        cout << "CPU time: " << ct.cur_CPUTime() << endl;
    } while(choice > 0);
    return 0;
}