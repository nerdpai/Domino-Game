#include"EmbeddedDomino.hpp"

char** GetDominoParts(void)
{
	char dominosTemp[7][10] = {
		{"         "},
		{"    O    "},
		{"  O   O  "},
		{"  O O O  "},
		{"O O   O O"},
		{"O O O O O"},
		{"O OO OO O"},
	};
	char** dominos = new char* [7];
	*dominos = new char[7 * 9];
	for (int i = 0; i < 7; i++)
	{
		dominos[i] = *dominos + i * 9;
		memcpy(dominos[i], dominosTemp[i], 9 * sizeof(char));
	}
	return dominos;
}

unsigned short GetDominoCols(void)
{
	return 3;
}

unsigned short GetDominoRows(void)
{
	return 3;
}

unsigned short GetMaxCountOfKnuckles(void)
{
	return 28;
}

const char* GetDominoFileName(void)
{
	return "DominoParts.txt";
}