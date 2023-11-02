#ifndef KNUCKLEOFDOMINO
#define KNUCKLEOFDOMINO

#include"GameObject.hpp"
#include"EmbeddedDomino.hpp"
#include<fstream>


class KnuckleOfDomino : public GameObject
{
public:
	static unsigned short GetCountOfKnuckles(void);

	static unsigned short GetKnuckleImageRows(void);
	static unsigned short GetKnuckleImageCols(void);
	static COORD GetKnuckleImageSize(void);

	KnuckleOfDomino(short int XStart = 0, short int YStart = 0, short int XEnd = 0, short int YEnd = 0, Rotation rotation = NORMAL, unsigned short CountOfDotsOnTopPart = 0, unsigned short CountOfDotsOnBottomPart = 0);
	KnuckleOfDomino(COORD positionStart, COORD positionEnd, Rotation rotation = NORMAL, unsigned short CountOfDotsOnTopPart = 0, unsigned short CountOfDotsOnBottomPart = 0);

	unsigned short SetCountOfDotsOnTopPart(unsigned short CountOfDotsOnTopPart = 0);
	unsigned short SetCountOfDotsOnBottomPart(unsigned short CountOfDotsOnBottomPart = 0);
	unsigned short SetCountOfDotsOnBothPart(unsigned short CountOfDotsOnTopPart = 0, unsigned short CountOfDotsOnBottomPart = 0);

	unsigned short GetCountOfDotsOnTopPart(void) const;
	unsigned short GetCountOfDotsOnBottomPart(void) const;
	unsigned short GetCountOfDotsOnBothPart(void) const;

	char** GetObjectImage(void) const override;

	~KnuckleOfDomino();

private:
	static unsigned short GetFileDominoParts_rowsOfPart(const char* cstr = GetDominoFileName());
	static unsigned short GetFileDominoParts_colsOfPart(const char* cstr = GetDominoFileName());
	static unsigned short GetFileDominoParts_imageRows(const char* cstr = GetDominoFileName());
	static unsigned short GetFileDominoParts_imageCols(const char* cstr = GetDominoFileName());
	static char** GetFileDominoParts_dominoParts(const char* cstr = GetDominoFileName());


	char** CreateObjectImage(void);
	void DeleteObjectImage(void);

	unsigned short CountOfDotsOnTopPart;
	unsigned short CountOfDotsOnBottomPart;

	char** knuckleImage;


	static unsigned short rowsOfPart;
	static unsigned short colsOfPart;

	static unsigned short imageRows;
	static unsigned short imageCols;


	static char** dominoParts;
	static unsigned short countOfKnuckles;
};



#endif

