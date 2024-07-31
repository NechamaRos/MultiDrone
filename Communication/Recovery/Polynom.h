#pragma once
#include<vector>
using namespace std;
typedef uint8_t RS_WORD;
class Polynom
{
	int size;
public:
	vector<RS_WORD> coef;
	Polynom();
	Polynom(vector<RS_WORD>& coef);
	Polynom(int size, vector<RS_WORD>& coef);
	int getSize();
	void setCopy(vector<RS_WORD>& coef);
	void setRef(vector<RS_WORD>& coef);
	void print();
	RS_WORD GetCoefficient(int index);
};

