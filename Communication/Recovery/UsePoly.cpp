#include "UsePoly.h"
#include "Polynom.h"
#include "UseException.h"


UsePoly::UsePoly()
{
}

vector<RS_WORD> UsePoly::Poly_Create(vector<RS_WORD>& coef)
{
	Polynom poly;
	poly.setCopy(coef);  // העתקת הערכים ל-Polynom

	std::vector<RS_WORD> result(poly.getSize());  // יצירת וקטור בגודל המתאים

	for (size_t i = 0; i < poly.getSize(); ++i) {
		result[i] = poly.GetCoefficient(i);  // מילוי הוקטור בערכים
	}

	return result;
}


void UsePoly::Poly_Free(vector<RS_WORD>& coef)
{	
	coef.clear();
}

//void UsePoly::Poly_Add(vector<RS_WORD>& out, vector<RS_WORD>& a, vector<RS_WORD>& b)
//{
//	/*if (a.empty() || b.empty()) {
//		throw invalid_argument("One or more input polynomials are empty.");
//	}*/
//	if (a.empty() || b.empty()) {
//		out.clear(); 
//		return;
//	}
//
//	int sizeA = a.size();
//	int sizeB = b.size();
//	int n = std::max(sizeA, sizeB);
//
//	out.resize(n, 0); 
//
//	for (int i = 0; i < sizeA; ++i) {
//		out[n - sizeA + i] ^= a[i];
//	}
//
//	for (int i = 0; i < sizeB; ++i) {
//		out[n - sizeB + i] ^= b[i];
//	}
//	
//}

void UsePoly::Poly_Add(vector<RS_WORD>& out,  vector<RS_WORD>& a,  vector<RS_WORD>& b)
{
	if (a.empty() && b.empty()) {
		out.clear();
		return;
	}

	int sizeA = a.size();
	int sizeB = b.size();
	int n = std::max(sizeA, sizeB);

	// התאם את גודל הוקטור
	out.resize(n, 0);

	// הוסף את הערכים מהפולינום הראשון
	for (int i = 0; i < sizeA; ++i) {
		out[n - sizeA + i] ^= a[i];
	}

	// הוסף את הערכים מהפולינום השני
	for (int i = 0; i < sizeB; ++i) {
		out[n - sizeB + i] ^= b[i];
	}
}


void UsePoly::Poly_Scale(vector<RS_WORD>& out, vector<RS_WORD>& in, RS_WORD scale, GaloisField* gf)
{	
	if (gf == nullptr) {
		throw invalid_argument("GaloisField pointer is null.");
	}

	if (in.empty()) {
		out.clear();
		return;
	}

	if (out.size() != in.size()) {
		out.resize(in.size());
	}

	if (scale == 0) {
		std::fill(out.begin(), out.end(), 0); 
	}
	else
	{
		for (size_t i = 0; i < in.size(); i++)
		{
			out[i] = gf->mult(in[i], scale);
		}
	}
}

void UsePoly::Poly_Mult(vector<RS_WORD>& out, vector<RS_WORD>& a, vector<RS_WORD>& b, GaloisField* gf)
{
	int n = a.size() + b.size() - 1;
	out.resize(n, 0);
	for (int i = 0; i < a.size(); i++)
	{
		for (int j = 0; j < b.size(); j++)
		{
			out[i + j] ^= gf->mult(a[i], b[j]);
		}
	}
}

void UsePoly::Poly_Div(vector<RS_WORD>& quotient, vector<RS_WORD>& remainder, vector<RS_WORD>& a, vector<RS_WORD>& b, GaloisField* gf)
{
	if (a.empty() || b.empty() || b[0] == 0) {
		throw UseException("Error: Invalid input to Poly_Div.");
	}

	int degree_a = a.size() - 1;
	int degree_b = b.size() - 1;

	quotient.resize(degree_a - degree_b + 1, 0);

	// Adjust remainder size to avoid out-of-bounds access
	remainder.resize(degree_a + 1 - degree_b);  // new size calculation

	// Copy a to remainder (avoid unnecessary assignment)
	std::copy(a.begin(), a.end(), remainder.begin());

	RS_WORD normalizer = b[0];

	for (int i = 0; i <= degree_a - degree_b; i++) {
		RS_WORD coef = gf->div(remainder[i], normalizer);
		quotient[i] = coef;

		if (coef != 0) {
			for (int j = 0; j <= degree_b; j++) {
				int target_index = i + j;
				if (target_index < remainder.size()) {  // Check within bounds
					remainder[target_index] ^= gf->mult(b[j], coef);
				}
			}
		}

		// Set unused elements of remainder to zero after each iteration
		for (int j = i + degree_b + 1; j < remainder.size(); j++) {  // new loop condition
			remainder[j] = 0;
		}
	}

	// Remove leading zeros from remainder (optional)
	while (remainder.size() > 1 && remainder.back() == 0) {
		remainder.pop_back();
	}
}






RS_WORD UsePoly::Poly_Eval(vector<RS_WORD>& poly, RS_WORD x, GaloisField* gf)
{
	RS_WORD result = 0;
	for (int i = poly.size() - 1; i >= 0; --i) {
		result = gf->mult(result, x) ^ poly[i];
	}
	return result;
}


void UsePoly::Poly_ChienSearch(vector<uint8_t>* out, vector<RS_WORD>& poly, int max, GaloisField* gf)
{
	RS_WORD* temp = new RS_WORD[poly.size()];
	if (temp == nullptr) {
		throw UseException("Error: Memory allocation failed.");
	}
	memcpy(temp, poly.data(), sizeof(RS_WORD) * poly.size());
	for (int i = 0; i < max; i++)
	{
		RS_WORD sum = 0;
		for (int j = 0; j < poly.size(); j++)
		{
			sum ^= temp[j];
			temp[j] = gf->mult(temp[j], gf->powTable[poly.size() - j - 1]);
		}
		if (!sum)
		{
			out->push_back(i);
		}
	}
	delete[] temp;
}

void UsePoly::Poly_Pad(vector<RS_WORD>& poly, int left, int right)
{
	int n = poly.size() + left + right;
	vector<RS_WORD> temp(n, 0); 
	memcpy(temp.data() + left, poly.data(), sizeof(RS_WORD) * poly.size());
	poly = std::move(temp);
}


void UsePoly::Poly_Trim(vector<RS_WORD>& poly, int left, int right)
{
	if (left + right >= poly.size()) {
		throw UseException("Error: Left and right trim sizes are too large.");
	}
	int n = poly.size() - left - right;
	vector<RS_WORD> temp(n);
	memcpy(temp.data(), poly.data() + left, sizeof(RS_WORD) * n);
	poly = std::move(temp);
}


void UsePoly::Poly_Append(vector<RS_WORD>& out, vector<RS_WORD>& a, vector<RS_WORD>& b)
{
	int n = a.size() + b.size();
	out.resize(n); 
	memcpy(out.data(), a.data(), sizeof(RS_WORD) * a.size());
	memcpy(out.data() + a.size(), b.data(), sizeof(RS_WORD) * b.size());
}


void UsePoly::Poly_Reverse(vector<RS_WORD>& out, vector<RS_WORD>& in)
{
	out.resize(in.size()); 
	for (int i = 0; i < in.size(); i++)
	{
		out[i] = in[in.size() - i - 1];
	}
}


PolyError UsePoly::getLastError()
{
	return lastError;
}


