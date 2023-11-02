#pragma warning(disable: 4996)
#include "Sprite.hpp"
#include"GameController.hpp"//



Sprite::Sprite(const char* path, short int XStart, short int YStart, short int XEnd, short int YEnd, Rotation rotation) : GameObject(XStart, YStart, XEnd, YEnd, rotation)
{
	withBackground = true;
	imageCols = imageRows = 0;
	spriteImage = nullptr;
	SetBmpImage(path);
}
Sprite::Sprite(COORD positionStart, COORD positionEnd, Rotation rotation, const char* path) : GameObject(positionStart, positionEnd, rotation)
{
	withBackground = true;
	imageCols = imageRows = 0;
	spriteImage = nullptr;
	SetBmpImage(path);
}


void Sprite::DeleteImage(void)
{
	if (imageCols != 0 && imageRows != 0)
	{
		delete[] spriteImage[imageCols * imageRows - 1];
		delete[] spriteImage;
		imageCols = imageRows = 0;
		spriteImage = nullptr;
	}
	SetPositionStart();
	SetPositionEnd();
}

Sprite::~Sprite()
{
	DeleteImage();
}


void Sprite::SetBmpImage(const char* path, bool downloadWithBackground)
{
	DeleteImage();
	{
		char format[5] = { '\0' };
		if (path == nullptr || strlen(path) < 4 || strcmp(strcpy(format, &path[strlen(path) - 4]), ".bmp") != 0)
			return;
	}

	std::ifstream fin;
	fin.open(path);
	if (!fin.is_open())
	{
		return;
	}


	unsigned char info[54] = { '\0' };
	fin.seekg(0, fin.end);
	int length = ((int)fin.tellg()) - 54;
	if (length <= 0)
		return;
	fin.seekg(0, fin.beg);
	fin.read((char*)info, 54);
	imageCols = *((unsigned short*)&info[18]);
	imageRows = *((unsigned short*)&info[22]);

	COORD console = GameController::GetConsoleSize();
	if (imageCols == 0 || imageRows == 0 || imageCols > console.X || imageRows > console.Y)
	{
		imageCols = imageRows = 0;
		SetPositionStart();
		SetPositionEnd();
		return;
	}


	spriteImage = new unsigned char* [imageCols * imageRows]{ nullptr };
	spriteImage[imageCols * imageRows - 1] = new unsigned char[imageCols * imageRows * RGB]{ '\0' };
	for (int i = imageCols * imageRows - 2, j = 1; i > -1; i--, j++)
	{
		spriteImage[i] = spriteImage[imageCols * imageRows - 1] + j * RGB;
	}
	fin.read((char*)spriteImage[imageCols * imageRows - 1], imageCols * imageRows * (unsigned short)RGB);
	fin.close();


	for (int i = 0; i < imageRows; i++)
	{
		for (int j = 0; j < int(ceil(imageCols / 2.0)); j++)
		{
			unsigned char* temp = spriteImage[i * imageCols + j];
			spriteImage[i * imageCols + j] = spriteImage[i * imageCols + imageCols - 1 - j];
			spriteImage[i * imageCols + imageCols - 1 - j] = temp;
		}
	}
	withBackground = downloadWithBackground;


	objectPositionStart.X = (console.X - imageCols) / 2;
	objectPositionStart.Y = (console.Y - imageRows) / 2;
	objectPositionEnd = console;
}


void Sprite::Print() const
{
	if (spriteImage == nullptr)
		return;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursor = objectPositionStart;
	cursor.Y--;
	for (unsigned int i = 0; i < imageRows; i++)
	{
		cursor.Y++;
		SetConsoleCursorPosition(hStdout, cursor);
		for (unsigned int j = 0; j < imageCols; j++)
		{
			if (withBackground)
			{
				printf("\033[38;2;%d;%d;%dm%c\033[m", (int)(spriteImage[i * imageCols + j][2]), (int)(spriteImage[i * imageCols + j][1]), (int)(spriteImage[i * imageCols + j][0]), ASCII_PIXEL);
			}
			else
			{
				if (spriteImage[0][2] == spriteImage[i * imageCols + j][2] && spriteImage[0][1] == spriteImage[i * imageCols + j][1] && spriteImage[0][0] == spriteImage[i * imageCols + j][0])
				{
					putchar(' ');
				}
				else
				{
					printf("\033[38;2;%d;%d;%dm%c\033[m", (int)(spriteImage[i * imageCols + j][2]), (int)(spriteImage[i * imageCols + j][1]), (int)(spriteImage[i * imageCols + j][0]), ASCII_PIXEL);
				}
			}
		}
	}
}


char** Sprite::GetObjectImage(void) const
{
	Print();
	return nullptr;
}
