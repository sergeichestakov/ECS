#include "MyFloat.h"

MyFloat::MyFloat(){
  sign = 0;
  exponent = 0;
  mantissa = 0;
}

MyFloat::MyFloat(float f){
  unpackFloat(f);
}

MyFloat::MyFloat(const MyFloat & rhs){
	sign = rhs.sign;
	exponent = rhs.exponent;
	mantissa = rhs.mantissa;
}

ostream& operator<<(std::ostream &strm, const MyFloat &f){
	strm << f.packFloat();
	return strm;

}

bool MyFloat::operator==(const float rhs) const{
	return packFloat() == rhs;
}

MyFloat MyFloat::operator+(const MyFloat& rhs) const{
	MyFloat sum;
	int diff;
	MyFloat smaller = rhs;
	MyFloat larger = *this;	

	if(exponent > rhs.exponent){ //Figure out which ones smaller and what the difference is
		diff = exponent - rhs.exponent;
	} else {
		diff = rhs.exponent - exponent;
		smaller = *this;
		larger = rhs;
	}

	smaller.mantissa ^= (1 << 23); //Restore first bit
	larger.mantissa ^= (1 << 23);

	sum.exponent = larger.exponent; //Sum is assumed to have larger exponent and sign
	sum.sign = larger.sign;

	smaller.mantissa >>= diff; //Shift smaller mantissa over to match the larger

	if(smaller.sign == larger.sign){ //Add or substract them depending on the signs
		sum.mantissa = smaller.mantissa + larger.mantissa;
	} else {
		sum.mantissa = larger.mantissa - smaller.mantissa;
		if(smaller.mantissa & 1) //Borrow
		sum.mantissa--;
	}
	
	if(sum.mantissa & (1 << 24)){ //25th bit is a 1!
		sum.mantissa >>= 1;
		sum.exponent++;
	}

	while(!(sum.mantissa & (1 << 23))){ //Normalize
		sum.mantissa <<= 1;
		if(sum.exponent == 127){ //Number is a zero
			sum.exponent = 0;
			sum.sign = 0;
			break;
		}
		sum.exponent--;
	}

	if(sum.mantissa & (1 << 23))
	sum.mantissa ^= (1 << 23); //Flip bit

	return sum;
}

MyFloat MyFloat::operator-(const MyFloat& rhs) const{
	MyFloat right = rhs;
	right.sign = !right.sign;
	return *this + right;
}



void MyFloat::unpackFloat(float f) { 
	__asm__(
	"movl %[f], %[sign];"
	"shrl $31, %[sign];"
	"movl %[f], %[exponent];"
	"shrl $23, %[exponent];"
	"andl $0xFF, %[exponent];"
	"movl %[f], %[mantissa];"
	"andl $0x7FFFFF, %[mantissa];"
	: 
	[sign] "=&r" (sign), [exponent] "=&r" (exponent), [mantissa] "=r" (mantissa)
	:
	[f] "r" (f)
	:
	"cc"
	);
}//unpackFloat

float MyFloat::packFloat() const{
  //returns the floating point number represented by this
  float f = 0;
	__asm__(
	"movl %[sign], %[f];"
	"shll $31, %[f];"
	"shll $23, %[exponent];"
	"orl %[exponent], %[f];"
	"orl %[mantissa], %[f];"
	:
	[f] "=r" (f)
	:
	[sign] "r" (sign), [exponent] "r" (exponent), [mantissa] "r" (mantissa)
	:
	"cc"
	);
  return f;
}//packFloat
//



