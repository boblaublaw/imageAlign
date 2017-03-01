// findOffset.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "stdlib.h"
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

const int MAX_TOKENS_PER_LINE = 2000;
const int MAX_CHARS_PER_LINE = MAX_TOKENS_PER_LINE * 4;

const char* const DELIMITER = " \t";

class pgmImage
{
private:
	char *desc;
	std::string pgmVersion;
	std::vector<std::vector<int>> imgData;
public:
	pgmImage() {};
	~pgmImage(void)
	{
		//std::cerr << "dealloc " << desc << std::endl;
		imgData.resize(0, std::vector<int>(0));
	}
	int X(void) const
	{
		return (imgData.size());
	}
	int Y(void) const
	{
		return (imgData[0].size());
	}
	void print(void) {
		//std::cout << "name: " << desc << std::endl << "version: " << pgmVersion << " with size " << X() << "," << Y() << std::endl;
		for (std::vector<std::vector<int>>::size_type i = 0; i < imgData.size(); i++)
		{
			for (std::vector<int>::size_type j = 0; j < imgData[i].size(); j++)
			{
				//std::cout << i << "," << j << ":" << imgData[i][j] << ' ';
				std::cout << imgData[i][j] << ' ';
			}
			std::cout << std::endl;
		}
	}
	pgmImage(char *f) : desc(f)
	{
		int X, Y;
		std::ifstream fin;
		char buf[MAX_CHARS_PER_LINE];

		//std::cout << "reading a pgm file: " << desc << std::endl;
		fin.open(desc);
		if (!fin.good())
		{
			throw std::string("Couldn't open the input file: ") + std::string(desc);
		}

		// first check the file header version
		fin.getline(buf, MAX_CHARS_PER_LINE);
		pgmVersion = std::string(buf);
		if (pgmVersion != "P2")
		{
			throw std::string("No idea how to parse a PGM of version ") + pgmVersion;
		}

		// next parse the resolution
		{
			fin.getline(buf, MAX_CHARS_PER_LINE);

			// parse the line into blank-delimited tokens
			int n = 0; // a for-loop index

			// array to store memory addresses of the tokens in buf
			const char* token[MAX_TOKENS_PER_LINE] = {};

			// parse the line
			token[0] = strtok(buf, DELIMITER); // first token
			if (token[0]) // zero if line is blank
			{
				for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
				{
					token[n] = strtok(0, DELIMITER); // subsequent tokens
					if (!token[n])
					{
						break; // no more tokens
					}
				}
			}
			if (n != 2)
			{
				throw std::string("there are more than 2 tokens on the resolution line of the header!");
			}
			try
			{
				Y = std::stoi(std::string(token[0]));
			}
			catch (...)
			{
				throw std::string("Failed to parse X resolution to integers: ") + std::string(token[0]);
			}
			try
			{
				X = std::stoi(std::string(token[1]));
			}
			catch (...)
			{
				throw std::string("Failed to parse Y resolution to integers: ") + std::string(token[1]);
			}
		}

		// first check the file header version
		fin.getline(buf, MAX_CHARS_PER_LINE);
		if (std::string(buf) != "255")
		{
			throw std::string("No idea how to parse a PGM maxValue of ") + std::string(buf);
		}

		// preallocate memory for the ints
		imgData.resize(X, std::vector<int>(Y, 0));

		int lineNum = 0;
		// read each line of the file
		while (!fin.eof())
		{
			// read an entire line into memory
			fin.getline(buf, MAX_CHARS_PER_LINE);

			// parse the line into blank-delimited tokens
			int n = 0; // a for-loop index

			// array to store memory addresses of the tokens in buf
			const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0

			// parse the line
			token[0] = strtok(buf, DELIMITER); // first token

			if (token[0]) // zero if line is blank
			{
				imgData[lineNum][0] = std::stoi(token[0]);

				for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
				{
					token[n] = strtok(0, DELIMITER); // subsequent tokens

					if (!token[n])
					{
						lineNum++;
						break; // no more tokens on this line
					}
					imgData[lineNum][n] = std::stoi(token[n]);
				}
				if (n != Y)
				{
					std::cerr << "wrong number of tokens on this line! " << n << " != " << Y << std::endl;
					throw "poop";
				}
			}
		}
		if (lineNum != X)
		{
			std::cerr << "wrong number of lines in this file! " << lineNum << " != " << X << std::endl;
			throw "pee";
		}
		fin.close();
	}
	pgmImage(const pgmImage &obj)
	{
		desc = "copy";
		pgmVersion = obj.pgmVersion;
		imgData = obj.imgData;
	}
	void operator = (const pgmImage &obj)
	{
		desc = "assignment copy";
		pgmVersion = obj.pgmVersion;
		imgData = obj.imgData;
	}
	pgmImage operator - (const pgmImage &ref)
	{
		if ((ref.X() != X())||(ref.Y() != Y())) {
			throw "mismatched matrix sizes!";
		}
		pgmImage result;
		result.pgmVersion = pgmVersion;
		result.desc = "subtraction result";
		result.imgData.resize(X(), std::vector<int>(Y(), 0));
		for (std::vector<std::vector<int>>::size_type i = 0; i < imgData.size(); i++)
		{
			for (std::vector<int>::size_type j = 0; j < imgData[i].size(); j++)
			{
				result.imgData[i][j] = imgData[i][j] - ref.imgData[i][j];
			}
		}
		return result;
	}
	int area(void) const
	{
		return X() * Y();
	}
	int totalDifference(void) const
	{
		int result = 0;
		for (std::vector<std::vector<int>>::size_type i = 0; i < imgData.size(); i++)
		{
			for (std::vector<int>::size_type j = 0; j < imgData[i].size(); j++)
			{
				result += fabs(imgData[i][j]);
			}
		}
		return result;
	}
	float error(void) const
	{
		return ((totalDifference() * 1.0f) / area());
	}
	/*
	these functions trim one row or col at a time from the sides
	*/
	void trimtop(void)
	{
		imgData.erase(imgData.begin());
	}
	void trimbottom(void)
	{
		imgData.erase(imgData.end() - 1);
	}
	void trimleft(void)
	{
		for (std::vector<std::vector<int>>::size_type i = 0; i < imgData.size(); i++)
		{
			imgData[i].erase(imgData[i].begin());
		}
	}
	void trimright(void)
	{
		for (std::vector<std::vector<int>>::size_type i = 0; i < imgData.size(); i++)
		{
			imgData[i].erase(imgData[i].end() - 1);
		}
	}

	void printImage(const char *filename)
	{
		std::ofstream fout;
		fout.open(filename);
		if (!fout.good())
		{
			throw std::string("Couldn't open the output file: ") + std::string(filename);
		}
		fout << "P2" << std::endl;
		fout << Y() << " " << X() << std::endl;
		fout << "255" << std::endl;
		for (std::vector<std::vector<int>>::size_type i = 0; i < imgData.size(); i++)
		{
			for (std::vector<int>::size_type j = 0; j < imgData[i].size(); j++)
			{
				fout << imgData[i][j] << " ";
			}
			fout << std::endl;
		}
	}
};

/*
this function shifts two images relative to eachother, crops off
anything that isn't overlapping, and measures the resulting error
between the two.

B moves relative to A.
*/
void prepImages(pgmImage &a, pgmImage &b, int x, int y)
{
	// trim the images according to the offsets.
	// if x or y is 0 we neednt trim in that dimension.
	while (x > 0)
	{
		//std::cout << "posX" << std::endl;
		a.trimleft();
		b.trimright();
		x--;
	}
	while (x < 0)
	{
		//std::cout << "negX" << std::endl;
		a.trimright();
		b.trimleft();
		x++;
	}
	while (y > 0)
	{
		//std::cout << "posY" << std::endl;
		a.trimtop();
		b.trimbottom();
		y--;
	}
	while (y < 0)
	{
		//std::cout << "negY" << std::endl;
		a.trimbottom();
		b.trimtop();
		y++;
	}
#ifdef UNDEFINED
	std::cout << std::endl << x << "," << y << std::endl;
	std::cout << "left:" << std::endl;
	a.print();
	std::cout << "right:" << std::endl;
	b.print();
#endif
}

/*
	this function measures the error between two images
*/
float measureDiff(pgmImage &a, pgmImage &b, int i, int j)
{
	// make a deep copy before we modify anything
	pgmImage c = a;
	pgmImage d = b;

	prepImages(c, d, i, j);
	pgmImage e = c - d;
#ifdef UNDEFINED
	std::cout << i << "," << j << std::endl << "left:" << std::endl;
	c.print();
	std::cout << "right:" << std::endl;
	d.print();
	std::cout << "diff:" << std::endl;
	e.print();
#endif
	return e.error();
}

/*
	this is the meat and potatoes. take two images and brute for a range
	of x,y offsets to find the offset with the least error over area.
*/
int findOffset(pgmImage &a, pgmImage &b, const int xRange, const int yRange, int &rx, int &ry)
{
	std::vector<std::vector<float>> err;
	err.resize((xRange*2)+1, std::vector<float>((yRange*2)+1, 0.0));

	float re = FLT_MAX;
	for (int relX = -xRange; relX <= xRange; relX++)
	{
		for (int relY = -yRange; relY <= yRange; relY++)
		{
			int absX = relX + xRange;
			int absY = relY + yRange;
			err[absX][absY] = measureDiff(a, b, relX, relY);
			if (fabs(err[absX][absY]) < re)
			{
				re = fabs(err[absX][absY]);
				rx = relX;
				ry = relY;
			}
			//std::cout << relX << "," << relY << " = " << err[absX][absY] << std::endl << std::endl;
		}
	}
	return EXIT_SUCCESS;
}

void usage(char* progname)
{
	std::cout << std::endl;
	std::cout << progname << " usage:" << std::endl;
	std::cout << std::endl;
	std::cout << "     " << progname << " LeftInputFrame.pgm RightInputFrame.pgm maxX maxY LeftOutputFrame.pgm RightOutputFrame.pgm" << std::endl;
	exit(EXIT_FAILURE);
}

/*
	main just reads and validates user input, then calls findOffset()
*/
int main(int argc, char* argv[])
{
	int xRange, yRange, rv = EXIT_FAILURE;

	if (argc !=7) {
		usage(argv[0]);
	}
	
	pgmImage *A, *B;

	try
	{
		xRange = std::stoi(argv[3]);
		yRange = std::stoi(argv[4]);
		//std::cout << "range of " << xRange << ',' << yRange << std::endl;
	}
	catch (...)
	{
		std::cout << "Problem parsing range values!" << std::endl;
		usage(argv[0]);
	}
	try
	{
		int xOff, yOff;
		A = new pgmImage(argv[1]);
		B = new pgmImage(argv[2]);

		rv = findOffset(*A, *B, xRange, yRange, xOff, yOff);
		std::cout << xOff << " " << yOff << std::endl;

		prepImages(*A, *B, xOff, yOff);
		A->printImage(argv[5]);
		B->printImage(argv[6]);
	}
	catch (char *e)
	{
		std::cout << "An exception occurred: " << e << std::endl;
		usage(argv[0]);
	}
	catch (std::string e)
	{
		std::cout << "An exception occurred: " << e.c_str() << std::endl;
		usage(argv[0]);
	}
	catch (...)
	{
		std::cout << "An unknown exception occurred!" << std::endl;
		usage(argv[0]);
	}
	exit(rv);
}