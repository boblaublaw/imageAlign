// findOffset.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "stdlib.h"
#include <string>
#include <iostream>

class pgmImage
{
public:
	char *filename;
	pgmImage() {};
	pgmImage(char *f) : filename(f)
	{
		std::cout << "reading a pgm file: " << filename << std::endl;
		if (false) {
			throw "poop";
		}
	}
	// pgmImage operator + (const CVector&);
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
	pgmImage A, B;

	/*
	check arguments
	*/
	if (argc !=3) {
		usage(argv[0]);
	}
	
	try
	{
		A = pgmImage(argv[1]);
		B = pgmImage(argv[2]);
	}
	catch (char *e)
	{
		std::cout << "An exception occurred: " << e << std::endl;
		usage(argv[0]);
	}

	std::cout << "all good brah" << std::endl;
	exit(EXIT_SUCCESS);
}

