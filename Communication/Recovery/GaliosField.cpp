#include "GaloisField.h"
#include "UseException.h"
unsigned int primes[] = {
	PRIM(0), PRIM(1), PRIM(2), PRIM(3),
	PRIM(4), PRIM(5), PRIM(6), PRIM(7),
	PRIM(8), PRIM(9), PRIM(10), PRIM(11),
	PRIM(12), PRIM(13), PRIM(14), PRIM(15),
	PRIM(16), PRIM(17), PRIM(18), PRIM(19),
	PRIM(20), PRIM(21), PRIM(22), PRIM(23),
	PRIM(24), PRIM(25), PRIM(26), PRIM(27),
	PRIM(28), PRIM(29), PRIM(30), PRIM(31)
};
void GaloisField::GenerateTables() {
	unsigned int val = 1;
	powTable[0] = 1;
	logTable[1] = 0;

	for (int i = 1; i < this->characteristic; i++) {
		val <<= 1;
		if (val >= (1 << fieldPower)) {
			val ^= this->primitivePoly;
		}
		powTable[i] = (RS_WORD)val;
		logTable[(RS_WORD)val] = i;
	}

	// השלמת טבלת powTable
	for (int i = this->characteristic; i < this->characteristic * 2; i++) {
		powTable[i] = powTable[i - this->characteristic];
	}
}



void GaloisField::GenerateLogTable() {
	for (int i = 0; i <= this->characteristic; i++) {
		logTable[i] = 0;  // אפס את הערכים
	}

	for (int i = 0; i < this->characteristic; i++) {
		logTable[powTable[i]] = i;
	}
}




//GaloisField::GaloisField()
//{
//}

GaloisField::GaloisField(int fieldPower) {
	// הגדרת גודל השדה
	this->fieldPower = fieldPower;
	this->characteristic = (1 << fieldPower) - 1;

	// הגדרת פולינום פרימיטיבי
	this->primitivePoly = primes[fieldPower];

	// הקצאת זיכרון
	this->powTable = new RS_WORD[characteristic * 2];
	this->logTable = new RS_WORD[characteristic + 1];

	// יצירת טבלאות
	GenerateTables();
	GenerateLogTable(); // וודא שאתה קורא לפונקציה זו
}

GaloisField::~GaloisField()
{
	delete[] this->powTable;
	delete[] this->logTable;
}


//GaloisField::GaloisField(int fieldPower) : fieldPower(fieldPower) {
//	// הגדרת גודל השדה
//	this->characteristic = (1 << fieldPower) - 1;
//
//	// הגדרת פולינום פרימיטיבי
//	this->primitivePoly = primes[fieldPower];
//
//	// הקצאת זיכרון
//	this->powTable = new RS_WORD[characteristic * 2];
//	this->logTable = new RS_WORD[characteristic + 1];
//
//	unsigned int val = 1;
//	powTable[0] = 1;
//	logTable[1] = 0;
//
//	for (RS_WORD i = 1; i < this->characteristic; i++) {
//		val <<= 1;
//		if (val >= (1 << fieldPower)) {
//			val ^= this->primitivePoly;
//		}
//		powTable[i] = (RS_WORD)val;
//		logTable[(RS_WORD)val] = i;
//	}
//
//	// השלמת הטבלה
//	for (RS_WORD i = this->characteristic; i < this->characteristic * 2; i++) {
//		powTable[i] = powTable[i - this->characteristic];
//	}
//
//	// מילוי ערכים חסרים ב-`logTable`
//	for (RS_WORD i = 0; i <= this->characteristic; i++) {
//		if (logTable[i] == 0 && i != 0) {
//			logTable[i] = characteristic;
//		}
//	}
//}




//GaloisField::~GaloisField()
//{
//	delete[] this->powTable;
//	delete[] this->logTable;
//}
//פונקציה לביצוע הכפל ללא שימוש בטבלאות חיפוש ב-`GaloisField`
RS_WORD GaloisField::multNoLUT(RS_WORD a, RS_WORD b)
{
	RS_WORD ret = 0;
	while (b > 0)
	{
		if (b & 1) //if odd//אי זוגי
		{
			ret ^= a;
		}
		b >>= 1;
		a <<= 1;
		if (a > this->characteristic)
		{
			a ^= this->primitivePoly;
		}
	}
	return ret;
}
RS_WORD GaloisField::add(RS_WORD a, RS_WORD b)
{
	if (a >= (1 << this->fieldPower) || b >= (1 << this->fieldPower)) {
		throw std::out_of_range("Value out of range");
	}
	return a ^ b;
}
RS_WORD GaloisField::sub(RS_WORD a, RS_WORD b)
{
	if (a >= (1 << this->fieldPower) || b >= (1 << this->fieldPower)) {
		throw std::out_of_range("Value out of range");
	}
	return a ^ b;
}

RS_WORD GaloisField::mult(RS_WORD a, RS_WORD b)
{
	if (a >= (1 << this->fieldPower) || b >= (1 << this->fieldPower)) {
		throw UseException("Error: Value out of range.");
	}
	if (a == 0 || b == 0) {
		return 0;
	}

	int sumLog = this->logTable[a] + this->logTable[b];
	if (sumLog >= this->characteristic) {
		sumLog -= this->characteristic;
	}

	return this->powTable[sumLog];
}

RS_WORD GaloisField::div(RS_WORD a, RS_WORD b)
{

	if (a >= (1 << this->fieldPower) || b >= (1 << this->fieldPower)) {
		throw std::out_of_range("Value out of range");
	}
	if (b == 0) {
		throw std::domain_error("Division by zero");
	}
	return a == 0 ? 0 : this->powTable[this->logTable[a] - this->logTable[b] + this->characteristic];
}

RS_WORD GaloisField::pow(RS_WORD x, RS_WORD power)
{
	if (x >= (1 << this->fieldPower)) {
		throw std::out_of_range("Value out of range");
	}
	return this->powTable[(this->logTable[x] * power) % this->characteristic];
}

RS_WORD GaloisField::inv(RS_WORD x)
{
	if (x >= (1 << this->fieldPower)) {
		throw std::out_of_range("Value out of range");
	}
	return this->powTable[this->characteristic - this->logTable[x]];
}

RS_WORD GaloisField::sqrt(RS_WORD x)
{
	if (x >= (1 << this->fieldPower)) {
		throw std::out_of_range("Value out of range");
	}
	if (x == 0) {
		return 0;
	}

	int logX = logTable[x];

	if (logX == -1) {
		std::cerr << "Error: logTable does not contain the value " << static_cast<int>(x) << std::endl;
		return 0; // Handle error appropriately
	}

	// Check if logX is valid for the given field
	if (logX >= this->characteristic) {
		std::cerr << "Error: logX is out of bounds" << std::endl;
		return 0; // Handle error appropriately
	}

	int halfLogX = logX / 2;

	std::cout << "x: " << static_cast<int>(x) << std::endl;
	std::cout << "logTable[x]: " << logX << std::endl;
	std::cout << "halfLogX: " << halfLogX << std::endl;

	return this->powTable[halfLogX];
}

void GaloisField::printTables() {
	std::cout << "powTable: " << std::endl;
	for (int i = 0; i < this->characteristic * 2; ++i) {
		std::cout << "powTable[" << i << "] = " << static_cast<int>(powTable[i]) << std::endl;
	}

	std::cout << "logTable: " << std::endl;
	for (int i = 0; i <= this->characteristic; ++i) {
		std::cout << "logTable[" << i << "] = " << static_cast<int>(logTable[i]) << std::endl;
	}
}


