#include "TextField.hpp"



TextField::TextField(short int XStart, short int YStart, short int XEnd, short int YEnd, Rotation rotation) : GameObject(XStart, YStart, XEnd, YEnd, rotation)
{
	maxSize = UNLIMITED;
	isHighlight = false;
}
TextField::TextField(COORD positionStart, COORD positionEnd, Rotation rotation) : GameObject(positionStart, positionEnd, rotation)
{
	maxSize = UNLIMITED;
	isHighlight = false;
}


const char* TextField::SetText(const char* text)
{
	if (strlen(text) > (unsigned)maxSize)
		return NULL;
	else
		return (this->text = text).c_str();
}
const char* TextField::SetText(std::string& text)
{
	if (text.size() > (unsigned)maxSize)
		return NULL;
	else
		return (this->text = text).c_str();
}


const char* TextField::SetTextAppend(const char* text)
{
	if (maxSize == UNLIMITED)
	{
		return (this->text += text).c_str();
	}
	else if (strlen(text)+this->text.size() > (unsigned)maxSize)
		return NULL;
	else
		return (this->text = this->text + text).c_str();
}
const char* TextField::SetTextAppend(std::string& text)
{
	if (maxSize == UNLIMITED)
	{
		return (this->text += text).c_str();
	}
	else if (text.size() + this->text.size() > (unsigned)maxSize)
		return NULL;
	else
		return (this->text = this->text + text).c_str();
}

short TextField::SetMaxTextSize(short maxSize)
{
	return this->maxSize = maxSize;
}


unsigned short TextField::GetTextSize(void)
{
	return (unsigned short)text.size();
}


const char* TextField::GetText(void)
{
	return this->text.c_str();
}

short TextField::GetMaxTextSize(void)
{
	return maxSize;
}

bool TextField::SetHighLight(bool isHighlight)
{
	return this->isHighlight = isHighlight;
}



void TextField::Print(void) const
{
	if (text.size() == 0)
		return;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursor = objectPositionStart;
	if (isHighlight)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
	}
	bool escape = false;
	for (int iCursor = objectPositionStart.Y, i = 0; iCursor <= objectPositionEnd.Y; iCursor++, i++)
	{
		cursor.Y = iCursor;
		SetConsoleCursorPosition(hStdout, cursor);
		for (int j = 0/*objectPositionStart.X*/; j <= objectPositionEnd.X - objectPositionStart.X /*objectPositionEnd.X*/; j++)
		{
			unsigned short index = i * (objectPositionEnd.X - objectPositionStart.X+1) + j;
			if (text[index] == '\n')
				break;
			if (text[index] == '\0')
			{
				escape = true;
				break;
			}
			/*if (text[index] == ' ' && isHighlight == true)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COMMON_LVB_REVERSE_VIDEO);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
				putchar(text[index]);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
				continue;
			}*/
			putchar(text[index]);
		}
		if (escape)
			break;
	}
	if (isHighlight)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COMMON_LVB_REVERSE_VIDEO);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
}

void TextField::Clean(void) const
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursor = objectPositionStart;
	for (int iCursor = objectPositionStart.Y, i = 0; iCursor <= objectPositionEnd.Y; iCursor++, i++)
	{
		cursor.Y = iCursor;
		SetConsoleCursorPosition(hStdout, cursor);
		for (int j = 0/*objectPositionStart.X*/; j <= objectPositionEnd.X - objectPositionStart.X /*objectPositionEnd.X*/; j++)
		{
			putchar(' ');
		}
	}
}


void TextField::DeleteLastElementPlayerName(void)
{
	unsigned int i = 0;
	for (; i < text.size(); i++)
	{
		if (i == 0 && text[i] == '_')
			return;
		if (text[i] == '_')
		{
			text[i - 1] = '_';
			return;
		}
	}
	text[i - 1] = '_';
}



void TextField::SetLastElementPlayerName(char c)
{
	for (unsigned int i = 0; i < text.size(); i++)
	{
		if (text[i] == '_')
		{
			text[i] = c;
			return;
		}
	}
}


char** TextField::GetObjectImage(void) const
{
	Print();
	return nullptr;
}

bool TextField::GetIsHighLight(void)
{
	return isHighlight;
}

const TextField& TextField::operator=(const TextField& other)
{
	this->isHighlight = other.isHighlight;
	this->maxSize = other.maxSize;
	this->text = other.text;
	return *this;
}

const TextField& TextField::operator=(const char* cstr)
{
	SetText(cstr);
	return *this;
}