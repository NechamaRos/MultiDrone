#include "Image.h"
using namespace std;

tuple<int, int> Image::getSize()
{
	return make_tuple(width, height);
}

bool Image::empty()
{
	return width * height == 0 || data == nullptr;
}

Image Image::clone()const
{
	vector<uint8_t> res;
	for (int i = 0; i < width * height * channels; i++)
	{
		res.push_back(data[i]);
	}
	Image m(width, height, channels, res);
	return m;
}

vector<vector<uint8_t>> Image::splitChannels()const
{
	vector<uint8_t>red;
	vector<uint8_t>green;
	vector<uint8_t>blue;
	for (int index = 0; index < width * height * channels; index++)
	{
		if (index % 3 == 0)
			red.push_back(data[index]);
		else
		{
			if (index % 3 == 1)
				green.push_back(data[index]);
			else
				blue.push_back(data[index]);
		}
	}
	vector<vector<uint8_t>>res({ red,green,blue });
	return res;
}

vector<uint8_t> Image::getData()
{
	vector<uint8_t>data;
	for (int i = 0; i < width * height * channels; i++)
	{
		data.push_back(this->data[i]);
	}
	return data;
}

std::vector<uint8_t> Image::at(int i, int j) {
	if (i < 0 || i >= height || j < 0 || j >= width) {
		throw std::out_of_range("Index out of range");
	}

	std::vector<uint8_t> pixel(channels);
	for (int c = 0; c < channels; ++c) {
		pixel[c] = data[(i * width * channels) + (j * channels) + c];
	}
	pixel[0] = 180;
	return pixel;
}

uint8_t Image::at(int i, int j, int c) {
	if (i<0 || i>height || j<0 || j>width || c < 0 || c>2) throw out_of_range("Index out of range");
	return data[(i * width * channels) + (j * channels) + c];
}

uint8_t* Image::ptr(int i)
{
	if (i<0 || i>height) throw out_of_range("Index out of range");
	return data + i * width * channels;
}

uint8_t* Image::ptr(int i, int j)
{
	if (i<0 || i>height || j<0 || j>width) throw out_of_range("Index out of range");
	return data + i * width * channels + j * channels;
}

uint8_t* Image::ptr(int i, int j, int c)
{
	if (i<0 || i>height || j<0 || j>width || c < 0 || c>2) throw out_of_range("Index out of range");
	return data + i * width * channels + j * channels + c;
}


Image Image::operator+(uint8_t value) const
{
	Image m(width, height, channels);
	for (int index = 0; index < width * height * channels; index++)
		m.data[index] = data[index] + value;
	return m;
}

Image Image::operator-(uint8_t value) const
{
	Image m(width, height, channels);
	for (int index = 0; index < width * height * channels; index++)
		m.data[index] = data[index] - value;
	return m;
}

bool Image::operator==(Image const& other) const
{
	if (width != other.width || height != other.height || channels != other.channels)
		throw exception("It is impossible to compare objects with different dimensions");
	for (int i = 0; i < width * height * channels; i++)
	{
		if (other.data[i] != data[i])
			return false;
	}
	return true;
}

bool Image::operator==(uint8_t const& value) const
{
	for (int i = 0; i < width * height * channels; i++)
	{
		if (data[i] != value)
			return false;
	}
	return true;
}

Image Image::operator=(Image const& other)
{
	Image m;
	m.width = other.width;
	height = other.height;
	ROI = other.ROI;
	channels = other.channels;
	data = other.data;
	refCount = (other.refCount);
	(*refCount)++;
	return m;
}

uint8_t& Image::operator()(int i, int j, int c) {
	if (i < 0 || i >= height || j < 0 || j >= width || c < 0 || c >= channels) {
		throw std::out_of_range("Index out of range");
	}
	return data[(i * width * channels) + (j * channels) + c];
}

void Image::copy(Image img)
{
	if (width != img.width || height != img.height || ROI != img.ROI || channels != img.channels)
		throw out_of_range("Index out of range");
	data = img.data;
	(*img.refCount)++;
	(*refCount)--;
}

void Image::copy2(Image img)
{
	if (width != img.width || height != img.height || ROI != img.ROI || channels != img.channels)
		throw out_of_range("Index out of range");
	for (int i = 0; i < width * height * channels; i++)
	{
		*(data + i) = *(img.data + i);
	}
}

void Image::waitKey(int delay)
{
	this_thread::sleep_for(std::chrono::seconds(delay));
}


vector<uint8_t> Image::getAllPixel(int i, int j) const {
	if (i < 0 || i >= height || j < 0 || j >= width) {
		throw std::out_of_range("Index out of range");
	}

	std::vector<uint8_t> pixel(channels);
	for (int c = 0; c < channels; ++c) {
		pixel[c] = data[(i * width * channels) + (j * channels) + c];
	}
	return pixel;
}

uint8_t& Image::getPixelCH(int i, int j, int c)
{
	if (i < 0 || i >= height || j < 0 || j >= width || c < 0 || c >= channels) {
		throw std::out_of_range("Index out of range");
	}
	return data[(i * width * channels) + (j * channels) + c];
}

void Image::setAllPixel(int i, int j, const vector<uint8_t>& pixel)
{

	if (pixel.size() < 0 || pixel.size() > 3 || i < 0 || i >= height || j < 0 || j >= width) throw std::out_of_range("Index out of range");
	for (int c = 0; c < channels; c++)
	{
		setPixelCH(i, j, c, pixel.at(c));
	}
}

void Image::setPixelCH(int i, int j, int c, int value)
{
	if (value < 0 || value>255) throw out_of_range("Index out of range");
	if (i<0 || i>height || j<0 || j>width || c < 0 || c>2) throw out_of_range("Index out of range");
	if (*refCount > 1) {}//function that nechami did
	data[(i * width * channels) + (j * channels) + c] = value;
}