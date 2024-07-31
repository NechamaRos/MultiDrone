#pragma once
#include <vector>
#include "Polynom.h"
#include "GaloisField.h"
using namespace std;

enum PolyError {
	POLY_SUCCESS,           // No error
	POLY_NULL_POINTER,      // Null pointer encountered
	POLY_ALLOCATION_FAILED, // Memory allocation failed
	POLY_OTHER_ERROR        // Other specific error (add more as needed)
};

class UsePoly
{
private:

	PolyError lastError;

public:
	UsePoly();
	//יצירת אובייקט פולינום חדש.
	//n: גודל הפולינום (כמות המקדמים). coef: מערך המקדמים של הפולינום.
	vector<RS_WORD> Poly_Create(vector<RS_WORD>& coef);

	//שחרור זיכרון של אובייקט פולינום.
	//poly: אובייקט פולינום לשחרור.
	void Poly_Free(vector<RS_WORD>& coef);

	// חיבור שני פולינומים.
	//out: פולינום שיקבל את התוצאה, a, b: הפולינומים לחיבור.
	//: יש לשנות את הפונקציה כך שתתמוך בפעולת חיבור במערך תלת - מימדי, לפי הממדיות המתאימות(width, height, depth).
	void Poly_Add(vector<RS_WORD>& out, vector<RS_WORD>& a, vector<RS_WORD>& b);

	//כפל פולינום בסקאלר.
	//out: פולינום שיקבל את התוצאה, in: הפולינום לכפל, scale: הסקאלר, gf: שדה גלואי בו מבוצע הכפל.
	void Poly_Scale(vector<RS_WORD>& out, vector<RS_WORD>& in, RS_WORD scale, GaloisField* gf);

	//כפל שני פולינומים.
	//out: פולינום שיקבל את התוצאה, a, b: הפולינומים לכפל, gf: שדה גלואי בו מבוצע הכפל.
	void Poly_Mult(vector<RS_WORD>& out, vector<RS_WORD>& a, vector<RS_WORD>& b, GaloisField* gf);

	//חלוקת פולינומים.
	//result: התוצאה של החלוקה, quotient, remainder: המנת והשארית של החלוקה, a, b: הפולינומים לחלוקה, gf: שדה גלואי בו מבוצעת החלוקה.
	void Poly_Div(vector<RS_WORD>& quotient, vector<RS_WORD>& remainder, vector<RS_WORD>& a, vector<RS_WORD>& b, GaloisField* gf);

	//חישוב הערך של פולינום בנקודה נתונה
	//poly: הפולינום לחישוב, x: הערך בנקודה, gf: שדה גלואי בו מבוצע החישוב.
	RS_WORD Poly_Eval(vector<RS_WORD>& poly, RS_WORD x, GaloisField* gf);

	//חיפוש שורשי פולינום במערך המסוים.
	// out: וקטור שמכיל את המיקומים של השורשים, poly: הפולינום לחיפוש, max: מספר המקומות לחיפוש, gf: שדה גלואי בו מבוצע החיפוש.
	void Poly_ChienSearch(vector<uint8_t>* out, vector<RS_WORD>& poly, int max, GaloisField* gf);

	//הוספת אפסים מיותרים לפולינום
	//poly: הפולינום להוספת האפסים, left, right: מספר האפסים להוספה בכל צד.
	void Poly_Pad(vector<RS_WORD>& poly, int left, int right);

	//הסרת אפסים מיותרים מהפולינום
	//poly: הפולינום להוספת האפסים, left, right: מספר האפסים להוספה בכל צד.
	void Poly_Trim(vector<RS_WORD>& poly, int left, int right);

	//הפונקציה מוסיפה את תוכן הפולינום b לסוף הפולינום a ומכניסה את התוצאה לפולינום out.
	//out: מצביע לפולינום המורכב שישמור את התוצאה. a: מצביע לפולינום המקורי שיקבל את ההוספה. b : מצביע לפולינום המקורי שיתווסף לסוף a.
	void Poly_Append(vector<RS_WORD>& out, vector<RS_WORD>& a, vector<RS_WORD>& b);

	//הפונקציה משפטת את הפולינום in ומכניסה את התוצאה לפולינום out.
	void Poly_Reverse(vector<RS_WORD>& out, vector<RS_WORD>& in);

	PolyError getLastError();
};

