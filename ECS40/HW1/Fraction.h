/*
Sergei Chestakov
913914694
This is the header file for a Fraction class that can add, substract, input, and output fractions
*/
#ifndef FRACTION_H
	#define FRACTION_H

#include <iostream>
#include <cstdlib>
#include <stdexcept>
using namespace std;

class Fraction{
	friend ostream& operator<<(ostream& out, Fraction& f);
	friend istream& operator>>(istream& in, Fraction& f);
	private:
	int num, den;
	public:	
	Fraction();
	Fraction(int a, int b);
	Fraction operator+(const Fraction rhs);
	Fraction operator-(const Fraction rhs);
	Fraction& operator =(const Fraction& newFraction);
	void changeSigns(int *n, int *d);
	void reduceFraction(int &n, int &d);
	int find_gcd(int n1, int n2);
	int getNum() const;
	int getDen() const;
};

#endif