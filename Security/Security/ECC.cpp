#include "ECC.h"
#include"Logger.h"
#include "Utils.h"
using namespace std;

ECC::ECC(const EllipticCurve& curve)
{
	this->curve = curve;
}



 