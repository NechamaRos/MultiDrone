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
	Image();
	//c'tor
	//Image(int w, int h, int c = 3, std::vector<uint8_t> const& _data = {});
	Image(int w, int h, int c = 3, std::vector<uint8_t> const& _data = {})
	{
		if (c != 1 && c != 3)
			throw std::exception("The channels must be 1 or 3");
		if (w < 1 || h < 1)
			throw std::exception("the arguments are invalid");
		width = w;
		height = h;
		channels = c;
		refCount = new int;
		*refCount = 1;
		ROI = width;
		data = new uint8_t[width * c * height];
		if (!_data.empty())
			memcpy(data, _data.data(), width * c * height);
	}

	//view c'tor
	Image(Image const& other, int tl_x, int tl_y, int ROI_width, int ROI_height);

	//copy c'tor
	Image(Image const& other);

	//move c'tor
	Image(Image&& other)noexcept;
	
	//d'tor
	~Image();

	//get a path of image - read the image and return it
	static Image readImage(std::string filename);

	//get an image and show it in the screem for 5 minutes by defualt
	static void displayImage(const Image& img, int delay=5);
	
	//get a path and an image and write the image in the path
	static void writeImage(const std::string& filename, const Image& image);

	//stop the program for 5 minutes by defualt
	static void waitKey(int delay = 5);
	
	//get a sorce image and new size- and return a new image in the new size
	static Image resize(const Image& src, std::tuple<int, int> const size);

	//return the whole size of the image- width*height
	int total();

	//return a tuple of the width and height
	std::tuple<int, int> getSize();


	//get a row, a col and a vector of three number- B,G,R- and set in this place this vector of pixel
	void setAllPixel(int i, int j, const std::vector<uint8_t>& pixel);

	//get a row, a col and number-0/1/2 and value - and set to specific channel in a pixel- 0-B, 1-G, 2-R  the value
	void setPixelCH(int i, int j, int c, int value);

	//get a number of row and return a new Image of width*1 with the data of this row
	Image row(int y) const;
	
	//get a number of col and return a new Image of 1*height with the data of this col
	Image col(int x) const;

	//return true if the image is empty
	bool empty();

	//copy the data of the Image
	Image clone()const;

	//split the data of the image to three vector of B,G,R
	std::vector<std::vector<uint8_t>>splitChannels()const;

	//get three vectors - B,G,R and marge them together
	std::vector<uint8_t> mergeChannels(std::vector<std::vector<uint8_t>>v);

	//return the data of the image
	std::vector<uint8_t>getData();

	//returns the location of the row and column and the color sent
	uint8_t at(int i, int j, int c);

	//returns the location of the row and column sent
	std::vector<uint8_t> at(int i, int j);

	//pointer to start of a raw
	uint8_t* ptr(int i);

	//pointer to start of a pixel
	uint8_t* ptr(int i, int j);

	//pointer to specific channel in a pixel
	uint8_t* ptr(int i, int j, int c);

	//like copy c'tor
	Image operator=(Image const& other);

	//add to all the pixels the value
	Image operator +(uint8_t value) const;
	
	//connect tow Image if match
	Image operator +(Image const& other) const; 

	//sub to all the pixels the value
	Image operator -(uint8_t value) const; // by element

	bool operator ==(Image const& other) const; // compare by element to other image

	bool operator ==(uint8_t const& value) const; // compare all elements to value

};

