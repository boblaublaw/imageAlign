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
public:
	char *filename;
	std::string pgmVersion;
	int X, Y;
	std::vector<std::vector<int>> imgData;

	pgmImage() {};
	pgmImage(char *f) : filename(f)
	{
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
				for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
				{
					token[n] = strtok(0, DELIMITER); // subsequent tokens

					if (!token[n])
					{
						imgData[lineNum][0] = std::stoi(token[0]);
						lineNum++;
						break; // no more tokens on this line
					}
					imgData[lineNum][n] = std::stoi(token[n]);
					//std::cout << imgData[lineNum][n] << " ";
				}
			}
			//std::cout << std::endl;
		}
		// process (print) the tokens
		for (std::vector<std::vector<int>>::size_type i = 0; i < imgData.size(); i++)
		{
			for (std::vector<int>::size_type j = 0; j < imgData[i].size(); j++)
			{
				std::cout << imgData[i][j] << ' ';
			}
			std::cout << std::endl;
		}
	}
	// pgmImage operator + (const pgmImage&);
};

void usage(char* progname)
{
	std::cout << std::endl;
	std::cout << progname << " usage:" << std::endl;
	std::cout << std::endl;
	std::cout << "     " << progname << " frameA.pgm frameB.pgm" << std::endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
	pgmImage *A, *B;

	/*
	check arguments
	*/
	if (argc !=3) {
		usage(argv[0]);
	}
	
	try
	{
		A = new pgmImage(argv[1]);
		B = new pgmImage(argv[2]);
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
	}
	exit(EXIT_SUCCESS);
}

