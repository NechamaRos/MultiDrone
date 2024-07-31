#include "ReedSoloEncode.h"
#include "Meta_Data.h"
#include "UsePoly.h"
#include "GaloisField.h"
#include "Data.h"
#include "doctest.h"



ReedSoloEncode::ReedSoloEncode(int fieldPower) : ReedSolomon(fieldPower)
{
}

//nsym:מיקום השגיאות בקוד.
 void ReedSoloEncode::createGenerator(Polynom* out, int nsym )
{
	//vector<unsigned int> metaData;

	//כאן אני כבר בונה ממש את שדה הפולינום המחולל
	 vector<RS_WORD> emptyVectorToCopy;
	out->setCopy(emptyVectorToCopy);
	out->coef[0] = 1;
	UsePoly usePoly;
	vector<RS_WORD> emptyVector;
	
	Polynom factor(2, emptyVector);
	factor.coef[0] = 1;
	for (int i = 0; i < nsym; i++)
	{
		
		factor.coef[1] = this->gf.powTable[i];

		//void UsePoly::Poly_Mult(vector<RS_WORD>&out, vector<RS_WORD>&a, vector<RS_WORD>&b, GaloisField * gf)
		usePoly.Poly_Mult(out->coef, out->coef, factor.coef, &this->gf);
	}

}
//
// ////אחרי שהטסטים לעיל יעברו...
 ///
 //k=orginalData ( data)  nsym=metaData+orginalData
 void ReedSoloEncode::encode(vector<RS_WORD> out, vector<RS_WORD> data, int nsym)
 {
	 int k = data.size();
	 UsePoly usePoly;
	// כאן יש את הנתונים המקוריים
	 Polynom msg(k, data);
	 int meta_DataSize = nsym - k;
	 Meta_Data Data(meta_DataSize);
	// Data data(msg);//generatorזה במקום ה
	 Polynom generator;
	 Polynom remainder;
	 vector<RS_WORD> emptyVector;
	 //this->createGenerator(&generator, nsym);
	 //////////////////////////////////////////////////
	 /////////////////////////////////////////////////////
	 // - יש עכשיו את הפולינום המחוללgenerator ב
	 //לפיו מתקנים את השגיאות בקוד הריד סולומון
	 this->createGenerator(&generator, nsym);

	 //שמה מימין לנתונים המקוריים 
	 //   בגודל nsym מקום מיוחד לקידוד
	 //בנתונים המקוריים אני מוסיפה אפסים כמספר הקידוד שבחרתי מצד ימין

	 usePoly.Poly_Pad(msg.coef, 0, nsym);//////////////////////////////לשנות לא להוסיף כלום
	 vector<RS_WORD> generatorCoef= generator.coef;
	// זוהי מחשב את שורת השארית המחושבת אותה חיפשנו!!!!!!!
	 //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 //usePoly.Poly_Div(emptyVector, emptyVector, remainder.coef, msg.coef, generatorCoef, gf);/////////////////////
	 //////////////////////////////////////////////////////////////////////////////////////////////////////////
	 //Poly_Div(vector<RS_WORD>&quotient, vector<RS_WORD>&remainder, vector<RS_WORD>&a, vector<RS_WORD>&b, GaloisField * gf)
	 //כאן זה הDATA
	// Data.setOrginalData(data, k);

	
	//זוהי הMETADATA שלנו.
	 Data.setRecavery( remainder, nsym-k);////לשנות!!!!

 }
 int  main() {
	 ReedSoloEncode rs(2);
	 vector<RS_WORD> out;
	 vector<RS_WORD> data = { 1,2,3,4 };
	 rs.encode(out, data, 2);

	 return 0;
 }
