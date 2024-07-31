#include "TextHandler.h"
#include <string>
using namespace std;

vector<uint8_t> TextHandler::toOneDementionalArray(string text)
{
	vector<uint8_t> data(text.begin(), text.end());
	return data;
}

TextHandler::TextHandler()
{
}






