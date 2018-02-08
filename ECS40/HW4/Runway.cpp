/*
Sergei Chestakov
913914694
*/
#include "Runway.h"
#include <sstream>
using namespace std;

Runway::Runway(string s) : site_number_(s.substr(0,10)), name_(s.substr(13,7)), length_(convert_length(s.substr(20,5))){}

string Runway::site_number() const {
	return site_number_;
}

string Runway::name() const {
	return name_;
}

int Runway::length() const {
	return length_;
}

//Private

int Runway::convert_length(string s) const {
	int len;
	istringstream stream(s);
	stream >> len;
	return len;
}
