#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <thread> 
#include <stdlib.h>
#include <fstream>
#include <Windows.h>
#include <conio.h>
class Image
{
public:
	int width;
	int height;
	int channels; 
	int ROI;
	int* refCount;
	uint8_t* data;

	//defult c'tor
	Image()
	{
		refCount = new int;
		data = nullptr;
	}

	//c'tor
	Image(int w, int h, int c = 3, std::vector<uint8_t> const& _data = {});
	
	//view c'tor
	Image(Image const& other, int tl_x, int tl_y, int ROI_width, int ROI_height);
	
	//copy c'tor
	Image(Image const& other);

	//move c'tor
	Image(Image&& other)noexcept :
		width(other.width), height(other.height), ROI(other.ROI), channels(other.channels)
	{
		data = other.data;
		other.data = nullptr;
		refCount = (other.refCount);
		other.refCount = nullptr;
	}

	//d'tor
	~Image();


	static Image readImage(std::string filename);

	static void displayImage(const Image& img,int delay);

	static void writeImage(const std::string& filename, const Image& image);


	static void waitKey(int delay = 0);

	static Image resize(const Image& src, std::tuple<int, int> const size);

	int total();

	//return a tuple of the width and height
	std::tuple<int, int> getSize();


	//get a row, a col and a vector of three number- B,G,R- and set in this place this vector of pixel
	void setAllPixel(int i, int j, const std::vector<uint8_t>& pixel);

	//get a row, a col and number-0/1/2 and value - and set to specific channel in a pixel- 0-B, 1-G, 2-R the value
	void setPixelCH(int i, int j, int c, int value);


	//get a number of row and return a new Image of width*1 with the data of this row
	Image row(int y) const;

	//get a number of col and return a new Image of 1*height with the data of this col
	Image col(int x) const;

	//return true if the image is empty
	bool empty();

	//return new image by copy the data
	Image clone()const;

	//split the data of the image to three vector of B,G,R
	std::vector<std::vector<uint8_t>>splitChannels()const;

	//get three vectors - B,G,R and marge them together
	static std::vector<uint8_t>margeChannels(std::vector<std::vector<uint8_t>>v);

	//return the data of the image
	std::vector<uint8_t>getData();


	//returns the location of the row and column and the color sent
	uint8_t at(int i, int j, int c);

	//returns the location of the row and column sent
	std::vector<uint8_t> at(int i, int j)const;


	//pointer to start of a raw
	uint8_t* ptr(int i);

	//pointer to start of a pixel
	uint8_t* ptr(int i, int j);

	//pointer to specific channel in a pixel
	uint8_t* ptr(int i, int j, int c);

	
	//like copy c'tor
	Image operator=(Image const& other);

	//add to all the pixels the value
	Image operator +(uint8_t value) const; // by element

	//connect tow Image if match
	Image operator+(Image const& other) const;

	//sub to all the pixels the value
	Image operator -(uint8_t value) const; // by element

	// compare by element to other image
	bool operator ==(Image const& other) const; // compare by element to other image

	// compare all elements to value
	bool operator ==(uint8_t const& value) const; // compare all elements to value

};

