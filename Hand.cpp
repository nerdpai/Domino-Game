#include "Hand.hpp"
#include"GameController.hpp"//


unsigned short Hand::countOfHands = 0;
unsigned short Hand::currentHandId = 0;
unsigned short Hand::maxCountOfVisibleKnucklesInHand = 0;


unsigned short Hand::SetMaxCountOfVisibleKnucklesInHand(void)
{
	COORD consoleSize = GameController::GetConsoleSize();
	unsigned short smallestAxis = (consoleSize.X < consoleSize.Y ? consoleSize.X : consoleSize.Y);
	return (unsigned short)round((smallestAxis - SPACE - (int)(smallestAxis*HOW_MANY_PERCENT_FOR_INFORMATION) - 2 * (KnuckleOfDomino::GetKnuckleImageRows()+SPACE)) / (double)(KnuckleOfDomino::GetKnuckleImageCols() + SPACE));
}


unsigned short Hand::GetCountOfHands(void)
{
	return countOfHands;
}


unsigned short Hand::SetCurrentHandId(unsigned short id)
{
	if (id >= countOfHands)
		return currentHandId = 0;
	return currentHandId = id;
}

unsigned short Hand::GetCurrentHandId(void)
{
	return currentHandId;
}



KnuckleOfDomino* Hand::SetKnuckle(KnuckleOfDomino* knuckle)
{
	if (knuckle == nullptr)
		return nullptr;
	knucklesInHand.push_back(knuckle);
	return knuckle;
}

std::vector<KnuckleOfDomino*> Hand::SetHandOfKnuckles(std::vector<KnuckleOfDomino*> knucklesInHand)
{
	return this->knucklesInHand = knucklesInHand;
}


unsigned short Hand::GetCountOfKnucklesInHand(void)
{
	return (unsigned short)knucklesInHand.size();
}


unsigned short Hand::GetMaxCountOfVisibleKnucklesInHand(void)
{
	return maxCountOfVisibleKnucklesInHand;
}



//void Hand::StartConfig(void)
//{
//	firstVisibleKnuckleIndex = 0;
//	chosenKnuckleIndex = None;
//}


Hand::Hand(short int XStart, short int YStart, short int XEnd, short int YEnd, Rotation rotation) : GameObject(XStart, YStart, XEnd, YEnd, rotation)
{
	Initialization();
}
Hand::Hand(COORD positionStart, COORD positionEnd, Rotation rotation) : GameObject(positionStart, positionEnd, rotation)
{
	Initialization();
}


void Hand::Initialization(void)
{
	if (arrows[0].GetTextSize() == 0)
	{
		arrows[HAND_ARROW_LEFT].SetText(" ##  #");
		arrows[HAND_ARROW_RIGHT].SetText("#  ## ");
	}
	if (!maxCountOfVisibleKnucklesInHand)
	{
		maxCountOfVisibleKnucklesInHand = SetMaxCountOfVisibleKnucklesInHand();
	}
	firstVisibleKnuckleIndex = 0;
	chosenKnuckleIndex = -1;//change to -1
	handId = countOfHands++;
	playerScore = 0;
	isChosenKnuckleOnTable = false;
	playerName.SetPositionStart(0, handId);
	playerName.SetPositionEnd(GameController::GetConsoleSize().X, handId);
}


char** Hand::GetObjectImage(void) const
{
	unsigned short handRowsImage = objectPositionEnd.Y - objectPositionStart.Y+1, handColsImage = objectPositionEnd.X - objectPositionStart.X+1;
	if (!handColsImage || !handRowsImage)
		return NULL;
	char** handImage = new char* [handRowsImage]{NULL};
	*handImage = new char[handRowsImage * handColsImage + 1]{'\0'};
	memset(*handImage, ' ', handRowsImage * handColsImage*sizeof(char));
	for (int i = 1; i < handRowsImage; i++)
	{
		handImage[i] = *handImage + i * handColsImage;
	}


	short rest = (knucklesInHand.size() < maxCountOfVisibleKnucklesInHand ? maxCountOfVisibleKnucklesInHand - knucklesInHand.size() : 0), singleOut = KnuckleOfDomino::GetKnuckleImageRows()/3;

	//if it isn't your turn
	if (handId != currentHandId)
	{
		if (objectRotation == NORMAL || objectRotation == FLIPPED)
		{
			for (unsigned short i = 0; i < maxCountOfVisibleKnucklesInHand-rest; i++)
			{
				for (/*unsigned*/ int j = (objectRotation == NORMAL ? singleOut : 0), jIm = 0; j < int(handRowsImage - (objectRotation == NORMAL ? 0 : singleOut)); j++, jIm++)
				{
					unsigned int shift = SPACE + (KnuckleOfDomino::GetKnuckleImageCols() + SPACE)*rest/2 + i * (KnuckleOfDomino::GetKnuckleImageCols() + SPACE);
					for (unsigned int g = shift; g < shift + KnuckleOfDomino::GetKnuckleImageCols(); g++)
					{
						if (jIm != 0 && jIm != KnuckleOfDomino::GetKnuckleImageRows() - 1)
						{
							handImage[j][g] = '*';
							g += KnuckleOfDomino::GetKnuckleImageCols() - 1;
							handImage[j][g] = '*';
						}
						else
							handImage[j][g] = '*';
					}
				}
			}
		}
		else if (objectRotation == ROTATE90TOTHELEFT || objectRotation == ROTATE90TOTHERIGHT)
		{
			for (unsigned short i = 0; i < maxCountOfVisibleKnucklesInHand-rest; i++)
			{
				unsigned int shift = SPACE + (KnuckleOfDomino::GetKnuckleImageCols() + SPACE) * rest / 2 + i * (KnuckleOfDomino::GetKnuckleImageCols() + SPACE);
				for (unsigned int j = shift, jR = 0; j < shift + KnuckleOfDomino::GetKnuckleImageCols(); j++, jR++)
				{
					for (/*unsigned*/ int g = (objectRotation == ROTATE90TOTHELEFT ? 0 : singleOut); g < int(handColsImage - (objectRotation == ROTATE90TOTHELEFT ? singleOut : 0)); g++)
					{
						if (jR != 0 && jR != 4)
						{
							handImage[j][g] = '*';
							g += KnuckleOfDomino::GetKnuckleImageRows() - 1;
							handImage[j][g] = '*';
						}
						else					
							handImage[j][g] = '*';
					}
				}
			}
		}
	}
	//

	//if yout turn
	else
	{
		arrows[HAND_ARROW_LEFT].Print();
		arrows[HAND_ARROW_RIGHT].Print();
		moveName.Print();



		short back = (firstVisibleKnuckleIndex + maxCountOfVisibleKnucklesInHand - rest == knucklesInHand.size() && isChosenKnuckleOnTable&&firstVisibleKnuckleIndex>0);

		if (/*firstVisibleKnuckleIndex >= chosenKnuckleIndex && chosenKnuckleIndex <= firstVisibleKnuckleIndex + maxCountOfVisibleKnucklesInHand && */isChosenKnuckleOnTable && knucklesInHand.size() <= maxCountOfVisibleKnucklesInHand)
			rest++;

		if (objectRotation == NORMAL || objectRotation == FLIPPED)
		{
			for (unsigned short i = 0, iIm = 0; i < maxCountOfVisibleKnucklesInHand - rest; i++)
			{
				if (i + firstVisibleKnuckleIndex - back + iIm == chosenKnuckleIndex && isChosenKnuckleOnTable)
				{
					i--; iIm++;
					continue;
				}

				char** knuckleImage = knucklesInHand[i + firstVisibleKnuckleIndex - back + iIm]->GetObjectImage();

				for (/*unsigned*/ int j = singleOut - (i + firstVisibleKnuckleIndex - back + iIm == chosenKnuckleIndex) * singleOut, jIm = 0; j < int(handRowsImage - (i + firstVisibleKnuckleIndex - back + iIm == chosenKnuckleIndex) * singleOut); j++, jIm++)
				{
					unsigned int shift = SPACE + (KnuckleOfDomino::GetKnuckleImageCols() + SPACE) * rest / 2 + i * (KnuckleOfDomino::GetKnuckleImageCols() + SPACE);

					for (unsigned int g = shift, gIm = 0; g < shift + KnuckleOfDomino::GetKnuckleImageCols(); g++, gIm++)
					{
						handImage[j][g] = knuckleImage[jIm][gIm];
					}
				}

				delete[] * knuckleImage;
				delete[] knuckleImage;

			}
		}
		else if (objectRotation == ROTATE90TOTHELEFT || objectRotation == ROTATE90TOTHERIGHT)
		{
			for (unsigned short i = 0, iIm = 0; i < maxCountOfVisibleKnucklesInHand - rest; i++)
			{
				char** knuckleImage = knucklesInHand[i + firstVisibleKnuckleIndex - back + iIm]->GetObjectImage();


				if (i + firstVisibleKnuckleIndex - back + iIm == chosenKnuckleIndex && isChosenKnuckleOnTable)
				{
					i--; iIm++;
					continue;
				}
				unsigned int shift = SPACE + (KnuckleOfDomino::GetKnuckleImageCols() + SPACE) * rest / 2 + i * (KnuckleOfDomino::GetKnuckleImageCols() + SPACE);
				for (unsigned int j = shift, jIm = 0; j < shift + KnuckleOfDomino::GetKnuckleImageCols(); j++, jIm++)
				{
					for (/*unsigned*/ int g = 0 + (i + firstVisibleKnuckleIndex - back + iIm == chosenKnuckleIndex) * singleOut, gIm = 0; g < (handColsImage - singleOut + (i + firstVisibleKnuckleIndex - back + iIm == chosenKnuckleIndex) * singleOut); g++, gIm++)
					{
						handImage[j][g] = knuckleImage[gIm][jIm];
					}
				}

				delete[] * knuckleImage;
				delete[] knuckleImage;
			}
		}
	}
	
	//some changes if hand rotate to 90
	if ((objectRotation == ROTATE90TOTHERIGHT || objectRotation == FLIPPED)&& handId == currentHandId)
	{
		char** handImageTemp = new char* [handRowsImage] {NULL};
		*handImageTemp = new char[handRowsImage * handColsImage + 1]{ '\0' };
		memset(*handImageTemp, ' ', handRowsImage * handColsImage * sizeof(char));
		for (int i = 1; i < handRowsImage; i++)
		{
			handImageTemp[i] = *handImageTemp + i * handColsImage;
		}
		for (int i = handRowsImage - 1, iIm = 0; i > -1; i--, iIm++)
		{
			for (int j = handColsImage - 1, jIm = 0; j > -1; j--, jIm++)
			{
				handImageTemp[iIm][jIm] = handImage[i][j];
			}
		}
		return handImageTemp;
	}
	//

	//

	return handImage;
}


const char* Hand::SetPlayerName(const TextField& playerName)
{
	this->playerName = playerName;
	SetPlayerMove(this->playerName.GetText());
	return this->playerName.GetText();
}

const char* Hand::SetPlayerName(const char* playerName)
{
	this->playerName = playerName;
	SetPlayerMove(this->playerName.GetText());
	return this->playerName.GetText();
}


const char* Hand::SetPlayerMove(const char* playerName)
{
	std::string str = playerName;
	str = str + ", your move!";
	COORD coor,console = GameController::GetConsoleSize();
	coor.Y = console.Y-(int)(console.Y* HOW_MANY_PERCENT_FOR_INFORMATION) +3;
	coor.X = SHORT(console.X / 2 - str.size() / 2 - 1);
	this->moveName.SetPositionStart(coor);
	coor.X = GameController::GetConsoleSize().X;
	this->moveName.SetPositionEnd(coor);
	this->moveName = str.c_str();
	return this->moveName.GetText();
}


unsigned int Hand::SetPlayerScore(unsigned int score)
{
	return playerScore += score;
}


unsigned int Hand::GetPlayerScore(void)
{
	return playerScore;
}



Rotation Hand::SetRotation(Rotation rotation)
{
	if ((int)rotation == 1)
		SetPositionEnd(objectPositionEnd.Y+objectPositionStart.X-objectPositionStart.Y, objectPositionEnd.X - objectPositionStart.X+ objectPositionStart.Y);
	else if ((int)rotation == 3)
	{
		COORD posStart = objectPositionStart;
		SetPositionStart(objectPositionEnd.X - objectPositionStart.Y + objectPositionEnd.Y, objectPositionStart.Y);
		SetPositionEnd(objectPositionEnd.X, objectPositionStart.Y + objectPositionEnd.X - posStart.X);
	}
	return objectRotation = rotation;
}


unsigned int Hand::AppendExtraPoints(unsigned int appendExtraPoints)
{
	return playerScore += appendExtraPoints;
}


void Hand::SetArrowsPosition(unsigned short XStart, unsigned short YStart, unsigned short XEnd, unsigned short YEnd, bool isRight, const char* cStr)
{
	unsigned int index = (isRight?HAND_ARROW_RIGHT:HAND_ARROW_LEFT);
	arrows[index].SetPositionStart(XStart,YStart);
	arrows[index].SetPositionEnd(XEnd,YEnd);
	arrows[index].SetText(cStr);
}
void Hand::SetArrowsPosition(COORD positionStart, COORD positionEnd, bool isRight, const char* cStr)
{
	SetArrowsPosition(positionStart.X, positionStart.Y, positionEnd.X, positionEnd.Y, isRight,cStr);
}


bool Hand::SetArrowHighLight(bool isRight, bool isHighlight)
{
	unsigned short index = (isRight ? HAND_ARROW_RIGHT : HAND_ARROW_LEFT);
	return arrows[index].SetHighLight(isHighlight);
}


void Hand::ResetChosenKnuckleIndex(void)
{
	chosenKnuckleIndex = -1;
}

short Hand::GetChosenKnuckleIndex(void)
{
	return chosenKnuckleIndex;
}

short Hand::SetChosenKnuckleIndex(short index)
{
	if (index >= (short)knucklesInHand.size())
	{
		//go to first
		firstVisibleKnuckleIndex = 0;
		return chosenKnuckleIndex = 0;
		//
	}
	else if (index<0)
	{
		//go to the end
		short firstVisible = 0;
		firstVisible = short(knucklesInHand.size() - maxCountOfVisibleKnucklesInHand);
		if (firstVisible < 0)
			firstVisible = 0;
		firstVisibleKnuckleIndex = firstVisible;
		return chosenKnuckleIndex = short(knucklesInHand.size() - 1);
		//
	}
	else
	{
		// change for a one step
		if (index > chosenKnuckleIndex)
		{
			if ((index - firstVisibleKnuckleIndex) == maxCountOfVisibleKnucklesInHand)
				firstVisibleKnuckleIndex++;
			return chosenKnuckleIndex = index;
		}
		else if (index < chosenKnuckleIndex)
		{
			if (chosenKnuckleIndex == firstVisibleKnuckleIndex)
				firstVisibleKnuckleIndex--;
			return chosenKnuckleIndex = index;
		}
		//
	}
	return GetChosenKnuckleIndex();
}


unsigned int Hand::GetOtherKnucklesDots(void)
{
	unsigned int otherDots = 0;
	for (int i = 0; i < int(knucklesInHand.size()); i++)
	{
		otherDots += knucklesInHand[i]->GetCountOfDotsOnBothPart();
	}
	return otherDots;
}

void Hand::DeleteChosenKnuckle(void)
{
	delete knucklesInHand[chosenKnuckleIndex];
	knucklesInHand.erase(knucklesInHand.begin() + chosenKnuckleIndex);
	RefreshHand();
}


bool Hand::SetIsChosenKnuckleOnTheTable(bool isOnTheTable)
{
	return isChosenKnuckleOnTable = isOnTheTable;
}


const KnuckleOfDomino* Hand::GetChosenKnuckle(void)
{
	if(chosenKnuckleIndex>=0&&chosenKnuckleIndex<int(knucklesInHand.size()))
		return knucklesInHand[chosenKnuckleIndex];
	return NULL;
}



void Hand::RefreshHand(void)
{
	arrows[0].Clean();
	arrows[1].Clean();
	arrows[1].SetHighLight(arrows[0].SetHighLight(false));
	
	isChosenKnuckleOnTable = false;
	firstVisibleKnuckleIndex = 0;
	chosenKnuckleIndex = -1;
	moveName.Clean();
}


const char* Hand::GetPlayerName(void)
{
	return playerName.GetText();
}


void Hand::CleanHand(void)
{
	for (int i = knucklesInHand.size() - 1; i > -1; i--)
	{
		delete knucklesInHand[i];
		knucklesInHand.pop_back();
	}
	RefreshHand();
}


Hand::~Hand()
{
	countOfHands--;
	CleanHand();
}