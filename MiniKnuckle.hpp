#ifndef MINIKNUCKLE_HPP
#define MINIKNUCKLE_HPP

#include"GameObject.hpp"
#include"KnuckleOfDomino.hpp"

#define MINI_KNUCKLE_ROWS_COUNT 3
#define MINI_KNUCKLE_COLS_COUNT 3


class MiniKnuckle : public GameObject
{
public:

	MiniKnuckle(short int XStart = 0, short int YStart = 0, short int XEnd = 0, short int YEnd = 0, Rotation rotation = NORMAL);
	MiniKnuckle(COORD positionStart, COORD positionEnd, Rotation rotation = NORMAL);

	//void Initialization(void);

	static unsigned short GetMiniKnucklesRowsCount(void);
	static unsigned short GetMiniKnucklesColsCount(void);

	char** GetObjectImage(void) const override;

	Rotation SetRotation(Rotation rotation) override;

	COORD SetPositionStart(short int XStart = 0, short int YStart = 0) override;

	COORD SetPositionEnd(short int XEnd = 0, short int YEnd = 0) override;

	const KnuckleOfDomino* SetMiniKnuckleParts(const KnuckleOfDomino* knuckleOfDomino);

	void RefreshMiniKnuckle(short int XStart = 0, short int YStart = 0);


private:
	void Initialization(void);

	char** miniKnuckleImage;
};


#endif
