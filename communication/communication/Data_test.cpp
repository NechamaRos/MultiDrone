#include "doctest.h"
#include "Data.h"

VecValidation vv({ 1,0 });
Meta_Data<1> mdv(&vv);
VecMessage vm1 ({ 'a','b','c' });
Data<1> vd(mdv, &vm1);

MatValidation mv({ 1 });
Meta_Data<2> mdm(&mv);
MatMessage mm1({ {1,1,1,0 },{1,1,1,0 },{1,1,1,0 } });
Data<2> md(mdm, &mm1);

D3Validation d3v({ 1 });
Meta_Data<3> mdd(&d3v);
D3Message d3m1({ { {1,1,1,0 },{1,1,1,0 },{1,1,1,0 } } ,{ {1,1,1,0 },{1,1,1,0 },{1,1,1,0 } } });
Data<3> d3d(mdd, &d3m1);


TEST_CASE("Constructor") {
	SUBCASE("Should build the object with the parameters") {
		VecValidation v1({ 1,0 });
		Meta_Data<1> md1(&v1);
		VecMessage m1({ 'a','b','c' });
		
		CHECK_NOTHROW(Data<1> d1(md1, &m1));
	}
	SUBCASE("Should throw an error if types of validation and message are not match") {
		MatValidation v1({ 1,0 });
		Meta_Data<2> md1(&v1);
		VecMessage m1({ 'a','b','c' });

		CHECK_THROWS_AS(Data<2> d1(md1, &m1),invalid_argument);
	}
	SUBCASE("Should throw an error if size of validation-data is longer then log2(sizeof data)") {
		MatValidation v1({ 1,0 });
		Meta_Data<2> md1(&v1);
		MatMessage m1({ {1,1,1,0 },{1,1,1,0 },{1,1,1,0 } });

		CHECK_THROWS_AS(Data<2> d1(md1, &m1), invalid_argument);
	}

}
TEST_CASE("Send function") {
	SUBCASE("Should set validation-data by message") {
		CHECK_NOTHROW(vd.send());
		CHECK_NOTHROW(md.send());
		CHECK_NOTHROW(d3d.send());
	}
	SUBCASE("should print an errors from crc-algorithm functions") {
		Meta_Data<2> mdm1(mdm);
		MatMessage mm11({ {1,1,2,0 },{1,1,3,0 },{1,1,1,0 } });
		Data<2> md1(mdm1, &mm11);
		 
		md1.send();
	}
}
TEST_CASE("Receive function") {
	SUBCASE("Should verify received data and print if the data is wrong") {
		CHECK_NOTHROW(vd.receive());
		CHECK_NOTHROW(md.receive());
		CHECK_NOTHROW(d3d.receive());
	}
	SUBCASE("Should throw an error if the Not filled") {
		Data<1> vd1(mdv, &vm1);
		CHECK_THROWS_AS(vd1.receive(), logic_error);
	}
}
