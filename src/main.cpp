#include "image.h"
#include "cxxopts.hpp"

int main(int argc, char** argv)
{
	cxxopts::Options options("TinyAscii", "a simple program that convert pictures to ascii");

	options.add_options()
		("i,image", "path to image", cxxopts::value<std::string>())
		("h,help", "Type -i \"path to image\" to get ascii version of image!")
		("r,ratio", "image ratio output (double)", cxxopts::value<int>()->default_value("17"))
		;

	auto result = options.parse(argc, argv);

	if (result.count("help"))
	{
		std::cout << options.help() << std::endl;
		exit(0);
	}
	std::string filePath;
	if (result.count("image"))
	{
		filePath = result["image"].as<std::string>();
	}

	int cellSize = result["ratio"].as<int>();

	if (filePath.empty()) {
		std::cerr << "error: please write path to image! for more info run with -h" << std::endl;
		exit(0);
	}
	Image img = loadImage(filePath);
	img = toGrayscale(img);


	std::string asciiCharacters = " .:-=+*#%@";
	auto ascii = toAscii(img, cellSize, asciiCharacters);
	auto vwc = mapToChar(ascii);

	/*
	for (int i = 0; i < vwc.size(); ++i)
	{
		if ((i % ascii.width) != 0)
		{
			std::cout << vwc[i];
		}
		else
		{
			std::cout << std::endl;
		}
	}
	 */

	for (int i = 0; i < ascii.height; ++i)
	{
		for (int j = 0; j < ascii.width; ++j)
		{
			int pos = j + ascii.width * i;
			std::cout <<  vwc[pos];
		}
		std::cout << std::endl;
	}

	return 0;
}
