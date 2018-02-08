/*
Sergei Chestakov
913914694
*/
#ifndef SITENUMBER_H
#define SITENUMBER_H
#include "Runway.h"
using namespace std;

class SiteNumber{
	private:
	string siteNum;
	public:
	SiteNumber(string s){
		siteNum = s;	
	}
	bool operator() (Runway* r){
		return r->site_number() == siteNum;
	}
};	
#endif
