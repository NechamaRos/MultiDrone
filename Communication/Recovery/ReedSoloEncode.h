#pragma once
#include "GaloisField.h"
#include "Polynom.h"
#include "UsePoly.h"

#include "ReedSolomon.h"
class ReedSoloEncode :public ReedSolomon
{
	
public:
	ReedSoloEncode(int fieldPower);
	
	//c'tor of this class 
	//(it init the GaloisField - which we will code according to).
	//ReedSoloEncode(int fieldPower):ReedSolomon(fieldPower);
	//initialize the place to the Metadata
	 void createGenerator(Polynom* out, int nsym);
	//This is a main function.to encode this funct
	 void encode(vector<RS_WORD> out, vector<RS_WORD> data, int nsym);
};
