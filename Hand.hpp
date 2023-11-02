#ifndef HAND_HPP
#define HAND_HPP

#include"GameObject.hpp"
#include"KnuckleOfDomino.hpp"
#include"TextField.hpp"
#include<string>
#include<vector>

#define None -1
#define End -2
#define PLAYER_NAME_SIZE 20
#define SPACE 3
#define HAND_ARROW_LEFT 0
#define HAND_ARROW_RIGHT 1
#define HIGH 1
#define LOW 0
#define HOW_MANY_PERCENT_FOR_INFORMATION 0.15


class Hand : public GameObject
{
public:
	static unsigned short GetCountOfHands(void);

	static unsigned short SetCurrentHandId(unsigned short id);
	static unsigned short GetCurrentHandId(void);

	unsigned int GetOtherKnucklesDots(void);

	KnuckleOfDomino* SetKnuckle(KnuckleOfDomino* knuckle);
	std::vector<KnuckleOfDomino*> SetHandOfKnuckles(std::vector<KnuckleOfDomino*> knucklesInHand);

	unsigned short GetCountOfKnucklesInHand(void);

	void DeleteChosenKnuckle(void);

	static unsigned short GetMaxCountOfVisibleKnucklesInHand(void);

	Hand(short int XStart = 0, short int YStart = 0, short int XEnd = 0, short int YEnd = 0, Rotation rotation = NORMAL);
	Hand(COORD positionStart, COORD positionEnd, Rotation rotation = NORMAL);

	char** GetObjectImage(void) const override;

	const char* SetPlayerName(const TextField& player);
	const char* SetPlayerName(const char* playerName);

	const char* GetPlayerName(void);

	const char* SetPlayerMove(const char* playerName);

	unsigned int SetPlayerScore(unsigned int score);
	unsigned int GetPlayerScore(void);
	

	void RefreshHand(void);


	void SetArrowsPosition(unsigned short XStart, unsigned short YStart, unsigned short XEnd, unsigned short YEnd, bool isRight, const char* cStr);
	void SetArrowsPosition(COORD positionStart, COORD positionEnd, bool isRight, const char* cStr);

	bool SetArrowHighLight(bool isRight, bool isHighlight);


	void ResetChosenKnuckleIndex(void);

	short GetChosenKnuckleIndex(void);

	short SetChosenKnuckleIndex(short index);

	bool SetIsChosenKnuckleOnTheTable(bool isOnTheTable);

	const KnuckleOfDomino* GetChosenKnuckle(void);

	/*const char* SetPlayerName(std::string& str, unsigned int comeFromCentral);
	const char* SetPlayerName(const char* cStr, unsigned int comeFromCentral);*/

	Rotation SetRotation(Rotation rotation) override;

	unsigned int AppendExtraPoints(unsigned int appendExtraPoints);

	//COORD SetPositionStart(short int XStart = 0, short int YStart = 0) override;

	//COORD SetPositionEnd(short int XEnd = 0, short int YEnd = 0) override;

	void CleanHand(void);

	~Hand();


private:

	TextField arrows[2];//0 - left arrow		1 - right arrow
	static unsigned short countOfHands;
	TextField playerName;
	TextField moveName;
	unsigned int playerScore;
	std::vector<KnuckleOfDomino*> knucklesInHand;
	unsigned short firstVisibleKnuckleIndex;
	short chosenKnuckleIndex;
	bool isChosenKnuckleOnTable;
	unsigned short handId;
	static unsigned short currentHandId;
	static unsigned short maxCountOfVisibleKnucklesInHand;

	unsigned short SetMaxCountOfVisibleKnucklesInHand(void);

	void Initialization(void);
};


#endif
