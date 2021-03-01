#include "TMshReader.h"

#include <fstream>
#include <iostream>

#include "Rendering/Geometry.h"

Geometry* TMshReader::geometryFromTMsh(std::string filename)
{
	return geometryFromTMsh(filename, 1.0f);
}

Geometry* TMshReader::geometryFromTMsh(std::string filename, float scale)
{
	// string path = "data/models/";
	std::string path = "";
	path.append(filename);
	// path.append(".tmsh");

	std::ifstream is(path);

	if (!is.is_open())
	{
		std::cout << "error reading file : " << path << std::endl;
		return NULL;
	}

	// Mandatory attributes:

	// name
	std::string str;
	while (str != "object")
		is >> str;
	std::string name;
	is >> name;

	// trans
	is >> str;
	if (str == "matrix")
	{
		// read matrix code, its skiped
		is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		is >> str;
	}

	// vertex count
	int vertexCount;
	is >> vertexCount;

	// indices count
	is >> str;
	int indicesCount;
	is >> indicesCount;

	// optional attributes

	bool dataTypes = true; // flag for the while cycle

	// attribute counter
	int positionSize = 0, normalSize = 0, uvCount = 0, colorSize = 0, colorCount = 0;
	bool weights = false;

	while (dataTypes)
	{
		is >> str;
		if (str == "position")
		{
			is >> positionSize;
		}
		else if (str == "normal")
		{
			is >> normalSize;
		}
		else if (str == "uv")
		{
			is >> str; // skip uv layer name
			int uvSize;
			is >> uvSize;
			uvCount += 1;
		}
		else if (str == "color")
		{
			is >> str; // skip color layer name
			int colorSize;
			is >> colorSize;
			colorCount += 1;
		}
		else if (str == "groupWeight")
		{
			is >> str;
			weights = true;
		}
		else
		{
			dataTypes = false;
		}
	}

	// vertices
	int vertexSize = positionSize + normalSize + uvCount * 2 + colorCount * colorSize;
	if (weights)
		vertexSize += 8;

	/*
	cout << "position size : " << positionSize << std::endl;
	cout << "normal size : " << normalSize << std::endl;
	cout << "uvCount : " << uvCount << std::endl;
	cout << "colorCount : " << colorCount << std::endl;
	cout << "colorSize : " << colorSize << std::endl;
	cout << "vertexSize : " << vertexSize << std::endl;
	*/

	// read vertices
	int verticesArraySize = vertexCount * vertexSize;
	float* vertices = new float[verticesArraySize];

	for (int i = 0; i < verticesArraySize; i++)
	{
		is >> vertices[i];
	}

	// scale the read vertices coordinates
	for (int i = 0; i < verticesArraySize; i += vertexSize)
	{
		vertices[i] *= scale;
		vertices[i + 1] *= scale;
		vertices[i + 2] *= scale;
	}

	// read indices
	is >> str;
	unsigned int* indices = new unsigned int[indicesCount];
	for (int i = 0; i < indicesCount; i++)
	{
		is >> indices[i];
	}

	is.close();

	return new Geometry(vertices, indices, vertexCount, indicesCount, vertexSize);
}
