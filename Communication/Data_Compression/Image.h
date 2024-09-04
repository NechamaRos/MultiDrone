#pragma once
#include <vector>
#include <string>
#include <random>
#include <array>
using namespace std;

/// <summary>
/// Represents an image with a given dimensionality.
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
template<size_t D>
/// <summary>
/// Represents an image with the specified dimensions and pixel data.
/// </summary>
struct Image {
	/// <summary>
	/// The dimensions of the image.
	/// </summary>
    array<size_t, D> dimensions;
	/// <summary>
	/// The pixel data of the image.
	/// </summary>
    vector<unsigned char> pixels;
	/// <summary>
    /// Initializes an image with the given dimensions.
    /// </summary>
    /// <type param name="D">The dimensionality of the image.</type param>
    /// <param name="dims">The dimensions of the image.</param>
    /// <returns>The total size of the image in pixels.</returns>
	void initImage(const array<size_t, D>& dims) {
		dimensions = dims;
		size_t totalSize = 1;
		for (size_t dim : dims) {
			totalSize *= dim;
		}
		pixels.resize(totalSize);
	}
};

/// <summary>
/// Creates a sample image with alternating pixel values of 0 and 255.
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="dims">The dimensions of the image.</param>
/// <returns>The created two color image.</returns>
template<size_t D>
Image<D> createTwoColorImage(const array<size_t, D>& dims) {
	Image<D> img;
	img.initImage(dims);
	
	for (size_t i = 0; i < img.pixels.size(); ++i) {
		img.pixels[i] = i % 2 == 0 ? 255 : 0;
	}

	return img;
}

/// <summary>
/// Creates an image with all characters
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="dims">The dimensions of the image.</param>
/// <returns>The created all color image.</returns>
template<size_t D>
Image<D> createAllValuesImage(const array<size_t, D>& dims) {
	Image<2> img;
	img.initImage(dims);
	for (size_t i = 0; i < img.pixels.size(); ++i) {
		img.pixels[i] = static_cast<unsigned char>(i % 256);
	}
	return img;
}

/// <summary>
/// Creates a random image with pixel values ranging from 0 to 255.
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="dims">The dimensions of the image.</param>
/// <returns>The created random image.</returns>
template<size_t D>
Image<D> createRandomImage(const array<size_t, D>& dims) {
	Image<D> img;
	img.initImage(dims);

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 255);

	for (size_t i = 0; i < img.pixels.size(); ++i) {
		img.pixels[i] = dis(gen);
	}

	return img;
}

/// <summary>
/// Creates a simple image with all pixels set to the given value.
/// </summary>
/// <type param name="D">The dimensionality of the image.</type param>
/// <param name="dims">The dimensions of the image.</param>
/// <param name="value">The pixel value to fill the image with.</param>
/// <returns>The created simple image.</returns>
template<size_t D>
Image<D> createSimpleImage(const array<size_t, D>& dims, unsigned char value) {
	Image<D> img;
	img.dimensions = dims;
	size_t totalSize = 1;
	for (size_t dim : dims) {
		totalSize *= dim;
	}
	img.pixels.resize(totalSize, value);
	//img.initImage(dims);
	//img.pixels.data(value);
	return img;
}

/// <summary>
/// Compares two images for equality.
/// </summary>
/// <type param name="D">The dimensionality of the images.</type param>
/// <param name="img1">The first image to compare.</param>
/// <param name="img2">The second image to compare.</param>
/// <returns>True if the images are equal, false otherwise.</returns>
template<size_t D>
bool compareImages(const Image<D>& img1, const Image<D>& img2) {
	if (img1.dimensions != img2.dimensions) {
		return false;
	}

	return equal(img1.pixels.begin(), img1.pixels.end(), img2.pixels.begin());
}
