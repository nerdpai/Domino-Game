#include "Table.hpp"
#include"GameController.hpp"//

Table::Table(short int XStart, short int YStart, short int XEnd, short int YEnd, Rotation rotation) : GameObject(XStart, YStart, XEnd, YEnd, rotation)
{
	Initialization();
}
Table::Table(COORD positionStart, COORD positionEnd, Rotation rotation) : GameObject(positionStart, positionEnd, rotation)
{
	Initialization();
}


bool Table::GetIsFirstKnuckleSet(void)
{
	return isFirstKnuckleSet;
}
bool Table::SetIsFirstKnuckleSet(bool isSet)
{
	if (tableImage[(objectPositionEnd.Y - objectPositionStart.Y) / 2][(objectPositionEnd.X - objectPositionStart.X) / 2] != ' ' && tableImage[(objectPositionEnd.Y - objectPositionStart.Y) / 2][(objectPositionEnd.X - objectPositionStart.X) / 2 + 2] != ' ' && tableImage[(objectPositionEnd.Y - objectPositionStart.Y) / 2][(objectPositionEnd.X - objectPositionStart.X) / 2 + 1] == '|')
	{
		return isFirstKnuckleSet = isSet;
	}
	return isFirstKnuckleSet;
}

void Table::SetFirstKnuckle(const KnuckleOfDomino* knuckle)
{
	if (knuckle == nullptr)
	{
		tableImage[(objectPositionEnd.Y - objectPositionStart.Y) / 2][(objectPositionEnd.X - objectPositionStart.X) / 2] = tableImage[(objectPositionEnd.Y - objectPositionStart.Y) / 2][(objectPositionEnd.X - objectPositionStart.X) / 2 + 2] = tableImage[(objectPositionEnd.Y - objectPositionStart.Y) / 2][(objectPositionEnd.X - objectPositionStart.X) / 2 + 1] = ' ';
		return;
	}
	tableImage[(objectPositionEnd.Y - objectPositionStart.Y) / 2][(objectPositionEnd.X - objectPositionStart.X) / 2] = knuckle->GetCountOfDotsOnTopPart()+'0';
	tableImage[(objectPositionEnd.Y - objectPositionStart.Y) / 2][(objectPositionEnd.X - objectPositionStart.X) / 2 + 2] = knuckle->GetCountOfDotsOnBottomPart()+'0';
	tableImage[(objectPositionEnd.Y - objectPositionStart.Y) / 2][(objectPositionEnd.X - objectPositionStart.X) / 2 + 1] = '|';
}


unsigned int Table::GetExtraPoints(void)
{
	unsigned int points = extraPoints;
	extraPoints = 0;
	return points;
}




void Table::Initialization(void)
{
	extraPoints = 0;

	isFirstKnuckleSet = false;

	COORD console = GameController::GetConsoleSize();
	SetPositionStart(KnuckleOfDomino::GetKnuckleImageRows() + SPACE + 2, KnuckleOfDomino::GetKnuckleImageRows() + SPACE + 2);
	SetPositionEnd(console.X - (KnuckleOfDomino::GetKnuckleImageRows() + SPACE + 2), console.Y - (KnuckleOfDomino::GetKnuckleImageRows() + SPACE + 2) - (int)(console.Y*HOW_MANY_PERCENT_FOR_INFORMATION));
	tableImageRows = objectPositionEnd.Y - objectPositionStart.Y + 1, tableImageCols = objectPositionEnd.X - objectPositionStart.X + 1;
	tableImage = new char* [tableImageRows];
	*tableImage = new char[tableImageCols * tableImageRows + 1]{'\0'};
	memset(*tableImage, ' ', tableImageCols * tableImageRows * sizeof(char));
	for (int i = 1; i < tableImageRows; i++)
	{
		tableImage[i] = *tableImage + i * tableImageCols;
	}

	for (int i = objectPositionStart.Y, iIm = 0; i <= objectPositionEnd.Y; i++, iIm++)
	{
		if (i == objectPositionStart.Y || i == objectPositionEnd.Y)
		{
			for (int j = objectPositionStart.X, jIm = 0; j <= objectPositionEnd.X; j++, jIm++)
				tableImage[iIm][jIm] = '#';
			continue;
		}
		tableImage[iIm][0] = tableImage[iIm][tableImageCols-1] = '#';
		if (i == objectPositionStart.Y + 4 || i == objectPositionEnd.Y - 4)
		{
			for (int j = objectPositionStart.X+4, jIm = 4; j <= objectPositionEnd.X-4; j++, jIm++)
				tableImage[iIm][jIm] = '+';
			continue;
		}
		else if (i > objectPositionStart.Y + 4 && i < objectPositionEnd.Y - 4)
		{
			tableImage[iIm][4] = tableImage[iIm][tableImageCols - 1-4] = '+';
		}
	}
	lastMoveEndPosition[0].X = (objectPositionEnd.X - objectPositionStart.X) / 2;// + objectPositionStart.X;
	lastMoveEndPosition[0].Y = (objectPositionEnd.Y - objectPositionStart.Y) / 2;// + objectPositionStart.Y;
	lastMoveEndPosition[1] = lastMoveEndPosition[0];
	lastMoveEndPosition[1].X += 2;

	/*tableImage[(objectPositionEnd.Y - objectPositionStart.Y) / 2][(objectPositionEnd.X - objectPositionStart.X) / 2] = tableImage[(objectPositionEnd.Y - objectPositionStart.Y) / 2][(objectPositionEnd.X - objectPositionStart.X) / 2 + 2] = '6';
	tableImage[(objectPositionEnd.Y - objectPositionStart.Y) / 2][(objectPositionEnd.X - objectPositionStart.X) / 2 + 1] = '|';*/
}

char** Table::GetObjectImage(void) const
{
	//table image + rewrite on it mini knuckle
	char** tableImageTemp = new char* [tableImageRows];
	*tableImageTemp = new char[tableImageRows * tableImageCols + 1]{'\0'};
	for (int i = 1; i < tableImageRows; i++)
	{
		tableImageTemp[i] = *tableImageTemp + i * tableImageCols;
	}
	memcpy(*tableImageTemp, *tableImage, tableImageRows * tableImageCols * sizeof(char));

	char** miniKnuckleImage = miniKnuckle.GetObjectImage();

	for (int iIm = miniKnuckle.GetPositionStart().Y - objectPositionStart.Y, i = 0; i < MINI_KNUCKLE_ROWS_COUNT; i++, iIm++)
	{
		for (int jIm = miniKnuckle.GetPositionStart().X - objectPositionStart.X, j = 0; j < MINI_KNUCKLE_COLS_COUNT; j++, jIm++)
		{
			if (miniKnuckleImage[i][j] == '\0')
				continue;
			tableImageTemp[iIm][jIm] = miniKnuckleImage[i][j];
		}
	}
	//

	delete[] * miniKnuckleImage;
	delete[] miniKnuckleImage;

	return tableImageTemp;
}


void Table::KnuckleMove(Move move)
{
	if ((int)move / 2 == 0)
	{
		COORD knuckleStart = miniKnuckle.GetPositionStart();
		if (move == UP)
		{
			if (knuckleStart.Y - 1 != objectPositionStart.Y)
			{
				bool space = true;
				for (int i = 0; i < MINI_KNUCKLE_COLS_COUNT && space; i++)
				{
					char c = tableImage[knuckleStart.Y - objectPositionStart.Y - 1][knuckleStart.X - objectPositionStart.X+i];
					space = c == ' ' || c == '+';
				}
				if (space)
				{
					miniKnuckle.SetPositionStart(knuckleStart.X, knuckleStart.Y - 1);
				}
			}
		}
		else if (move == LEFT)
		{
			if (knuckleStart.X - 1 != objectPositionStart.X)
			{
				bool space = true;
				for (int i = 0; i < MINI_KNUCKLE_COLS_COUNT && space; i++)
				{
					char c = tableImage[knuckleStart.Y - objectPositionStart.Y + i][knuckleStart.X - objectPositionStart.X - 1];
					space = c == ' ' || c == '+';
				}
				if (space)
				{
					miniKnuckle.SetPositionStart(knuckleStart.X-1, knuckleStart.Y);
				}
			}
		}
	}
	else if ((int)move / 2 == 1)
	{
		COORD knuckleEnd = miniKnuckle.GetPositionEnd();
		if (move == DOWN)
		{
			if (knuckleEnd.Y + 1 != objectPositionEnd.Y)
			{
				bool space = true;
				for (int i = 0; i < MINI_KNUCKLE_COLS_COUNT && space; i++)
				{
					char c = tableImage[knuckleEnd.Y - objectPositionStart.Y + 1][knuckleEnd.X - objectPositionStart.X - i];
					space = c == ' ' || c == '+';
				}
				if (space)
				{
					miniKnuckle.SetPositionEnd(knuckleEnd.X, knuckleEnd.Y + 1);
				}
			}
		}
		else if (move == RIGHT)
		{
			if (knuckleEnd.X + 1 != objectPositionEnd.X)
			{
				bool space = true;
				for (int i = 0; i < MINI_KNUCKLE_COLS_COUNT && space; i++)
				{
					char c = tableImage[knuckleEnd.Y - objectPositionStart.Y - i][knuckleEnd.X - objectPositionStart.X + 1];
					space = c == ' ' || c == '+';
				}
				if (space)
				{
					miniKnuckle.SetPositionEnd(knuckleEnd.X + 1, knuckleEnd.Y);
				}
			}
		}
	}
	else if(move == ROTATE)
	{
		miniKnuckle.SetRotation(Rotation((int)miniKnuckle.GetRotation() + 1));
	}
}


void Table::KnuckleRefresh(void)
{
	miniKnuckle.RefreshMiniKnuckle(objectPositionStart.X + (objectPositionEnd.X - objectPositionStart.X) / 2 - 2, objectPositionEnd.Y - MINI_KNUCKLE_ROWS_COUNT);
}


const KnuckleOfDomino* Table::SetKnuckleParts(const KnuckleOfDomino* knuckle)
{
	return miniKnuckle.SetMiniKnuckleParts(knuckle);
}


unsigned short Table::AddKnuckleToTableImage(const KnuckleOfDomino* knuckle)
// 0 - any of two, 1 - one new pos, 2 - winner
{
	if (miniKnuckle.GetPositionStart().Y >= objectPositionStart.Y + 5 && miniKnuckle.GetPositionStart().X >= objectPositionStart.X + 5 && miniKnuckle.GetPositionEnd().Y <= objectPositionEnd.Y - 5 && miniKnuckle.GetPositionEnd().X <= objectPositionEnd.X - 5)
	{
		char** tableImageTemp = GetObjectImage();
		//trash search
		/*for (int i = 0; i < 2; i++)
		{
			bool trash = false;
			char c[4] = { tableImageTemp[lastMoveEndPosition[i].Y - 1][lastMoveEndPosition[i].X - 1], tableImageTemp[lastMoveEndPosition[i].Y - 1][lastMoveEndPosition[i].X + 1], tableImageTemp[lastMoveEndPosition[i].Y + 1][lastMoveEndPosition[i].X - 1], tableImageTemp[lastMoveEndPosition[i].Y + 1][lastMoveEndPosition[i].X + 1] };

			for (int j = 0; j < 4 && !trash; j++)
				trash = c[j] != ' ' && c[j] != '+';
			if (trash)
			{
				delete[] * tableImageTemp;
				delete[] tableImageTemp;
				return 0;
			}
		}*/


		//if one part == second part, rewrite image
		unsigned short network = 0;
		for (int i = 0; i < 2; i++)
		{
			bool plus = false;
			char main = tableImageTemp[lastMoveEndPosition[i].Y][lastMoveEndPosition[i].X];
			if (main == tableImageTemp[lastMoveEndPosition[i].Y-1][lastMoveEndPosition[i].X])
			{
				network++;
				plus = true;
				lastMoveEndPosition[i] = miniKnuckle.GetPositionStart();
				lastMoveEndPosition[i].X++;
			}
			else if (main == tableImageTemp[lastMoveEndPosition[i].Y + 1][lastMoveEndPosition[i].X])
			{
				network++;
				plus = true;
				lastMoveEndPosition[i] = miniKnuckle.GetPositionEnd();
				lastMoveEndPosition[i].X--;
			}
			else if (main == tableImageTemp[lastMoveEndPosition[i].Y][lastMoveEndPosition[i].X+1])
			{
				network++;
				plus = true;
				lastMoveEndPosition[i] = miniKnuckle.GetPositionEnd();
				lastMoveEndPosition[i].Y--;
			}
			else if (main == tableImageTemp[lastMoveEndPosition[i].Y][lastMoveEndPosition[i].X-1])
			{
				network++;
				plus = true;
				lastMoveEndPosition[i] = miniKnuckle.GetPositionStart();
				lastMoveEndPosition[i].Y++;
			}
			if (plus)
			{
				lastMoveEndPosition[i].X -= objectPositionStart.X;
				lastMoveEndPosition[i].Y -= objectPositionStart.Y;
			}
		}

		if (network > 0)
		{
			delete[] * tableImage;
			*tableImage = *tableImageTemp;
			for (int i = 1; i < tableImageRows; i++)
				tableImage[i] = *tableImage + i * tableImageCols;
		}
		else
		{
			delete[] * tableImageTemp;
			delete[] tableImageTemp;
		}
		//
		//get external points
		if (network == 1 && knuckle!=nullptr)
		{
			unsigned int extraPoints = 0;
			if (knuckle->GetCountOfDotsOnBottomPart() == knuckle->GetCountOfDotsOnTopPart())
			{
				extraPoints = knuckle->GetCountOfDotsOnBothPart() + ((tableImageTemp[lastMoveEndPosition[0].Y][lastMoveEndPosition[0].X] != (knuckle->GetCountOfDotsOnTopPart()+'0'))? tableImageTemp[lastMoveEndPosition[0].Y][lastMoveEndPosition[0].X] -'0' : tableImageTemp[lastMoveEndPosition[1].Y][lastMoveEndPosition[1].X]-'0');
			}
			else
			{
				extraPoints = tableImageTemp[lastMoveEndPosition[0].Y][lastMoveEndPosition[0].X] - '0' + tableImageTemp[lastMoveEndPosition[1].Y][lastMoveEndPosition[1].X] - '0';
			}

			if (extraPoints % 5 == 0)
			{
				this->extraPoints = extraPoints;
			}
		}
		//

		return network;
	}
	return 0;
}