#include "Polynom.h"
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;
typedef uint8_t RS_WORD;


Polynom::Polynom()
{	
	
}

Polynom::Polynom(vector<RS_WORD>& c)
{
	coef = c;
}

Polynom::Polynom(int size, vector<RS_WORD>& coef)
{
	this->size = size;
	this->coef = coef;
}

int Polynom::getSize()
{
	return size;
}

void Polynom::setRef(vector<RS_WORD>& coef) {
	//this->size = coef.size();
	this->coef = coef; 
}

void Polynom::setCopy(vector<RS_WORD>& coef) {
	this->coef = coef;
	this->size = coef.size();
}


void Polynom::print()
{
	
		cout << "[" << setw(3) << hex << (int)coef[0];
	
	for (int i = 1; i < coef.capacity(); i++)
	{
		cout << ", " << setw(3) << hex << (int)coef[i];
	}
	if (coef.capacity() > 0)
	{
		cout << "]" << dec << endl;
	}
}

RS_WORD Polynom::GetCoefficient(int index)
{
	if (index >= 0 && index < coef.size()) {
		return coef[index];
	}
	return 0; 
}

int main2() {
	vector<RS_WORD> vec = { 11,21,31,14 };
	Polynom* p = new Polynom(vec);
	p->print();
	return 0;
}