//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
//#include "doctest.h"
//#include "UsePoly.h" 
//#include "GaloisField.h" 
//#include "UseException.h"
//#include "Polynom.h"
//typedef uint8_t RS_WORD;
//
//
//TEST_CASE("Polynom_Create") {
//    UsePoly usePolynom;
//    vector<RS_WORD> coef= { 1, 2, 3 };
//
//    // יצירת פולינום ריק (n=0)
//
//    // יצירת פולינום עם גודל חיובי (n>0) וערכים אקראיים
//
//    vector<RS_WORD> poly = usePolynom.Poly_Create(coef);
//    //poly.setCopy(poly); // העברת הנתונים לפולינום
//    CHECK(poly.at(0) == 1);
//    CHECK(poly.at(1) == 2);
//    CHECK(poly.at(2) == 3);
//
//    CHECK(poly.size() > 0);
//
//    // בדיקה שאין ערכים מחוץ לטווח 0-255
//    for (auto& value : poly) {
//        CHECK(value >= 0);
//        CHECK(value <= 255);
//    }
//
//    // בדיקה שאין ערכים שליליים
//    for (auto& value : poly) {
//        CHECK(value >= 0);
//    }
//    //הקוד לא נותן מראש לאתחל ערכים מעל 255 או NULL
//}
//
//
//TEST_CASE("Polynom_Free") {
//    UsePoly usePolynom;
//    vector<RS_WORD> coef = { 1, 2, 3 };
//
//    //לא נותן, יש כזה דבר אתחול וקטור ריק?
//   /* SUBCASE("Free empty Polynom") {
//        vector<RS_WORD> Polynom = usePolynom.Poly_Create(nullptr);
//        CHECK_NOTHROW(usePolynom.Poly_Free(Polynom)); 
//    }*/
//
//    SUBCASE("Free Polynom with positive size") {
//        vector<RS_WORD> Polynom = usePolynom.Poly_Create(coef);
//        CHECK_NOTHROW(usePolynom.Poly_Free(Polynom)); 
//    }
//     
//    /*SUBCASE("Free already freed Polynom") {
//        vector<RS_WORD> Polynom = usePolynom.Poly_Create(coef);
//        CHECK_NOTHROW(usePolynom.Poly_Free(Polynom)); 
//        CHECK_THROWS_AS(usePolynom.Poly_Free(Polynom), UseException); 
//    }*/
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
//TEST_CASE("Polynom_Add") {
//    UsePoly usePolynom;
//    vector<RS_WORD> coefA = { 1, 2, 3 };
//    vector<RS_WORD> coefB = { 3, 2, 1 };
//
//    SUBCASE("Polynom_Add handles memory allocation failures") {
//
//    }
//
//    SUBCASE("Add two empty Polynomnomials") {
//        vector<RS_WORD> vec;
//        vector<RS_WORD> a = usePolynom.Poly_Create(vec);
//        vector<RS_WORD> b = usePolynom.Poly_Create(vec);
//        vector<RS_WORD> out = usePolynom.Poly_Create(vec);
//
//        //CHECK_THROWS_AS(usePolynom.Poly_Add(out, a, b), std::invalid_argument);
//
//        CHECK_NOTHROW(usePolynom.Poly_Add(out, a, b)); 
//        usePolynom.Poly_Free(a);
//        usePolynom.Poly_Free(b);
//        usePolynom.Poly_Free(out);
//    }
//
//    SUBCASE("Add two Polynomnomials with equal size and random values") {
//        vector<RS_WORD> a = usePolynom.Poly_Create(coefA);
//        vector<RS_WORD> b = usePolynom.Poly_Create(coefB);
//        vector<RS_WORD> out = usePolynom.Poly_Create(coefA);
//        CHECK_NOTHROW(usePolynom.Poly_Add(out, a, b)); 
//        CHECK(out.at(0) == 3);
//        CHECK(out.at(1) == 2);
//        CHECK(out.at(2) == 1);
//        usePolynom.Poly_Free(a);
//        usePolynom.Poly_Free(b);
//        usePolynom.Poly_Free(out);
//    }
//
//    SUBCASE("Add two Polynomnomials with different sizes and random values") {
//        vector<RS_WORD> vec;
//        vector<RS_WORD> a = usePolynom.Poly_Create(coefA);
//        vector<RS_WORD> b = usePolynom.Poly_Create(coefB);
//        vector<RS_WORD> out = usePolynom.Poly_Create(vec);
//        CHECK_NOTHROW(usePolynom.Poly_Add(out, a, b)); 
//        CHECK(out.at(0) == 2);
//        CHECK(out.at(1) == 0);
//        CHECK(out.at(2) == 2);
//        usePolynom.Poly_Free(a);
//        usePolynom.Poly_Free(b);
//        usePolynom.Poly_Free(out);
//    }
//
//    SUBCASE("Add empty Polynomnomial to another Polynomnomial") {
//        vector<RS_WORD> vec;
//        vector<RS_WORD> a = usePolynom.Poly_Create(coefA);
//        vector<RS_WORD> b = usePolynom.Poly_Create(vec); 
//        vector<RS_WORD> out = usePolynom.Poly_Create(vec);
//
//        CHECK_NOTHROW(usePolynom.Poly_Add(out, a, b));
//
//        CHECK(out.size() == coefA.size()); 
//        for (size_t i = 0; i < coefA.size(); ++i) {
//            CHECK(out[i] == coefA[i]); 
//        }
//
//        usePolynom.Poly_Free(a);
//        usePolynom.Poly_Free(b);
//        usePolynom.Poly_Free(out);
//
//        /*CHECK(out.at(0) == 1);
//        CHECK(out.at(1) == 2);
//        CHECK(out.at(2) == 3);*/
//    }
//
//    SUBCASE("Add Polynomnomials with empty vectors should throw error") {
//        vector<RS_WORD> emptyVec;
//        vector<RS_WORD> a = usePolynom.Poly_Create(emptyVec);
//        vector<RS_WORD> b = usePolynom.Poly_Create(coefB);
//        vector<RS_WORD> out = usePolynom.Poly_Create(emptyVec);
//        CHECK_NOTHROW(usePolynom.Poly_Add(out, a, b));
//        //CHECK_THROWS_AS(usePolynom.Poly_Add(out, a, b), invalid_argument);
//
//        usePolynom.Poly_Free(a);
//        usePolynom.Poly_Free(b);
//        usePolynom.Poly_Free(out);
//    }
//
//
//    SUBCASE("Add Polynomnomials with unallocated dynamic memory") {
//        vector<RS_WORD> vec; 
//        vector<RS_WORD> a;   
//        vector<RS_WORD> b;   
//        vector<RS_WORD> out = usePolynom.Poly_Create(vec); 
//        CHECK_NOTHROW(usePolynom.Poly_Add(out, a, b)); 
//        CHECK(out.empty()); 
//        usePolynom.Poly_Free(out);
//    }
//}
//
//TEST_CASE("Polynom_Scale") {
//    UsePoly usePolynom;
//    GaloisField gf(0x11d);
//    vector<RS_WORD> coef = { 1, 2, 3 };
//    vector<RS_WORD> vec;
//
//    SUBCASE("Scale empty polynomial") {
//        vector<RS_WORD> Polynom = usePolynom.Poly_Create(vec);
//        vector<RS_WORD> out = usePolynom.Poly_Create(vec);
//        usePolynom.Poly_Scale(out, Polynom, 5, &gf);
//        CHECK(out.empty()); // Expect empty vector
//        usePolynom.Poly_Free(Polynom);
//        usePolynom.Poly_Free(out);
//    }
//
//    SUBCASE("Scale polynomial with positive scalar") {
//        vector<RS_WORD> Polynom = usePolynom.Poly_Create(coef);
//        vector<RS_WORD> out = usePolynom.Poly_Create(coef); // Ensure out has the same size as coef
//        usePolynom.Poly_Scale(out, Polynom, 5, &gf);
//
//        RS_WORD expected0 = gf.mult(1, 5);
//        RS_WORD expected1 = gf.mult(2, 5);
//        RS_WORD expected2 = gf.mult(3, 5);
//
//        CHECK(static_cast<int>(out.at(0)) == expected0);
//        CHECK(static_cast<int>(out.at(1)) == expected1);
//        //CHECK(static_cast<int>(out.at(2)) == expected2);
//
//        usePolynom.Poly_Free(Polynom);
//        usePolynom.Poly_Free(out);
//    }
//
//    SUBCASE("Scale polynomial with zero scalar") {
//        vector<RS_WORD> Polynom = usePolynom.Poly_Create(coef);
//        vector<RS_WORD> out = usePolynom.Poly_Create(coef);
//        usePolynom.Poly_Scale(out, Polynom, 0, &gf);
//
//        CHECK(out.at(0) == 0);
//        CHECK(out.at(1) == 0);
//        CHECK(out.at(2) == 0);
//
//        usePolynom.Poly_Free(Polynom);
//        usePolynom.Poly_Free(out);
//    }
//}
//
//TEST_CASE("Poly_Div") {
//    UsePoly usePolynom;
//    GaloisField gf(0x11d);
//
//    SUBCASE("Divide polynomials with valid input") {
//        vector<RS_WORD> a = { 1, 2, 3 }; // פולינום מחולק
//        vector<RS_WORD> b = { 1, 1 };     // פולינום מחלק
//        vector<RS_WORD> quotient;         // פולינום התוצאה
//        vector<RS_WORD> remainder;        // השארית
//
//        // בצע את חלוקת הפולינומים
//        usePolynom.Poly_Div(quotient, remainder, a, b, &gf);
//
//        // אמת את התוצאות
//        CHECK(quotient.size() == 2); // הפולינום התוצאה צריך להיות בגודל 2
//        CHECK(quotient[0] == 1);    // התוצאה הראשונה של החילוק
//        CHECK(quotient[1] == 1);    // התוצאה השנייה של החילוק
//
//        CHECK(remainder.empty()); // השארית צריכה להיות בגודל 1
//        CHECK(remainder[0] == 0);    // התוצאה של השארית
//    }
//
//    SUBCASE("Divide polynomials with leading zero") {
//        vector<RS_WORD> a = { 0, 1, 2, 3 }; // פולינום מחולק עם אפס בהתחלה
//        vector<RS_WORD> b = { 1, 1 };        // פולינום מחלק
//        vector<RS_WORD> quotient;            // פולינום התוצאה
//        vector<RS_WORD> remainder;           // השארית
//
//        // בצע את חלוקת הפולינומים
//        usePolynom.Poly_Div(quotient, remainder, a, b, &gf);
//
//        // אמת את התוצאות
//        CHECK(quotient.size() == 2); // הפולינום התוצאה צריך להיות בגודל 2
//        CHECK(quotient[0] == 1);    // התוצאה הראשונה של החילוק
//        CHECK(quotient[1] == 1);    // התוצאה השנייה של החילוק
//
//        CHECK(remainder.empty()); // השארית צריכה להיות בגודל 1
//        CHECK(remainder[0] == 0);    // התוצאה של השארית
//    }
//
//    SUBCASE("Divide polynomials with empty inputs") {
//        vector<RS_WORD> a;             // פולינום מחולק ריק
//        vector<RS_WORD> b = { 1, 1 };  // פולינום מחלק
//        vector<RS_WORD> quotient;      // פולינום התוצאה
//        vector<RS_WORD> remainder;     // השארית
//
//        CHECK_THROWS_AS(usePolynom.Poly_Div(quotient, remainder, a, b, &gf), UseException); // צריך לזרוק שגיאה
//    }
//
//    SUBCASE("Divide polynomials with zero divisor") {
//        vector<RS_WORD> a = { 1, 2, 3 }; // פולינום מחולק
//        vector<RS_WORD> b = { 0 };        // פולינום מחלק הוא אפס (לא חוקי)
//        vector<RS_WORD> quotient;         // פולינום התוצאה
//        vector<RS_WORD> remainder;        // השארית
//
//        CHECK_THROWS_AS(usePolynom.Poly_Div(quotient, remainder, a, b, &gf), UseException); // צריך לזרוק שגיאה
//    }
//}
//
//
//
//TEST_CASE("Poly_Mult") {
//    UsePoly usePolynom;
//    GaloisField gf(0x11d);
//
//    SUBCASE("Multiply polynomials") {
//        vector<RS_WORD> a = { 1, 2 };
//        vector<RS_WORD> b = { 3, 4 };
//        vector<RS_WORD> out;
//        out.resize(a.size() + b.size() - 1);
//
//        usePolynom.Poly_Mult(out, a, b, &gf);
//
//        RS_WORD expected_0 = gf.mult(1, 3);
//        RS_WORD expected_1 = gf.mult(1, 4) ^ gf.mult(2, 3);
//        RS_WORD expected_2 = gf.mult(2, 4);
//
//        CHECK(static_cast<int>(out[0]) == expected_0);
//        CHECK(static_cast<int>(out[1]) == expected_1);
//        CHECK(static_cast<int>(out[2]) == expected_2);
//    }
//}
//
////כותב שזה מידי מסובך- כנראה בגלל הMULT
//TEST_CASE("Poly_Eval") {
//    UsePoly usePolynom;
//    GaloisField gf(0x11d); // הגדרת השדה
//
//    SUBCASE("Evaluate polynomial") {
//        vector<RS_WORD> poly = { 1, 2, 3 }; // Polynomial: 1 + 2x + 3x^2
//        RS_WORD x = 2;
//
//        RS_WORD result = usePolynom.Poly_Eval(poly, x, &gf);
//        RS_WORD expected = gf.mult(3, gf.mult(x, x)) ^ gf.mult(2, x) ^ 1;
//        CHECK(result == expected); // Expected result
//    }
//}
//
//
//TEST_CASE("Poly_ChienSearch") {
//    UsePoly usePolynom;
//    GaloisField gf(0x11d); // הגדרת השדה
//
//    SUBCASE("Chien search for polynomial roots") {
//        vector<RS_WORD> poly = { 1, 0, 1 }; // Polynomial: x^2 + 1
//        vector<uint8_t> roots;
//
//        usePolynom.Poly_ChienSearch(&roots, poly, 16, &gf);
//
//        CHECK(roots.size() == 1); // Expecting 1 root
//        CHECK(roots[0] == 0); // Expected root
//    }
//}
//
//TEST_CASE("Poly_Pad") {
//    UsePoly usePolynom;
//    GaloisField gf(0x11d); // הגדרת השדה
//
//    SUBCASE("Pad polynomial") {
//        vector<RS_WORD> poly = { 1, 2, 3 };
//        usePolynom.Poly_Pad(poly, 2, 3);
//
//        CHECK(poly.size() == 8);
//        CHECK(poly[0] == 0); // ודא שהרווחים בצד השמאלי הם אפס
//        CHECK(poly[1] == 0); // ודא שהרווחים בצד השמאלי הם אפס
//        CHECK(poly[2] == 1); // ערכים של הפולינום המקורי
//        CHECK(poly[3] == 2); // ערכים של הפולינום המקורי
//        CHECK(poly[4] == 3); // ערכים של הפולינום המקורי
//        CHECK(poly[5] == 0); // ודא שהרווחים בצד הימני הם אפס
//        CHECK(poly[6] == 0); // ודא שהרווחים בצד הימני הם אפס
//        CHECK(poly[7] == 0); // ודא שהרווחים בצד הימני הם אפס
//    }
//}
//
//
//TEST_CASE("Poly_Trim") {
//    UsePoly usePolynom;
//    GaloisField gf(0x11d); // הגדרת השדה
//
//    SUBCASE("Trim polynomial") {
//        vector<RS_WORD> poly = { 0, 0, 1, 2, 3, 0, 0 };
//        usePolynom.Poly_Trim(poly, 2, 2);
//
//        CHECK(poly.size() == 3);
//        CHECK(poly[0] == 1); // ערכים לאחר חיתוך
//        CHECK(poly[1] == 2); // ערכים לאחר חיתוך
//        CHECK(poly[2] == 3); // ערכים לאחר חיתוך
//    }
//}
//
//TEST_CASE("Poly_Append") {
//    UsePoly usePolynom;
//    GaloisField gf(0x11d); // הגדרת השדה
//
//    SUBCASE("Append polynomials") {
//        vector<RS_WORD> a = { 1, 2 };
//        vector<RS_WORD> b = { 3, 4 };
//        vector<RS_WORD> out;
//
//        usePolynom.Poly_Append(out, a, b);
//
//        CHECK(out.size() == 4); // גודל הוקטור צריך להיות 4
//        CHECK(out[0] == 1);    // התוצאה של a
//        CHECK(out[1] == 2);    // התוצאה של a
//        CHECK(out[2] == 3);    // התוצאה של b
//        CHECK(out[3] == 4);    // התוצאה של b
//    }
//}
//
//TEST_CASE("Poly_Reverse") {
//    UsePoly usePolynom;
//    GaloisField gf(0x11d); // הגדרת השדה
//
//    SUBCASE("Reverse polynomial") {
//        vector<RS_WORD> in = { 1, 2, 3 };
//        vector<RS_WORD> out;
//
//        usePolynom.Poly_Reverse(out, in);
//
//        CHECK(out.size() == 3); // גודל הוקטור צריך להיות 3
//        CHECK(out[0] == 3);    // הערך הראשון צריך להיות 3
//        CHECK(out[1] == 2);    // הערך השני צריך להיות 2
//        CHECK(out[2] == 1);    // הערך השלישי צריך להיות 1
//    }
//}
//
