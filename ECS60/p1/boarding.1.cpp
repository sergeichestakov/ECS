#include <iostream>
#include <fstream>
#include"StackAr.h"
#include"QueueAr.h"

using namespace std;

struct Seat {
    int rowNum;
    char seatLetter;
    /*Seat& operator=(const Seat &rhs) {
        this->rowNum = rhs.rowNum;
        this->seatLetter = rhs.seatLetter;
        return *this;
    }*/
    bool operator==(const Seat &rhs) const {
        return (this->rowNum == rhs.rowNum && this->seatLetter == rhs.seatLetter);
    }
    bool operator!=(const Seat &rhs) const {
        return (this->rowNum != rhs.rowNum || this->seatLetter != rhs.seatLetter);
    }
};

enum State {NONE, AC_IN, AC_OUT, DF_IN, DF_OUT, STORE_LUGGAGE1, STORE_LUGGAGE2};

const Seat EMPTY_SEAT = {-1, '?'};

class Row {
    public:
        State state;
        Seat currentlySeating;
        Seat prevRowSeat;
        StackAr<Seat> ABC;
        StackAr<Seat> DEF;
        StackAr<Seat> temp;
        Row() : ABC(3), DEF(3), temp(2) {
            state = NONE;
            currentlySeating = EMPTY_SEAT;
            prevRowSeat = EMPTY_SEAT;
        }
};

const int MOVE_TIME = 5;
const int NUM_ROWS = 48;
const int NUM_PEOPLE = 288;

int numSeated;
ifstream input;

void printRows(StackAr<Row> &rows, int rowDepth) {
    StackAr<Row> rowsTemp(NUM_ROWS);
    for(int i=0;i<NUM_ROWS;i++) {
        Row row = rows.top();
        rowsTemp.push(rows.topAndPop());

        if(i < rowDepth) {
            while(!row.ABC.isEmpty()) {
                cout << row.ABC.top().rowNum << row.ABC.top().seatLetter << " ";
                row.ABC.pop();
            }
            
            if(!rows.isEmpty() && row.currentlySeating != EMPTY_SEAT) {
                cout << "s ";
            }
            if(!rows.isEmpty() && rows.top().prevRowSeat != EMPTY_SEAT) {
                cout << "o ";
            } else {
                cout << "x ";
            }
            
            while(!row.DEF.isEmpty()) {
                cout << row.DEF.top().rowNum << row.DEF.top().seatLetter << " ";
                row.DEF.pop();
            }
            cout << endl;
        }
    }
    while(!rowsTemp.isEmpty()) {
        rows.push(rowsTemp.topAndPop());
    }
}

void updateRow(StackAr<Row> &rows, Row &row, int rowNum) {
    switch(row.state) {
        case NONE: {
            if(row.prevRowSeat != EMPTY_SEAT) { //if someone is waiting to move into this row
                if(row.prevRowSeat.rowNum != rowNum && !rows.isEmpty()) { //if person does not need to sit on this row
                    Row lastRow = rows.topAndPop();
                    //Move the person up a row
                    if(lastRow.state == NONE && lastRow.prevRowSeat == EMPTY_SEAT) {
                        lastRow.prevRowSeat = row.prevRowSeat;
                        row.prevRowSeat = EMPTY_SEAT;
                    }
                    rows.push(lastRow);
                } else { //the person does want to sit down on this row
                    row.currentlySeating = row.prevRowSeat;
                    row.prevRowSeat = EMPTY_SEAT;
                    row.state = STORE_LUGGAGE1;
                }
            }
            break;
        }
        case AC_IN: {
            if(row.currentlySeating != EMPTY_SEAT) { //Someone new is sitting down
                //They sit down in row
                row.ABC.push(row.currentlySeating);
                row.currentlySeating = EMPTY_SEAT;
            } else {
                //People who made way sit down
                row.ABC.push(row.temp.topAndPop());
            }
            //cout << "AI3";
            if(row.temp.isEmpty()) {
                numSeated++;
                row.state = NONE;
            }
            //cout << numSeated << " ";
            break;
        }
        case AC_OUT: {
            //cout << "hia";
            row.temp.push(row.ABC.topAndPop());
            if(row.ABC.isEmpty() || row.ABC.top().seatLetter < row.currentlySeating.seatLetter) {
                row.state = AC_IN;
            }
            break;
        }
        case DF_IN: {
            if(row.currentlySeating != EMPTY_SEAT) {
                row.DEF.push(row.currentlySeating);
                row.currentlySeating = EMPTY_SEAT;
            } else {
                row.DEF.push(row.temp.topAndPop());
            }
            if(row.temp.isEmpty()) {
                numSeated++;
                row.state = NONE;
            }
            //cout << numSeated << " ";
            break;
        }
        case DF_OUT: {
            //cout << "hid";
            row.temp.push(row.DEF.topAndPop());
            if(row.DEF.isEmpty() || row.DEF.top().seatLetter > row.currentlySeating.seatLetter) {
                row.state = DF_IN;
            }
            break;
        }
        case STORE_LUGGAGE1: {
            row.state = STORE_LUGGAGE2;
            break;
        }
        case STORE_LUGGAGE2: {
            if(row.currentlySeating.seatLetter == 'A' || row.currentlySeating.seatLetter == 'B' || row.currentlySeating.seatLetter == 'C') {
                if(row.currentlySeating.seatLetter == 'C') {
                    row.state = AC_IN;
                } else {
                    if(row.currentlySeating.seatLetter == 'B') {
                        if(row.ABC.isEmpty() || row.ABC.top().seatLetter != 'C') {
                            row.state = AC_IN;
                        } else {
                            row.state = AC_OUT;
                        }
                    } else { //row.ABC.top.seatLetter != 'A'
                        if(row.ABC.isEmpty()) {
                            row.state = AC_IN;
                        } else {
                            row.state = AC_OUT;
                        }
                    }
                }
            } else {
                if(row.currentlySeating.seatLetter == 'D') {
                    row.state = DF_IN;
                } else {
                    if(row.currentlySeating.seatLetter == 'E') {
                        if(row.DEF.isEmpty() || row.DEF.top().seatLetter != 'D') {
                            row.state = DF_IN;
                        } else {
                            row.state = DF_OUT;
                        }
                    } else { //row.ABC.top.seatLetter != 'F'
                        if(row.DEF.isEmpty()) {
                            row.state = DF_IN;
                        } else {
                            row.state = DF_OUT;
                        }
                    }
                }
            }
            break;
        } 
        default:;
    }
}

//Simulate boarding with queue of passenger data
int simulateBoarding(Queue<Seat> passengers) {
    //Setup
    int timeElapsed = 0;
    numSeated = 0;
    //Initialize stack of rows
    StackAr<Row> rows(NUM_ROWS);
    StackAr<Row> rowsTemp(NUM_ROWS);
    for(int i=0;i<NUM_ROWS;i++){
        Row row;
        rows.push(row);
    }
    Row firstRow = rows.topAndPop();
    firstRow.prevRowSeat = passengers.dequeue();
    rows.push(firstRow);
    //Seat passengers in order
    while(numSeated != NUM_PEOPLE) {
        for(int i=0;i<NUM_ROWS;i++) {
            rowsTemp.push(rows.topAndPop());
        }
        for(int i=0;i<NUM_ROWS;i++) {
            Row row = rowsTemp.topAndPop();
            updateRow(rows, row, NUM_ROWS-i);
            //Dequeue passengers and place in first row if it is empty to continue boarding
            if(i == NUM_ROWS-1){
                /*if(row.currentlySeating.seatLetter == 'A') {
                    cout << timeElapsed << endl;
                }*/
                if(row.state == NONE && row.prevRowSeat == EMPTY_SEAT && !passengers.isEmpty()) {
                    row.prevRowSeat = passengers.dequeue();
                }
            }
            //Push row back as we are done with it
            rows.push(row);
        }
        timeElapsed += MOVE_TIME;
        if(numSeated <= 1) {
            printRows(rows, 3);
            cout << endl;
        }
    }
    //printRows(rows);
    return timeElapsed;
}

//Read one line of passenger data
Queue<Seat> readPassengers() {
    Queue<Seat> passengers(NUM_PEOPLE);
    for(int i=0;i<NUM_PEOPLE;i++) {
        Seat seat;
        input >> seat.rowNum >> seat.seatLetter;
        passengers.enqueue(seat);
    }
    return passengers;
}

int main(int argc, char *argv[]) {
    string filename = argv[1];
    input.open(filename.c_str());
    Queue<Seat> passengers = readPassengers();
    cout << "Back to front: " << simulateBoarding(passengers) << endl;
    /*passengers = readPassengers();
    cout << "Random: " << simulateBoarding(passengers) << endl;
    passengers = readPassengers();
    cout << "Outside in: " << simulateBoarding(passengers) << endl;*/
    input.close();
    return 0;
}