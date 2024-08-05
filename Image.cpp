#include "Image.h"
using namespace std;


Image::Image()
{
	refCount = new int;
	data = nullptr;
}

Image::Image(Image const& other, int tl_x, int tl_y, int ROI_width, int ROI_height)
{
	if (tl_x<0 || tl_y + ROI_width > other.ROI || tl_y<0 || tl_x + ROI_height>other.height)
		throw std::invalid_argument("Bad view image dimension");
	width = ROI_width;
	height = ROI_height;
	ROI = other.ROI;
	channels = other.channels;
	data = other.data + tl_x * ROI + tl_y;
	refCount = (other.refCount);
	(*refCount)++;
}

Image::Image(Image const& other)
	:width(other.width), height(other.height), ROI(other.ROI), channels(other.channels)
{
	data = other.data;
	refCount = (other.refCount);
	(*refCount)++;
}

Image::Image(Image&& other)noexcept :
	width(other.width), height(other.height), ROI(other.ROI), channels(other.channels)
{
	data = other.data;
	other.data = nullptr;
	refCount = (other.refCount);
	other.refCount = nullptr;
}

Image::~Image()
{
	if (refCount)
	{
		if (*refCount == 1)
		{
			delete refCount;
			delete[] data;
		}
		else
			(*refCount)--;
	}
}

Image Image::row(int y) const
{
	if (y > height) throw exception("index out of range");
	Image img(width, 1, channels);
	for (int j = 0; j < width - 1; j++)
	{
		img.setAllPixel(0, j, getAllPixel(j, y));
	}
	return img;
}

Image Image::col(int x) const
{
	if (x > width) throw exception("index out of range");
	Image img(1, height, channels);
	for (int i = 0; i < height - 1; i++)
	{
		img.setAllPixel(i, 0, getAllPixel(x, i));
	}
	return img;
}

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
			blue.push_back(data[index]);
		else
		{
			if (index % 3 == 1)
				green.push_back(data[index]);
			else
				red.push_back(data[index]);
		}
	}
	vector<vector<uint8_t>>res({ red,green,blue });
	return res;
}

std::vector<uint8_t> Image::mergeChannels(std::vector<vector<uint8_t>> v)
{
	if (v.size() != 3)
		throw exception("the input is invalid");
	if (v[0].size() != v[1].size() || v[0].size() != v[2].size())
		throw exception("the input is invalid");
	vector<uint8_t>res;
	for (int i = 0; i < v[0].size(); i++)
	{
		res.push_back(v[0][i]);
		res.push_back(v[1][i]);
		res.push_back(v[2][i]);
	}
	return res;
}

vector<uint8_t> Image::getData()
{
	vector<uint8_t>data;
	for (int i = 0; i < width * channels * height; i++)
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

Image Image::operator+(Image const& other) const
{
	if (width == other.width)
	{
		int c = other.channels == 3 || channels == 3 ? 3 : 1;
		Image res(width, height + other.height, c);
		int index = 0;
		for (int i = 0; i < width * height * channels; i++)
		{
			res.data[index++] = data[i];
		}
		for (int i = 0; i < other.width * other.height * other.channels; i++)
		{
			res.data[index++] = other.data[i];
		}
		return res;
	}
	if (height == other.height)
	{
		int c = other.channels == 3 || channels == 3 ? 3 : 1;
		Image res(width + other.width, height, c);
		int index = 0;
		int i1 = 0;
		int i2 = 0;
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				res.data[index++] = data[i1++];
			}
			for (int i = 0; i < other.width; i++)
			{
				res.data[index++] = data[i2++];
			}
		}
		return res;
	}
	else
		return Image();
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
		return false;
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




Image Image::readImage(string filename)
{
	int width = 0, height = 0;
	vector<unsigned char> pixels;

	ifstream file(filename, ios::binary);
	if (!file.is_open()) {
		throw runtime_error("Unable to open file");
	}

	file.seekg(18, ios::beg);
	file.read(reinterpret_cast<char*>(&width), sizeof(width));
	file.read(reinterpret_cast<char*>(&height), sizeof(height));

	file.seekg(54, ios::beg); // Skip to pixel data

	int rowSize = ((width * 3 + 3) / 4) * 4; // Calculate padded row size
	int padding = rowSize - width * 3;
	pixels.resize(width * 3 * height);
	for (int i = height - 1; i >= 0; i--) {
		file.read(reinterpret_cast<char*>(&pixels[i * width * 3]), width * 3);
		file.seekg(padding, ios::cur);
	}

	file.close();

	Image result(width, height, 3, pixels);
	return result;
}

void Image::displayImage(const Image& img, int delay)
{
	int rowSize = img.width * 3; // Calculate padded row size
	int padding = (((img.width * 3 + 3) / 4) * 4) - img.width * 3;
	int rowSizeWithPadding = ((img.width * 3 + 3) / 4) * 4;
	vector<uint8_t>data;
	uint8_t* data2 = new uint8_t[rowSizeWithPadding * img.height];
	int i = 0;
	for (int y = 0; y < img.height; y++) {
		for (int x = 0; x < img.width; x++) {
			int index = (y * img.width + x) * 3;
			data2[i++] = img.data[index]; // Blue channel
			data2[i++] = img.data[index + 1]; // Blue channel
			data2[i++] = img.data[index + 2]; // Blue channel

		}
		// Write padding bytes
		for (int p = 0; p < padding; p++) {
			data2[i++] = uint8_t("\0");
		}
	}



	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = img.width;
	bmi.bmiHeader.biHeight = -img.height;  // Minus height to display image right-side up
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;  // Assuming 24-bit RGB image
	bmi.bmiHeader.biCompression = BI_RGB;

	HWND hwnd = CreateWindowA("STATIC", "Image Viewer", WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, img.width, img.height, NULL, NULL, GetModuleHandle(NULL), NULL);

	HDC hdc = GetDC(hwnd);
	StretchDIBits(hdc, 0, 0, img.width, img.height, 0, 0, img.width, img.height, data2, &bmi, DIB_RGB_COLORS, SRCCOPY);

	img.waitKey(delay);
	ReleaseDC(hwnd, hdc);
	DestroyWindow(hwnd);

}

void Image::writeImage(const std::string& filename, const Image& image)
{
	std::ofstream bmpFile(filename, std::ios::out | std::ios::binary);
	if (!bmpFile) {
		std::cerr << "Error opening file for writing." << std::endl;
		return;
	}

	int rowSize = image.width * 3; // Calculate padded row size
	// Row size including padding
	int padding = (((image.width * 3 + 3) / 4) * 4) - image.width * 3;
	int rowSizeWithPadding = ((image.width * 3 + 3) / 4) * 4;
	int fileSize = 54 + rowSizeWithPadding * image.height; // Total file size
	int imageSize = rowSizeWithPadding * image.height;


	bmpFile.put('B').put('M'); // BMP magic number
	bmpFile.write(reinterpret_cast<const char*>(&fileSize), 4); // File size
	bmpFile.write("\0\0\0\0", 4); // Reserved
	int offsetToData = 54;
	bmpFile.write(reinterpret_cast<const char*>(&offsetToData), 4); // Offset to image data

	// DIB Header (BITMAPINFOHEADER)
	int headerSize = 40;
	bmpFile.write(reinterpret_cast<const char*>(&headerSize), 4); // DIB header size
	bmpFile.write(reinterpret_cast<const char*>(&image.width), 4); // Image width
	bmpFile.write(reinterpret_cast<const char*>(&image.height), 4); // Image height
	short planes = 1;
	bmpFile.write(reinterpret_cast<const char*>(&planes), 2); // Planes
	short bitsPerPixel = 24;
	bmpFile.write(reinterpret_cast<const char*>(&bitsPerPixel), 2); // Bits per pixel
	int compression = 0;
	bmpFile.write(reinterpret_cast<const char*>(&compression), 4); // Compression method
	bmpFile.write(reinterpret_cast<const char*>(&imageSize), 4); // Image size
	int resolutionX = 2835; // Pixels per meter (72 DPI)
	int resolutionY = 2835; // Pixels per meter (72 DPI)
	bmpFile.write(reinterpret_cast<const char*>(&resolutionX), 4); // Horizontal resolution
	bmpFile.write(reinterpret_cast<const char*>(&resolutionY), 4); // Vertical resolution
	int colorsUsed = 0;
	bmpFile.write(reinterpret_cast<const char*>(&colorsUsed), 4); // Colors in color table
	int importantColors = 0;
	bmpFile.write(reinterpret_cast<const char*>(&importantColors), 4); // Important color count

	// Write image data (bottom to top)
	for (int y = image.height - 1; y >= 0; y--) {
		for (int x = 0; x < image.width; x++) {
			int index = (y * image.width + x) * 3;
			bmpFile.write(reinterpret_cast<const char*>(&image.data[index]), 1); // Blue channel
			bmpFile.write(reinterpret_cast<const char*>(&image.data[index + 1]), 1); // Green channel
			bmpFile.write(reinterpret_cast<const char*>(&image.data[index + 2]), 1); // Red channel
		}
		// Write padding bytes
		for (int p = 0; p < padding; p++) {
			bmpFile.write("\0", 1);
		}
	}

	bmpFile.close();
}


void Image::waitKey(int delay)
{
	this_thread::sleep_for(std::chrono::seconds(delay));
}

Image Image::resize(const Image& src, tuple<int, int> const size)
{
	Image image = src;
	int oldWidth = src.width;
	int oldHeight = src.height;
	int newWidth = get<0>(size);
	int newHeight = get<1>(size);
	Image resImage(newWidth, newHeight);
	//double loop of through each pixel in the new image (newWidth and newHeight).
	for (int y = 0; y < newHeight; ++y) {
		for (int x = 0; x < newWidth; ++x) 
		{
			//the positions in the original image that correspond to a pixel in the new image.
			float gx = ((float)(x + 0.5) / newWidth) * oldWidth - 0.5;
			float gy = ((float)(y + 0.5) / newHeight) * oldHeight - 0.5;
			
			//The integer index of the pixel position in the original image.
			int gxi = static_cast<int>(gx);
			int gyi = static_cast<int>(gy);

			//The pixel values ​​(red, green, blue) at the point (gxi, gyi) in the original image
			float c00r = image.getPixelCH(gyi, gxi, 0);
			float c00g = image.getPixelCH(gyi, gxi, 1);
			float c00b = image.getPixelCH(gyi, gxi, 2);

			//The values ​​of the pixels adjacent to the point (gxi, gyi) in the original image.
			// If the pixels go out of bounds, we use the existing pixel values.
			float c10r = (gxi + 1 < oldWidth) ? image.getPixelCH(gyi, gxi + 1, 0) : c00r;
			float c10g = (gxi + 1 < oldWidth) ? image.getPixelCH(gyi, gxi + 1, 1) : c00g;
			float c10b = (gxi + 1 < oldWidth) ? image.getPixelCH(gyi, gxi + 1, 2) : c00b;

			float c01r = (gyi + 1 < oldHeight) ? image.getPixelCH(gyi + 1, gxi, 0) : c00r;
			float c01g = (gyi + 1 < oldHeight) ? image.getPixelCH(gyi + 1, gxi, 1) : c00g;
			float c01b = (gyi + 1 < oldHeight) ? image.getPixelCH(gyi + 1, gxi, 2) : c00b;

			float c11r = (gxi + 1 < oldWidth && gyi + 1 < oldHeight) ? image.getPixelCH(gyi + 1, gxi + 1, 0) : c00r;
			float c11g = (gxi + 1 < oldWidth && gyi + 1 < oldHeight) ? image.getPixelCH(gyi + 1, gxi + 1, 1) : c00g;
			float c11b = (gxi + 1 < oldWidth && gyi + 1 < oldHeight) ? image.getPixelCH(gyi + 1, gxi + 1, 2) : c00b;

			//Calculation of weights for interpolation. Representing the relative distance of the new pixel from the original whole pixel.
			float wx = gx - gxi;
			float wy = gy - gyi;

			//Calculating the new pixel values ​​using interpolation
			uint8_t r = (1 - wx) * (1 - wy) * c00r + wx * (1 - wy) * c10r + (1 - wx) * wy * c01r + wx * wy * c11r;
			uint8_t g = (1 - wx) * (1 - wy) * c00g + wx * (1 - wy) * c10g + (1 - wx) * wy * c01g + wx * wy * c11g;
			uint8_t b = (1 - wx) * (1 - wy) * c00b + wx * (1 - wy) * c10b + (1 - wx) * wy * c01b + wx * wy * c11b;


			//Update the pixel in the new image		 
			vector<uint8_t> pixel = { r, g, b };
			resImage.setAllPixel(y, x, pixel);
		}
	}
	return resImage;
}

int Image::total()
{
	return width * height;
}


vector<uint8_t> Image::getAllPixel(int i, int j) const {
	if (i < 0 || i >= width || j < 0 || j >= height) {
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

	if (pixel.size() < 0 || pixel.size() > 3 || i < 0 || i >= height || j < 0 || j >= width)
		throw std::out_of_range("Index out of range");
	for (int c = 0; c < channels; c++)
	{
		setPixelCH(i, j, c, pixel.at(c));
	}
}

//not return anything, here the copy on write, so what i need to return?
void Image::setPixelCH(int i, int j, int c, int value)
{
	if (value < 0 || value>255) throw out_of_range("Index out of range");
	if (i<0 || i>height || j<0 || j>width || c < 0 || c>2) throw out_of_range("Index out of range");
	data[(i * width * channels) + (j * channels) + c] = value;
}
