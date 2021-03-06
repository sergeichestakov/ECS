/*
Sergei Chestakov
913914694
*/
#include "Facility.h"
#include "gcdistance.h"
#include <sstream>

using namespace std;

Facility::Facility(string s) : site_number_(s.substr(0,10)), type_(s.substr(11, 13)), code_(s.substr(24, 4)),
	name_(s.substr(130,50)), latitude_(convert_latitude(s.substr(535, 12))), longitude_(convert_longitude(s.substr(562, 12))){}

string Facility::site_number() const{
	return site_number_;
}

string Facility::type() const{
	return type_;
}

string Facility::code() const{
	return code_;
}

string Facility::name() const{
	return name_;
}

double Facility::latitude() const{
	return latitude_;
}

double Facility::longitude() const{
	return longitude_;
}

double Facility::distance(double lat, double lon) const{
	return gcdistance(latitude_, longitude_, lat, lon);
}

//Private

double Facility::convert_latitude(string s) const{
	double lat;
	istringstream stream(s.substr(0,11));
	stream >> lat;
	lat /= 3600;
	if(s.at(11) == 'S'){
		lat = -lat;
	}
	return lat;
}

double Facility::convert_longitude(string s) const{
	double lng;
	istringstream stream(s.substr(0,11));
	stream >> lng;
	lng /= 3600;
	if(s.at(11) == 'W'){
		lng = -lng;
	}
	return lng;
}
