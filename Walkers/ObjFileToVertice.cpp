#include "ObjFileToVertice.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include <string>

ObjFileToVertice::ObjFileToVertice()
{
}

ObjFileToVertice::~ObjFileToVertice()
{
}

std::vector<float> ObjFileToVertice::ReadVectors(char *fileDir)
{
	char letter = 'v';
	std::vector<float> prevFloats(0);

	std::ifstream file(fileDir);
	if (file.is_open())
	{
		std::string line;
		std::istringstream iss;

		// loop lines
		while (std::getline(file, line))
		{
			// if first character equals v (for vector)
			if (line.at(0) == letter && line.at(1) == ' ')
			{
				iss.str(line);

				std::vector<std::string> results;
				std::string result;
				std::istringstream innerIss(line);
				while(std::getline(innerIss, result, ' '))
				{
					results.push_back(result);
				}
				// HINT: {iss} inserts several elements into an STL container instead of 1 at a time

				int newContainerSize = prevFloats.size() + 3;
				std::vector<float> parsedFloats(newContainerSize);

				// copy older values into new container
				for (int i = 0; i < prevFloats.size(); i++)
				{
					parsedFloats[i] = prevFloats[i];
				}
				// copy new values in new container
				for (int i = 0; i < 3; i++)
				{
					parsedFloats[newContainerSize - (3-i)] = ::strtof(results[i+1].c_str(),0);
				}
				prevFloats.clear();
				prevFloats = parsedFloats;
			}
		}
	}

	return prevFloats;
}

