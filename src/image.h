#ifndef IMGASCII_SRC_IMAGE_H_
#define IMGASCII_SRC_IMAGE_H_

#include <iostream>
#include <vector>
#include <string>


/*
 *  Work with Image
 */
struct Pixel
{
	uint8_t r, g, b;
};

struct Image
{
	int width, height;
	int channel;
	std::vector<Pixel> pixels;
};

Image loadImage(const std::string& path);
Image toGrayscale(const Image& image);

//void saveImage(const Image& image);


/*
 *  Work with Ascii Art
 */
struct Cell
{
	int x, y;
	int brightness;
};

Cell processCell(const Image& image, int x, int y, int width, int height);

struct AsciiArt
{
	int width, height;
	int cellSize;
	std::string characters;
	std::vector<Cell> pixelsArt;
};

AsciiArt toAscii(const Image& image, int sellSize, std::string& characters);
std::vector<char> mapToChar(const AsciiArt& ascii);

#endif //IMGASCII_SRC_IMAGE_H_
