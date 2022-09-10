#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "image.h"

/*
 * load image from file path to Image object using stbi_load()
 */
Image loadImage(const std::string& path)
{
	int w, h, c, channels;
	unsigned char* data = stbi_load(path.c_str(), &w, &h, &c, channels);
	if (!data)
	{
		std::cerr << "Error: stb_image cannot load file using stbi_load(): "
				  << path
				  << stbi_failure_reason()
				  << std::endl;
		exit(0);
	}
	// todo: check channels == 3
	// todo: check image w, h != 0

	Image img{ w, h, c, std::vector<Pixel>(w * h) };

	for (int i = 0; i < w; ++i)
	{
		for (int j = 0; j < h; ++j)
		{
			unsigned char* pixelOffset = data + ((i + w * j) * 3);
			img.pixels[j * w + i].r = static_cast<uint8_t>(pixelOffset[0]);
			img.pixels[j * w + i].g = static_cast<uint8_t>(pixelOffset[1]);
			img.pixels[j * w + i].b = static_cast<uint8_t>(pixelOffset[2]);
			// todo: work with a too
		}
	}

	return img;
}

/*
 * Convert Image to grayscale image using BT.601
 */
Image toGrayscale(const Image& image)
{
	// we use channel == 1 because we want only gray image without color channels
	Image grayScale{ image.width, image.height, 1, std::vector<Pixel>(image.width * image.height) };

	int c = 0;
	for (auto& i : image.pixels)
	{
		grayScale.pixels[c].r =
			(0.2989 * static_cast<double>(i.r)
				+ 0.5870 * static_cast<double>(i.g)
				+ 0.1140 * static_cast<double>(i.b));
		c += 1;
	}
	return grayScale;
}

/*
 * Save image to file using stbi_write_jpg()
 */
/*
 * void saveImage(const Image& image)
{
	int w = image.width;
	int h = image.height;
	int c = image.channel;

	unsigned char data[w * h * c];

	int index = 0;

	for (auto& i : image.pixels)
	{
		data[index++] = static_cast<unsigned char>(i.r);
	}

	stbi_write_jpg("test.jpeg", w, h, c, data, w * c);
}
 */

AsciiArt toAscii(const Image& image, int cellSize, std::string& characters)
{

	int w = image.width;
	int h = image.height;

	int yCellHeight = std::floor(cellSize * 2.1); // ratio

	std::vector<Cell> pixelsBrg;

	// new width and height our art <= image w, h
	int nw = w / cellSize;
	if ((nw % w) != 0)
	{
		nw += 1;
	}
	int nh = h / cellSize;
	if ((nh % w) != 0)
	{
		nw += 1;
	}

	for (int y = 0; y < h; y += cellSize)
	{
		for (int x = 0; x < w; x += cellSize)
		{
			auto cell = processCell(image, x, y, std::min(w - x, cellSize), std::min(h - y, yCellHeight));
			pixelsBrg.push_back(cell);

		}
	}
	AsciiArt ascii{ nw, nh, cellSize, characters, pixelsBrg };
	return ascii;
}

/*
 *  divide image into "cells" of pixel (ex: 5 * 5)
 *  and count (average brightness of cell) = (brightness of pixel) / (pixels in cell) number from [0...255]
 */
Cell processCell(const Image& image, int x, int y, int width, int height)
{
	int avgBr = 0;
	int pixelCount = 0;
	for (int i = y; i < y + height; ++i)
	{
		for (int j = x; j < x + width; ++j)
		{
			int posX = j * 1;
			int posY = i * 1;
			int pos = (posY * image.width) + posX;
			avgBr += static_cast<int>(image.pixels[pos].r);
			pixelCount += 1;
		}
	}

	Cell cell{ x, y, avgBr / pixelCount };

	return cell;
}

std::vector<char> mapToChar(const AsciiArt& ascii)
{
	std::vector<char> res(ascii.width * ascii.height);
	int ix = 0;
	int length = ascii.characters.length();
	for (auto& i : ascii.pixelsArt)
	{
		auto idx = (int)std::round((length - 1) * i.brightness / 255);
		res[ix] = ascii.characters[idx];
		ix += 1;
	}

	return res;
}


