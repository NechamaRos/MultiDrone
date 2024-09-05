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

	std::tuple<int, int> getSize();


	void setAllPixel(int i, int j, const std::vector<uint8_t>& pixel);

	void setPixelCH(int i, int j, int c, int value);


	Image row(int y) const;

	Image col(int x) const;

	bool empty();

	Image clone()const;

	std::vector<std::vector<uint8_t>>splitChannels()const;

	static std::vector<uint8_t>margeChannels(std::vector<std::vector<uint8_t>>v);

	std::vector<uint8_t>getData();


	uint8_t at(int i, int j, int c);

	std::vector<uint8_t> at(int i, int j)const;


	uint8_t* ptr(int i);

	uint8_t* ptr(int i, int j);

	uint8_t* ptr(int i, int j, int c);


	Image operator+(Image const& other) const;

	Image operator=(Image const& other);

	Image operator +(uint8_t value) const; // by element

	Image operator -(uint8_t value) const; // by element

	bool operator ==(Image const& other) const; // compare by element to other image

	bool operator ==(uint8_t const& value) const; // compare all elements to value

};

