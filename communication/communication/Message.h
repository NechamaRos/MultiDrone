#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Message
{
public:

	virtual vector<vector<vector<vector<uint8_t>>>> getreminder(int ,int,int,  vector<uint8_t>&) ;
	virtual vector<vector<vector<uint8_t>>> getreminder(int,int,  vector<uint8_t>&) ;
	virtual vector<uint8_t> getreminder(int,vector<uint8_t>&) ;
};

