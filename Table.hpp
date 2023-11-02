#ifndef TABLE_HPP
#define TABLE_HPP

enum Move
{
	UP = 0,
	LEFT = 1,
	DOWN = 2,
	RIGHT = 3,
	ROTATE = 4
};

#include"GameObject.hpp"
#include"Hand.hpp"
#include"MiniKnuckle.hpp"

class Table : public GameObject
{
public:
	Table(short int XStart = 0, short int YStart = 0, short int XEnd = 0, short int YEnd = 0, Rotation rotation = NORMAL);
	Table(COORD positionStart, COORD positionEnd, Rotation rotation = NORMAL);

	void KnuckleMove(Move move);
	void KnuckleRefresh(void);

	const KnuckleOfDomino* SetKnuckleParts(const KnuckleOfDomino* knuckle);

	unsigned short AddKnuckleToTableImage(const KnuckleOfDomino* knuckle = nullptr);

	//void Initialization(void);

	char** GetObjectImage(void) const override;

	bool GetIsFirstKnuckleSet(void);
	bool SetIsFirstKnuckleSet(bool isSet);
	void SetFirstKnuckle(const KnuckleOfDomino* knuckle = nullptr);

	unsigned int GetExtraPoints(void);

private:
	bool isFirstKnuckleSet;

	void Initialization(void);
	char** tableImage;
	int tableImageRows;
	int tableImageCols;
	COORD lastMoveEndPosition[2];
	MiniKnuckle miniKnuckle;
	unsigned int extraPoints;


};



#endif

