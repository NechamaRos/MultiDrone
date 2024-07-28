#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <array>

using namespace std;

/// <summary>
/// Represents an image with a given dimensionality.
/// </summary>
/// <typeparam name="D">The dimensionality of the image.</typeparam>
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
};

/// <summary>
/// Creates a sample image with alternating pixel values of 0 and 255.
/// </summary>
/// <typeparam name="D">The dimensionality of the image.</typeparam>
/// <param name="dims">The dimensions of the image.</param>
/// <returns>The created sample image.</returns>
template<size_t D>
Image<D> createSampleImage(const array<size_t, D>& dims) {
	Image<D> img;
	img.dimensions = dims;
	size_t totalSize = 1;
	for (size_t dim : dims) {
		totalSize *= dim;
	}
	img.pixels.resize(totalSize);

	for (int i = 0; i < totalSize; ++i) {
		img.pixels[i] = i % 2 == 0 ? 255 : 0;
	}

	return img;
}

/// <summary>
/// Creates a random image with pixel values ranging from 0 to 255.
/// </summary>
/// <typeparam name="D">The dimensionality of the image.</typeparam>
/// <param name="dims">The dimensions of the image.</param>
/// <returns>The created random image.</returns>
template<size_t D>
Image<D> createRandomImage(const array<size_t, D>& dims) {
	Image<D> img;
	img.dimensions = dims;
	size_t totalSize = 1;
	for (size_t dim : dims) {
		totalSize *= dim;
	}
	img.pixels.resize(totalSize);

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, 255);

	for (int i = 0; i < totalSize; ++i) {
		img.pixels[i] = dis(gen);
	}

	return img;
}

/// <summary>
/// Creates a simple image with all pixels set to the given value.
/// </summary>
/// <typeparam name="D">The dimensionality of the image.</typeparam>
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
	return img;
}

/// <summary>
/// Compares two images for equality.
/// </summary>
/// <typeparam name="D">The dimensionality of the images.</typeparam>
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
