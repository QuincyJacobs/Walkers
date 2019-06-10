#include "ObjFileToVertice.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include <string>
#include <vector>

ObjFileToVertice::ObjFileToVertice()
{
}

ObjFileToVertice::~ObjFileToVertice()
{
}

void ObjFileToVertice::ReadVectors(char *fileDir)
{
	char letter = 'v';

	//std::string fullFileDir = fileDir;

	std::ifstream file(fileDir);
	if (file.is_open())
	{
		std::string line;
		std::istringstream iss;

		// loop lines
		while (std::getline(file, line))
		{
			// if first character equals v (for vector)
			if (line.at(0) == letter)
			{
				iss.str(line);

				// {iss} inserts several elements into an STL container instead of 1 at a time
				std::vector<std::string> results(std::istream_iterator<std::string>{iss},
					std::istream_iterator<std::string>());

				// TODO(Quincy): put results into float array
			}
		}
	}
}