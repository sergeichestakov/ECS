/*
Sergei Chestakov
913914694
This class defines the Domain
*/
#include "Domain.h"
#include <iostream>
using namespace std;

Domain::Domain(int sx, int sy) : size_x(sx), 	size_y(sy) {}

void Domain::addShape(char type, int x, int y){
	Shape* newShape = Shape::makeShape(type, x, y);
	if(fits(*newShape)){
		for(size_t i = 0; i < sList.size(); i++){
			if(sList[i]->overlap(*newShape)){
				delete newShape;
				return;
			}
		}
		sList.push_back(newShape);
	} else {
		delete newShape;
	}
}

bool Domain::fits(const Shape &s) const{
	for(int i = 0; i < s.size(); i++){
		if(s.getX(i) < 0 || s.getX(i) >= size_x || s.getY(i) < 0 || s.getY(i) >= size_y){
			return false;
		}	
	}
	return true;
}

void Domain::draw(void) const{
	cout << "<?xml version=\"1.0\" encoding=\"utf-8\"  standalone=\"no\"?>" << endl;
	cout << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"" << endl;
	cout << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl;
	cout << "<svg width=\"670\" height=\"670\" viewBox=\"0 0 650 650\"" << endl;
	cout << "xmlns=\"http://www.w3.org/2000/svg\"" << endl;
	cout << "xmlns:xlink=\"http://www.w3.org/1999/xlink\" >" << endl;
	cout << "<g transform=\"matrix(1,0,0,-1,50,650)\">" << endl;
	int w = size_x * 40;
	int h = size_y * 40;
	cout << "<rect fill=\"white\" stroke=\"black\" x=\"0\" y=\"0\"  width=\"" << w << "\" height=\"" << h << "\"/>" << endl;
	for (size_t shape = 0; shape < sList.size(); shape++){
		sList.at(shape)->draw();
	}
	cout << "</g>" << endl;
	cout << "</svg>" << endl;
}

bool Domain::full(void) const{
	int total = 0;
	for(size_t i = 0; i < sList.size(); i++){
		total += sList[i]->size();
	}
	return total == (size_x * size_y);
}
