#include "Settings.h"
#include <fstream>
#include <string>
#include <algorithm>

Settings::Settings()
{
	std::ifstream inFile{ "resources/settings.cfg" };
	std::string line, identifier, value;

	while (std::getline(inFile, line))
	{
		auto index = std::find(line.cbegin(), line.cend(), '=');
		identifier = std::string{ line.cbegin(), index };
		value = std::string{ index + 1, line.cend() };

		if (identifier == "width")
		{
			width = std::stoi(value);
		}
		else if (identifier == "height")
		{
			height = std::stoi(value);
		}
		else if (identifier == "fov")
		{
			fov = std::stoi(value);
		}
		else if (identifier == "sensitivity")
		{
			sensitivity = std::stof(value);
		}
		else if (identifier == "view-distance")
		{
			viewDistance = std::stof(value);
		}
	}
}
