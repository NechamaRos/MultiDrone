#include "doctest.h"
#include "Polynom.h"
#include "UsePoly.h"
#include "GaloisField.h"
class ReedSolomon
{
public:
	GaloisField gf;
	ReedSolomon(int fieldPower);
};