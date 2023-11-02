#pragma warning(disable: 4996)
#include "PullOfDominoKnuckles.hpp"
#include"GameController.hpp"//

PullOfDominoKnuckles::PullOfDominoKnuckles(short int XStart, short int YStart, short int XEnd, short int YEnd, Rotation rotation) : GameObject(XStart, YStart, XEnd, YEnd, rotation)
{
	SetPositionEnd(GameController::GetConsoleSize());
	CreatePull();
}
PullOfDominoKnuckles::PullOfDominoKnuckles(COORD positionStart, COORD positionEnd, Rotation rotation) : GameObject(positionStart, positionEnd, rotation)
{
	SetPositionEnd(GameController::GetConsoleSize());
	CreatePull();
}

std::vector<KnuckleOfDomino*> PullOfDominoKnuckles::CreatePull(const char* cstr)
{
	FILE* fin = fopen(cstr, "r");
	if (fin == NULL)
		numberOfRemainingKnuckles = GetMaxCountOfKnuckles();
	else
	{
		fscanf(fin, "%hu", &numberOfRemainingKnuckles);
		fclose(fin);
	}
	for (int i = 0; i < numberOfRemainingKnuckles; i++)
		knuckles.push_back(new KnuckleOfDomino);
	for (int i = 0,k = 0; i < 7; i++)
	{
		for (int j = i; j < 7; j++,k++)
		{
			knuckles[k]->SetCountOfDotsOnBothPart(i, j);
		}
	}
	return knuckles;
}


std::vector<KnuckleOfDomino*> PullOfDominoKnuckles::GetHandOfKnuckels(unsigned int handSize)
{
	srand(time(NULL));
	std::vector<KnuckleOfDomino*>hand;
	for (unsigned int i = 0; i < handSize && numberOfRemainingKnuckles>0; i++)
	{
		unsigned int randomKnuckle = rand() % numberOfRemainingKnuckles;
		hand.push_back(knuckles[randomKnuckle]);
		knuckles.erase(knuckles.begin() + randomKnuckle);
		numberOfRemainingKnuckles--;
	}
	return hand;
}

KnuckleOfDomino* PullOfDominoKnuckles::GetKnuckle(void)
{
	if (numberOfRemainingKnuckles)
	{
		KnuckleOfDomino* knuckle;
		srand(time(NULL));
		unsigned int randomKnuckle = rand() % numberOfRemainingKnuckles;
		knuckle = knuckles[randomKnuckle];
		knuckles.erase(knuckles.begin() + randomKnuckle);
		numberOfRemainingKnuckles--;
		return knuckle;
	}
	return nullptr;
}


unsigned int PullOfDominoKnuckles::GetNumberOfRemainingKnuckles(void) const
{
	return numberOfRemainingKnuckles;
}


char** PullOfDominoKnuckles::GetObjectImage(void) const
{
	unsigned int countOfKnucklesInOneRows = GameController::GetConsoleSize().X / (KnuckleOfDomino::GetKnuckleImageCols() + 3);
	unsigned int knuckleRows = (unsigned int)ceil(((double)numberOfRemainingKnuckles) / countOfKnucklesInOneRows);
	//pull is empty
	if (knuckleRows == 0)
	{
		char** pullImage = new char* [(objectPositionEnd.Y/2+1)];
		*pullImage = new char[(objectPositionEnd.X+1)* (objectPositionEnd.Y / 2 + 1) +1]{ '\0' };
		memset(*pullImage, ' ', ((objectPositionEnd.X + 1) * (objectPositionEnd.Y / 2 + 1)-1) *sizeof(char));
		for (int i = 1; i < (objectPositionEnd.Y / 2 + 1); i++)
		{
			pullImage[i] = *pullImage + i * (GameController::GetConsoleSize().X+1);
		}
		for (unsigned int i = 0; i < strlen("NOTHING"); i++)
		{
			pullImage[objectPositionEnd.Y / 2][(objectPositionEnd.X - strlen("NOTHING")) / 2 + i] = "NOTHING"[i];
		}
		for (unsigned int i = 0; i < strlen("Pull is empty"); i++)
		{
			pullImage[objectPositionEnd.Y / 4][(objectPositionEnd.X - strlen("Pull is empty")) / 2 + i] = "Pull is empty"[i];
		}
		return pullImage;
	}
	//

	unsigned int pullImageRows = knuckleRows * KnuckleOfDomino::GetKnuckleImageRows() + (knuckleRows - 1) * 3;
	unsigned int pullImageCols = GameController::GetConsoleSize().X;
	char** pullImage = new char* [pullImageRows];
	*pullImage = new char[pullImageRows * pullImageCols + 1]{ '\0' };
	for (unsigned int i = 1; i < pullImageRows; i++)
	{
		pullImage[i] = *pullImage + i * pullImageCols;
	}
	memset(*pullImage, ' ', pullImageRows * pullImageCols * sizeof(char));
	for (int i = 0; i < numberOfRemainingKnuckles; i++)
	{
		int j = (i / countOfKnucklesInOneRows) * (KnuckleOfDomino::GetKnuckleImageRows() + 3);
		char** knuckleImage = knuckles[i]->GetObjectImage();
		for (int jKnuckle = 0; jKnuckle <KnuckleOfDomino::GetKnuckleImageRows(); j++, jKnuckle++)
		{
			int g = (i % countOfKnucklesInOneRows) * (KnuckleOfDomino::GetKnuckleImageCols() + 3);
			for (int gKnuckle = 0; gKnuckle < KnuckleOfDomino::GetKnuckleImageCols(); g++, gKnuckle++)
			{
				pullImage[j][g] = knuckleImage[jKnuckle][gKnuckle];
			}
		}
	}
	return pullImage;
}


PullOfDominoKnuckles::~PullOfDominoKnuckles()
{
	for (int i = knuckles.size(); i > 0; i--)
	{
		delete knuckles[i - 1];
		knuckles.pop_back();
	}
}