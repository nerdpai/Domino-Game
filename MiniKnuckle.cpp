#include "MiniKnuckle.hpp"


MiniKnuckle::MiniKnuckle(short int XStart, short int YStart, short int XEnd, short int YEnd, Rotation rotation) : GameObject(XStart, YStart, XEnd, YEnd, rotation)
{
	Initialization();
}
MiniKnuckle::MiniKnuckle(COORD positionStart, COORD positionEnd, Rotation rotation) : GameObject(positionStart, positionEnd, rotation)
{
	Initialization();
}


void MiniKnuckle::Initialization(void)
{
	miniKnuckleImage = new char* [MINI_KNUCKLE_ROWS_COUNT];
	*miniKnuckleImage = new char[MINI_KNUCKLE_ROWS_COUNT * MINI_KNUCKLE_COLS_COUNT]{'\0'};
	for (int i = 1; i < MINI_KNUCKLE_ROWS_COUNT; i++)
		miniKnuckleImage[i] = *miniKnuckleImage + i * MINI_KNUCKLE_COLS_COUNT;
}


unsigned short MiniKnuckle::GetMiniKnucklesRowsCount(void)
{
	return MINI_KNUCKLE_ROWS_COUNT;
}
unsigned short MiniKnuckle::GetMiniKnucklesColsCount(void)
{
	return MINI_KNUCKLE_COLS_COUNT;
}


char** MiniKnuckle::GetObjectImage(void) const
{
	char** miniKnuckleImageTemp = new char* [MINI_KNUCKLE_ROWS_COUNT];
	*miniKnuckleImageTemp = new char[MINI_KNUCKLE_ROWS_COUNT * MINI_KNUCKLE_COLS_COUNT]{ '\0' };
	for (int i = 1; i < MINI_KNUCKLE_ROWS_COUNT; i++)
		miniKnuckleImageTemp[i] = *miniKnuckleImageTemp + i * MINI_KNUCKLE_COLS_COUNT;
	memcpy(*miniKnuckleImageTemp, *miniKnuckleImage, MINI_KNUCKLE_COLS_COUNT * MINI_KNUCKLE_ROWS_COUNT * sizeof(char));
	return miniKnuckleImageTemp;
}


Rotation MiniKnuckle::SetRotation(Rotation rotation)
{
	for (int i = 0; i < abs((int)rotation - (int)objectRotation);i++)
	{
		char temp = miniKnuckleImage[0][1];
		miniKnuckleImage[0][1] = miniKnuckleImage[1][0];
		miniKnuckleImage[1][0] = miniKnuckleImage[2][1];
		miniKnuckleImage[2][1] = miniKnuckleImage[1][2];
		miniKnuckleImage[1][2] = temp;
	}
	if ((int)rotation % 2 == 1)
		miniKnuckleImage[1][1] = '|';
	else
		miniKnuckleImage[1][1] = '-';

	return objectRotation = Rotation((int)rotation % 4);;
}


COORD MiniKnuckle::SetPositionStart(short int XStart, short int YStart)
{
	objectPositionEnd = { short(XStart + 2), short(YStart + 2) };
	return objectPositionStart = { XStart, YStart };

}

COORD MiniKnuckle::SetPositionEnd(short int XEnd, short int YEnd)
{
	objectPositionStart = { short(XEnd - 2), short(YEnd - 2) };
	return objectPositionEnd = { XEnd, YEnd };
}

const KnuckleOfDomino* MiniKnuckle::SetMiniKnuckleParts(const KnuckleOfDomino* knuckleOfDomino)
{
	if (knuckleOfDomino == NULL)
		return NULL;
	SetRotation(NORMAL);
	miniKnuckleImage[0][1] = '0' + (int)knuckleOfDomino->GetCountOfDotsOnTopPart();
	miniKnuckleImage[2][1] = '0' + (int)knuckleOfDomino->GetCountOfDotsOnBottomPart();
		
	return knuckleOfDomino;
}

void MiniKnuckle::RefreshMiniKnuckle(short int XStart, short int YStart)
{
	memset(*miniKnuckleImage, '\0', MINI_KNUCKLE_COLS_COUNT * MINI_KNUCKLE_ROWS_COUNT * sizeof(char));
	SetPositionStart(XStart, YStart);
}