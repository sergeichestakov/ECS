/*
Sergei Chestakov
913914694
This is the implementation of a Fraction class that can add, substract, input, and output fractions
*/
#include "Fraction.h"

Fraction::Fraction(){
	num = 0;
	den = 1;
}

Fraction::Fraction(int a, int b){
	if(b == 0){
		throw invalid_argument("denominator is zero");
	}
	reduceFraction(a, b);
	changeSigns(&a, &b);
	num = a;
	den = b;
}

Fraction Fraction::operator+(const Fraction rhs){
	int denom, numer;
	denom = den * rhs.den;
	numer = (num * rhs.den) + (den * rhs.num);
	return Fraction(numer, denom);
}

Fraction Fraction::operator-(const Fraction rhs){
	int denom, numer;
	denom = den * rhs.den;
	numer = (num * rhs.den) - (den * rhs.num);
	return Fraction(numer, denom);
}

void Fraction::changeSigns(int *n, int *d){
	int sign;
	if(*n * *d >= 0){
		sign = 1;
	} else {
		sign = -1;
	}
	*n = sign * abs(*n);
	*d = abs(*d);
}

void Fraction::reduceFraction(int &n, int &d){
	int gcd = find_gcd(n, d);
	n = n/gcd;
	d = d/gcd;
}

istream &operator>>(istream &in, Fraction &f){
	int n, d;
	char slash;
	in >> n >> slash >> d;
	Fraction newFraction(n, d);
	f = newFraction;
	return in;
}

ostream &operator<<(ostream &out, Fraction &f){
	if(f.den != 1){
		out << f.num << "/" << f.den << endl;
	} else {
		out << f.num << endl;
	}
	return out; 
}

Fraction& Fraction::operator =(const Fraction& newFraction){
	if(&newFraction == this){
		return *this;
	}	
	this->num = newFraction.num;
	this->den = newFraction.den;
	return *this;
}

int Fraction::find_gcd(int n1, int n2){
	int r;
	r = n1 % n2;
	while(r != 0){
		n1 = n2;
		n2 = r;
		r = n1 % n2;
	}
	return n2;
}

int Fraction::getNum() const{
	return num;
}	

int Fraction::getDen() const{
	return den;
}
