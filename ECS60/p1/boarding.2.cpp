#include <iostream>
#include <fstream>
#include"StackAr.h"
#include"QueueAr.h"

using namespace std;

struct Seat {
    int rowNum;
    char seatLetter;
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
int NUM_ROWS = 0;
int NUM_PEOPLE = 0;

int numSeated;
ifstream input;

void printRows(StackAr<Row> &rows, int rowDepth) {
    StackAr<Row> rowsTemp(NUM_ROWS);
    for(int i=0;i<NUM_ROWS;i++) {
        Row row = rows.topAndPop();
        rowsTemp.push(row);

        if(i < rowDepth) {
            while(!row.ABC.isEmpty()) {
                cout << row.ABC.top().rowNum << row.ABC.top().seatLetter << " ";
                row.ABC.pop();
            }
            bool printedLet = false;
            if(row.currentlySeating != EMPTY_SEAT) {
                cout << "s ";
                printedLet = true;
            }
            if(!rows.isEmpty() && rows.top().prevRowSeat != EMPTY_SEAT) {
                cout << "p ";
            } else {
                if(!printedLet) {
                    cout << "e ";
                }
            }
            
            while(!row.DEF.isEmpty()) {
                cout << row.DEF.top().rowNum << row.DEF.top().seatLetter << " ";
                row.DEF.pop();
            }
            cout << " waiting " << row.prevRowSeat.rowNum << row.prevRowSeat.seatLetter << " st " << row.state << endl;
        }
    }
    while(!rowsTemp.isEmpty()) {
        rows.push(rowsTemp.topAndPop());
    }
}

void moveUp(StackAr<Row> &rows, Row &row, int rowNum) {
    if(row.prevRowSeat != EMPTY_SEAT) { //if someone is waiting to move into this row
        if(row.prevRowSeat.rowNum != rowNum && !rows.isEmpty()) { //if person does not need to sit on this row
            //Move the person up a row
            Row lastRow = rows.topAndPop();
            if(row.state == NONE && lastRow.prevRowSeat == EMPTY_SEAT) {
                lastRow.prevRowSeat = row.prevRowSeat;
                row.prevRowSeat = EMPTY_SEAT;
            }
            rows.push(lastRow);
        } else { //the person does want to sit down on this row
            if(row.state == NONE && ((!rows.isEmpty() && rows.top().prevRowSeat == EMPTY_SEAT) || (rows.isEmpty()))) {
                row.currentlySeating = row.prevRowSeat;
                row.prevRowSeat = EMPTY_SEAT;
                row.state = STORE_LUGGAGE1;
            }
        }
    }
}

void luggageCalc(StackAr<Row> &rows, Row &row, int rowNum) {
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
}

void updateRow(StackAr<Row> &rows, Row &row, Queue<Seat> &passengers, int rowNum) { //Look at 105/110 - 110 should occur at 105
    switch(row.state) {
        case NONE: {
            moveUp(rows, row, rowNum);
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
            if(row.temp.isEmpty()) {
                numSeated++;
                row.state = NONE;
                //Move the next person up simultaneously
                if(rowNum == 1 && !passengers.isEmpty() && row.prevRowSeat == EMPTY_SEAT) { //should be in 105
                    row.prevRowSeat = passengers.dequeue(); //does not happen?
                } else {
                    moveUp(rows, row, rowNum);
                }
            }
            break;
        }
        case AC_OUT: {
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
                //Move the next person up simultaneously 
                if(rowNum == 1 && !passengers.isEmpty() && row.prevRowSeat == EMPTY_SEAT) {
                    row.prevRowSeat = passengers.dequeue();
                } else {
                    moveUp(rows, row, rowNum);
                }
            }
            break;
        }
        case DF_OUT: {
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
            luggageCalc(rows, row, rowNum);
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
            updateRow(rows, row, passengers, NUM_ROWS-i);
            //Dequeue passengers and place in first row if it is empty to continue boarding
            if(i == NUM_ROWS-1){
                //Move a new passenger into the before first row areas if empty
                if(row.prevRowSeat == EMPTY_SEAT && !passengers.isEmpty()) {
                    row.prevRowSeat = passengers.dequeue();
                }
            }
            //Push row back as we are done with it
            rows.push(row);
        }
        timeElapsed += MOVE_TIME;
        /*if(timeElapsed <= 275) {
            printRows(rows, 3);
            cout << timeElapsed << endl << endl;
        }*/
    }
    //printRows(rows);
    return timeElapsed;
}

//Read one line of passenger data
Queue<Seat> readPassengers() {
    NUM_PEOPLE = 288;
    Queue<Seat> passengers(NUM_PEOPLE);
    for(int i=0;i<NUM_PEOPLE;i++) {
        Seat seat;
        input >> seat.rowNum >> seat.seatLetter;
        if(seat.rowNum > NUM_ROWS) {
            NUM_ROWS = seat.rowNum;
        }
        passengers.enqueue(seat);
    }
    return passengers;
}

int main(int argc, char *argv[]) {
    string filename = argv[1];
    input.open(filename.c_str());
    Queue<Seat> passengers = readPassengers();
    cout << "Back to front: " << simulateBoarding(passengers) << endl;
    passengers = readPassengers();
    cout << "Random: " << simulateBoarding(passengers) << endl;
    passengers = readPassengers();
    cout << "Outside in: " << simulateBoarding(passengers) << endl;
    input.close();
    return 0;
}