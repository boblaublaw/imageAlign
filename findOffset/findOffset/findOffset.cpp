// findOffset.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "stdlib.h"
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
	char *filename;
	std::string pgmVersion;
	std::vector<std::vector<int>> imgData;
public:
	pgmImage() {};
	~pgmImage(void)
	{
		//std::cerr << "dealloc " << filename << std::endl;
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
		std::cout << "name: " << filename << std::endl << "version: " << pgmVersion << " with size " << X() << "," << Y() << std::endl;
		for (std::vector<std::vector<int>>::size_type i = 0; i < imgData.size(); i++)
		{
			for (std::vector<int>::size_type j = 0; j < imgData[i].size(); j++)
			{
				std::cout << i << "," << j << ":" << imgData[i][j] << ' ';
			}
			std::cout << std::endl;
		}
	}
	pgmImage(char *f) : filename(f)
	{
		int X, Y;
		std::ifstream fin;
		char buf[MAX_CHARS_PER_LINE];

		std::cout << "reading a pgm file: " << filename << std::endl;
		fin.open(filename);
		if (!fin.good())
		{
			throw std::string("Couldn't open the input file: ") + std::string(filename);
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
				X = std::stoi(std::string(token[0]));
			}
			catch (...)
			{
				throw std::string("Failed to parse X resolution to integers: ") + std::string(token[0]);
			}
			try
			{
				Y = std::stoi(std::string(token[1]));
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
		imgData.resize(Y, std::vector<int>(X, 0));

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
			}
		}
		fin.close();
	}
	pgmImage(const pgmImage &obj)
	{
		filename = "copy";
		pgmVersion = obj.pgmVersion;
		imgData = obj.imgData;
	}
	void operator = (const pgmImage &obj)
	{
		filename = "assignment copy";
		pgmVersion = obj.pgmVersion;
		imgData = obj.imgData;
	}
	pgmImage operator - (const pgmImage &ref) {
		if ((ref.X() != X())||(ref.Y() != Y())) {
			throw "mismatched matrix sizes!";
		}
		pgmImage result;
		result.pgmVersion = pgmVersion;
		result.filename = "subtraction result";
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
	int sum (void) const
	{
		int result = 0;
		for (std::vector<std::vector<int>>::size_type i = 0; i < imgData.size(); i++)
		{
			for (std::vector<int>::size_type j = 0; j < imgData[i].size(); j++)
			{
				result += imgData[i][j];
			}
		}
		return result;
	}
	float error(void) const
	{
		return ((sum() * 1.0f) / area());
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

	}
	void trimright(void)
	{
	}
};

void usage(char* progname)
{
	std::cout << std::endl;
	std::cout << progname << " usage:" << std::endl;
	std::cout << std::endl;
	std::cout << "     " << progname << " frameA.pgm frameB.pgm" << std::endl;
	exit(EXIT_FAILURE);
}

/*
	this function shifts two images relative to eachother, crops off
	anything that isn't overlapping, and measures the resulting error
	between the two.

	B moves relative to A.
*/
float measureDiff(pgmImage &a, pgmImage &b, int x, int y)
{
	// make a deep copy before we modify anything
	pgmImage c = a;
	pgmImage d = b;

	// trim the images according to the offsets.
	// if x or y is 0 we neednt trim in that dimension.
	while (x-- > 0)
	{
		c.trimleft();
		d.trimright();
	}
	while (x++ < 0)
	{
		c.trimright();
		d.trimleft();
	}
	while (y-- > 0)
	{
		c.trimtop();
		d.trimbottom();
	}
	while (y++ < 0)
	{
		c.trimbottom();
		d.trimtop();
	}
	pgmImage e = c - d;
	return e.error();
}

/*
	this is the meat and potatoes. take two images and brute for a range
	of x,y offsets to find the offset with the least error over area.
*/
int findOffset(pgmImage &a, pgmImage &b, int x, int y)
{
	std::vector<std::vector<float>> err;
	err.resize((x*2)+1, std::vector<float>((y*2)+1, 0.0));

	for (int i = -x; i <= x; i++)
	{
		for (int j = -y; j <= y; j++)
		{
			int k = i + x;
			int l = j + y;
			err[k][l] = measureDiff(a, b, i, j);
			std::cout << i << "," << j << " = " << err[k][l] << std::endl;
		}
	}
	return EXIT_SUCCESS;
}

/*
	main just reads and validates user input:
*/
int main(int argc, char* argv[])
{
	int xRange, yRange, rv = EXIT_FAILURE;

	if (argc !=5) {
		usage(argv[0]);
	}
	
	pgmImage *A, *B;

	try
	{
		xRange = std::stoi(argv[3]);
		yRange = std::stoi(argv[4]);
		std::cout << "range of " << xRange << ',' << yRange << std::endl;
	}
	catch (...)
	{
		std::cout << "Problem parsing range values!" << std::endl;
		usage(argv[0]);
	}
	try
	{
		A = new pgmImage(argv[1]);
		B = new pgmImage(argv[2]);

		rv = findOffset(*A, *B, xRange, yRange);
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

#ifdef UNDEFINED
D = new pgmImage(*B);
C = *A;
std::cout << "A" << std::endl;
A->print();
std::cout << "B" << std::endl;
B->print();
std::cout << std::endl;

std::cout << "B - A" << std::endl;
C = *B - *A;
C.print();
std::cout << std::endl << "error of B-A" << std::endl;
std::cout << C.error() << std::endl;

A->imgData.resize(2, std::vector<int>(2, 0));
std::cout << "resized A" << std::endl;
A->print();
std::cout << "C" << std::endl;
C.print();
B->imgData.resize(2, std::vector<int>(2, 0));
std::cout << "resized B" << std::endl;
B->print();
std::cout << "D" << std::endl;
D->print();
std::cout << "A - B" << std::endl;
C = *A - *B;
C.print();
std::cout << "A" << std::endl;
A->print();
std::cout << "B" << std::endl;
B->print();
#endif

