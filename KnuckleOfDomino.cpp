#include "KnuckleOfDomino.hpp"


unsigned short KnuckleOfDomino::GetCountOfKnuckles(void)
{
	return countOfKnuckles;
}

unsigned short KnuckleOfDomino::GetKnuckleImageRows(void)
{
	return imageRows;
}
unsigned short KnuckleOfDomino::GetKnuckleImageCols(void)
{
	return imageCols;
}
COORD KnuckleOfDomino::GetKnuckleImageSize(void)
{
	COORD knuckleImageSize;
	knuckleImageSize.X = imageCols;
	knuckleImageSize.Y = imageRows;
	return knuckleImageSize;
}


char** KnuckleOfDomino::dominoParts = KnuckleOfDomino::GetFileDominoParts_dominoParts();
unsigned short KnuckleOfDomino::rowsOfPart = KnuckleOfDomino::GetFileDominoParts_rowsOfPart();
unsigned short KnuckleOfDomino::colsOfPart = KnuckleOfDomino::GetFileDominoParts_colsOfPart();

unsigned short KnuckleOfDomino::imageRows = KnuckleOfDomino::GetFileDominoParts_imageRows();
unsigned short KnuckleOfDomino::imageCols = KnuckleOfDomino::GetFileDominoParts_imageCols();

unsigned short KnuckleOfDomino::countOfKnuckles = 0;



unsigned short KnuckleOfDomino::GetFileDominoParts_rowsOfPart(const char* cstr)
{
	std::ifstream fin;
	fin.open(cstr, std::fstream::in);
	if (!fin.is_open())
		return GetDominoRows();
	unsigned short rows=0;
	fin >> rows >> rows;
	fin.close();
	return rows;
}
unsigned short KnuckleOfDomino::GetFileDominoParts_colsOfPart(const char* cstr)
{
	std::ifstream fin;
	fin.open(cstr, std::fstream::in);
	if (!fin.is_open())
		return GetDominoCols();
	unsigned short cols = 0;
	fin >> cols >> cols >> cols;
	fin.close();
	return cols;
}
unsigned short KnuckleOfDomino::GetFileDominoParts_imageRows(const char* cstr)
{
	std::ifstream fin;
	fin.open(cstr, std::fstream::in);
	if (!fin.is_open())
		return GetDominoRows()*2+3;
	unsigned short rows = 0;
	fin >> rows >> rows;
	fin.close();
	return rows*2+3;
}
unsigned short KnuckleOfDomino::GetFileDominoParts_imageCols(const char* cstr)
{
	std::ifstream fin;
	fin.open(cstr, std::fstream::in);
	if (!fin.is_open())
		return GetDominoCols()+2;
	unsigned short cols = 0;
	fin >> cols >> cols >> cols;
	fin.close();
	return cols+2;
}
char** KnuckleOfDomino::GetFileDominoParts_dominoParts(const char* cstr)
{
	std::ifstream fin;
	fin.open(cstr, std::fstream::in);
	if (!fin.is_open())
	{
		return GetDominoParts();
	}
	fin >> rowsOfPart >> rowsOfPart >> colsOfPart;
	char** dominoParts = new char* [7];
	*dominoParts = new char[7 * rowsOfPart * colsOfPart]{ '\0' };
	for (int i = 0; i < 7; i++)
	{
		dominoParts[i] = *dominoParts + i * rowsOfPart * colsOfPart;
		fin.get();
		fin.read(dominoParts[i], rowsOfPart * colsOfPart);
	}
	fin.close();
	return dominoParts;
}





KnuckleOfDomino::KnuckleOfDomino(short int XStart, short int YStart, short int XEnd, short int YEnd, Rotation rotation, unsigned short CountOfDotsOnTopPart, unsigned short CountOfDotsOnBottomPart) : GameObject(XStart, YStart, XEnd, YEnd, rotation)
{
	knuckleImage = nullptr;
	SetCountOfDotsOnBothPart(CountOfDotsOnTopPart, CountOfDotsOnBottomPart);
	countOfKnuckles++;
}
KnuckleOfDomino::KnuckleOfDomino(COORD positionStart, COORD positionEnd, Rotation rotation, unsigned short CountOfDotsOnTopPart, unsigned short CountOfDotsOnBottomPart) : GameObject(positionStart, positionEnd, rotation)
{
	knuckleImage = nullptr;
	SetCountOfDotsOnBothPart(CountOfDotsOnTopPart, CountOfDotsOnBottomPart);
	countOfKnuckles++;
}


unsigned short KnuckleOfDomino::SetCountOfDotsOnTopPart(unsigned short CountOfDotsOnTopPart)
{
	this->CountOfDotsOnTopPart = CountOfDotsOnTopPart;
	if (knuckleImage != nullptr)
		DeleteObjectImage();
	CreateObjectImage();
	return this->CountOfDotsOnTopPart;
}
unsigned short KnuckleOfDomino::SetCountOfDotsOnBottomPart(unsigned short CountOfDotsOnBottomPart)
{
	this->CountOfDotsOnBottomPart = CountOfDotsOnBottomPart;
	if (knuckleImage != nullptr)
		DeleteObjectImage();
	CreateObjectImage();
	return this->CountOfDotsOnBottomPart;
}
unsigned short KnuckleOfDomino::SetCountOfDotsOnBothPart(unsigned short CountOfDotsOnTopPart, unsigned short CountOfDotsOnBottomPart)
{
	this->CountOfDotsOnTopPart = CountOfDotsOnTopPart;
	this->CountOfDotsOnBottomPart = CountOfDotsOnBottomPart;
	if (knuckleImage != nullptr)
		DeleteObjectImage();
	CreateObjectImage();
	return this->CountOfDotsOnTopPart + this->CountOfDotsOnBottomPart;
}


unsigned short KnuckleOfDomino::GetCountOfDotsOnTopPart(void) const
{
	return CountOfDotsOnTopPart;
}
unsigned short KnuckleOfDomino::GetCountOfDotsOnBottomPart(void) const
{
	return CountOfDotsOnBottomPart;
}

unsigned short KnuckleOfDomino::GetCountOfDotsOnBothPart(void) const
{
	return CountOfDotsOnBottomPart + CountOfDotsOnTopPart;
}


char** KnuckleOfDomino::GetObjectImage(void) const
{
	char** knuckleImageTemp = new char* [imageRows];
	*knuckleImageTemp = new char[imageRows * imageCols + 1]{ '\0' };
	for (int i = 1; i < imageRows; i++)
		knuckleImageTemp[i] = *knuckleImageTemp + i * imageCols;
	memcpy(*knuckleImageTemp, *knuckleImage, imageRows * imageCols * sizeof(char));

	return knuckleImageTemp;

	/*return CreateObjectImage();*/
}


char** KnuckleOfDomino::CreateObjectImage(void)
{
	//what are you wan't from me? just getereting image
	if (knuckleImage == nullptr)
	{
		knuckleImage = new char* [imageRows];
		*knuckleImage = new char[imageRows * imageCols + 1]{ '\0' };
		for (int i = 1; i < imageRows; i++)
			knuckleImage[i] = *knuckleImage + i * imageCols;
		memset(*knuckleImage, ' ', imageRows * imageCols * sizeof(char));

		for (int j = 0; j < imageCols; j++)
		{
			knuckleImage[rowsOfPart + 1][j] = knuckleImage[imageRows - 1][j] = knuckleImage[0][j] = '+';
		}
		for (int i = 0, k = rowsOfPart + 2; i < rowsOfPart; i++, k++)
		{
			knuckleImage[i + 1][0] = '+';
			knuckleImage[k][0] = '+';
			int j = 0;
			for (; j < colsOfPart; j++)
			{
				knuckleImage[i + 1][j + 1] = dominoParts[CountOfDotsOnTopPart][j + i * colsOfPart];
				knuckleImage[k][j + 1] = dominoParts[CountOfDotsOnBottomPart][j + i * colsOfPart];
			}
			knuckleImage[i + 1][j + 1] = '+';
			knuckleImage[k][j + 1] = '+';
		}
	}
	//


	char** knuckleImageTemp = new char* [imageRows];
	*knuckleImageTemp = new char[imageRows * imageCols + 1]{ '\0' };
	for (int i = 1; i < imageRows; i++)
		knuckleImageTemp[i] = *knuckleImageTemp + i * imageCols;
	memcpy(*knuckleImageTemp, *knuckleImage, imageRows * imageCols * sizeof(char));


	return knuckleImageTemp;
}


void KnuckleOfDomino::DeleteObjectImage(void)
{
	delete[] * knuckleImage;
	delete[] knuckleImage;
	knuckleImage = nullptr;
}


//legacy initialization
//unsigned short KnuckleOfDomino::GetFileDominoParts(const char* cstr)
//{
//	FILE* fin = fopen(cstr, "r");
//	if (fin == NULL)
//	{
//		rowsOfPart = GetDominoRows();
//		imageRows = rowsOfPart * 2 + 3;
//		colsOfPart = GetDominoCols();
//		imageCols = colsOfPart + 2;
//		dominoParts = GetDominoParts();
//		return 0;
//	}
//	fscanf(fin, "%*s%hu%hu", &rowsOfPart, &colsOfPart);
//	imageRows = rowsOfPart * 2 + 3;
//	imageCols = colsOfPart + 2;
//	fgetc(fin);
//	dominoParts = new char* [7];
//	*dominoParts = new char[7 * rowsOfPart * colsOfPart];
//	for (int i = 0; i < 7; i++)
//	{
//		dominoParts[i] = *dominoParts + i * rowsOfPart * colsOfPart;
//		for (int j = 0; j < rowsOfPart * colsOfPart; j++)
//		{
//			dominoParts[i][j] = fgetc(fin);
//		}
//		fgetc(fin);
//	}
//	fclose(fin);
//	return 0;
//}



KnuckleOfDomino::~KnuckleOfDomino()
{
	countOfKnuckles--;
	if (!countOfKnuckles)
	{
		delete[] * dominoParts;
		delete[] dominoParts;
		dominoParts = nullptr;
	}
	DeleteObjectImage();
}

