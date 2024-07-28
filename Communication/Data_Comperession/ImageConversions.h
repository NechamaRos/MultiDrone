#pragma once
#include"Image.h"

/// <summary>
/// Converts a simple message (vector of chars) to a 1D Image.
/// </summary>
/// <param name="message">The input message as a vector of chars.</param>
/// <returns>A 1D Image representation of the message.</returns>
Image<1> convertSimpleMessageToImage(const vector<char>& message);

/// <summary>
/// Converts a black and white image (2D vector of uint8_t) to a 2D Image.
/// </summary>
/// <param name="bwImage">The input black and white image.</param>
/// <returns>A 2D Image representation of the black and white image.</returns>
Image<2> convertBlackWhiteToImage(const vector<vector<uint8_t>>& bwImage);

/// <summary>
/// Converts a color image (3D vector of uint8_t) to a 3D Image.
/// </summary>
/// <param name="colorImage">The input color image.</param>
/// <returns>A 3D Image representation of the color image.</returns>
Image<3> convertColorToImage(const vector<vector<vector<uint8_t>>>& colorImage);

/// <summary>
/// Converts a 1D Image back to a simple message (vector of chars).
/// </summary>
/// <param name="img">The input 1D Image.</param>
/// <returns>A vector of chars representing the simple message.</returns>
vector<char> convertImageToSimpleMessage(const Image<1>& img);

/// <summary>
/// Converts a 2D Image back to a black and white image (2D vector of uint8_t).
/// </summary>
/// <param name="img">The input 2D Image.</param>
/// <returns>A 2D vector of uint8_t representing the black and white image.</returns>
vector<vector<uint8_t>> convertImageToBlackWhite(const Image<2>& img);

/// <summary>
/// Converts a 3D Image back to a color image (3D vector of uint8_t).
/// </summary>
/// <param name="img">The input 3D Image.</param>
/// <returns>A 3D vector of uint8_t representing the color image.</returns>
vector<vector<vector<uint8_t>>> convertImageToColor(const Image<3>& img);
