#pragma once
#include <vector>
class ObjFileToVertice
{
public:
	ObjFileToVertice();
	~ObjFileToVertice();
	std::vector<float> ReadVectors(char *fileDir);
};

