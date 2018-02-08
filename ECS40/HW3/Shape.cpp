/*
Sergei Chestakov
913914694
This is the implementation of an abstract shape class and several specific shape objects that can move around and have a specific name and size
*/
#include "Shape.h"
#include <iostream>
#include <stdexcept>
#include <ctype.h>
using namespace std;

Shape::~Shape(void){
	delete x;
	delete y;
}

void Shape::print(void) const{
	cout << name() << " at";
	for (int i = 0; i < size(); i++){
		cout << "(" << x[i] << "," << y[i] << ") ";
	}
}
void Shape::move (int dx, int dy){
	for(int i = 0; i < size(); i++){
		x[i] += dx;
		y[i] += dy;
	}
}
bool Shape::overlap(const Shape &t) const{
	for(int i = 0; i < size(); i++){
		for(int j = 0; j <t.size(); j++){
			if(x[i] == t.x[j]){
				if(y[i] == t.y[j]){
					return true;
				}
			}
		}
	}
	return false;	
}
int Shape::getX(int i) const{
	return x[i];
}
int Shape::getY(int i) const{
	return y[i];
}
Shape* Shape::makeShape(char ch,int posx,int posy){
	switch(toupper(ch)){
		case 'O':
			return new O(posx, posy);
		case 'I':
			return new I(posx, posy);
		case 'L':
			return new L(posx, posy);
		case 'S':
			return new S(posx, posy);
		case 'X':
			return new X(posx, posy);
		case 'U':
			return new U(posx, posy);
		default:
			break;
	}
	throw invalid_argument("invalid type");
}
void Shape::draw(void) const{
	for(int i = 0; i < size(); i++){
		int xc = x[i] * 40;
		int yc = y[i] * 40;
		cout << "<rect fill=\"" << color() << "\" stroke=\"black\" x=\"" << xc << "\" y=\"" << yc << "\" width=\"40\" height=\"40\"/>" << endl;
	}
}

O::O(int posx, int posy){
	x = new int[1];
	y = new int[1];
	x[0] = posx;
	y[0] = posy;
}
char O::name(void) const{
	return 'O';
}
int O::size(void) const{
	return 1;
}
const char* O::color(void) const{
	return "cyan";
}

I::I(int posx, int posy){
	x = new int[2];
	y = new int[2];
	x[0] = posx;
	y[0] = posy;
	x[1] = posx;
	y[1] = posy + 1;
}
char I::name(void) const{
	return 'I';
}
int I::size(void) const{
	return 2;
}
const char* I::color(void) const{
	return "yellow";
}


L::L(int posx, int posy){
	x = new int[3];
	y = new int[3];
	x[0] = posx;
	y[0] = posy;
	x[1] = posx + 1;
	y[1] = posy;
	x[2] = posx;
	y[2] = posy + 1;
}
char L::name(void) const{
	return 'L';
}
int L::size(void) const{
	return 3;
}
const char* L::color(void) const{
	return "purple";
}


S::S(int posx, int posy){
	x = new int[4];
	y = new int[4];
	x[0] = posx;
	y[0] = posy;
	x[1] = posx + 1;
	y[1] = posy;
	x[2] = posx + 1;
	y[2] = posy + 1;
	x[3] = posx + 2;
	y[3] = posy + 1;
}
char S::name(void) const{
	return 'S';
}
int S::size(void) const{
	return 4;
}
const char* S::color(void) const{
	return "blue";
}


X::X(int posx, int posy){
	x = new int[5];
	y = new int[5];
	x[0] = posx;
	y[0] = posy;
	x[1] = posx - 1;
	y[1] = posy + 1;
	x[2] = posx;
	y[2] = posy + 1;
	x[3] = posx + 1;
	y[3] = posy + 1;
	x[4] = posx;
	y[4] = posy + 2;
}
char X::name(void) const{
	return 'X';
}
int X::size(void) const{
	return 5;
}
const char* X::color(void) const{
	return "orange";
}


U::U(int posx, int posy){
	x = new int[7];
	y = new int[7];
	x[0] = posx;
	y[0] = posy;
	x[1] = posx + 1;
	y[1] = posy;
	x[2] = posx + 2;
	y[2] = posy;
	x[3] = posx;
	y[3] = posy + 1;
	x[4] = posx + 2;
	y[4] = posy + 1;
	x[5] = posx;
	y[5] = posy + 2;
	x[6] = posx + 2;
	y[6] = posy + 2;
}
char U::name(void) const{
	return 'U';
}
int U::size(void) const{
	return 7;
}
const char* U::color(void) const{
	return "red";
}
