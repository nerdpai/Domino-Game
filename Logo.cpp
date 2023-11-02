#include "Logo.hpp"


//Logo::Logo(const char* path,short int XStart, short int YStart, short int XEnd, short int YEnd, Rotation rotation) : GameObject(XStart, YStart, XEnd, YEnd, rotation)
//{
//	imageCols = imageRows = 0;
//	logoImage = nullptr;
//	SetBmpImage(path);
//}
//Logo::Logo(COORD positionStart, COORD positionEnd, Rotation rotation, const char* path) : GameObject(positionStart, positionEnd, rotation)
//{
//	imageCols = imageRows = 0;
//	logoImage = nullptr;
//	SetBmpImage(path);
//}
//
//
//void Logo::DeleteImage(void)
//{
//	if (imageCols != 0 && imageRows != 0)
//	{
//		delete[] logoImage[imageCols * imageRows - 1];
//		delete[] logoImage;
//		imageCols = imageRows = 0;
//		logoImage = nullptr;
//	}
//	SetPositionStart();
//	SetPositionEnd();
//}
//
//Logo::~Logo()
//{
//	DeleteImage();
//}
//
//
//void Logo::SetBmpImage(const char* path)
//{
//	DeleteImage();
//	{
//		char format[5] = { '\0' };
//		if (path == nullptr || strlen(path) < 4 || strcmp(strcpy(format, &path[strlen(path) - 4]), ".bmp") != 0)
//			return;
//	}
//
//	std::ifstream fin;
//	fin.open(path);
//	if (!fin.is_open())
//	{
//		return;
//	}
//
//
//	char info[54] = { '\0' };
//	fin.seekg(0, fin.end);
//	int length = ((int)fin.tellg()) - 54;
//	if (length <= 0)
//		return;
//	fin.seekg(0, fin.beg);
//	fin.read(info, 54);
//	imageCols = *((unsigned short*)&info[18]);
//	imageRows = *((unsigned short*)&info[22]);
//
//	COORD console = GameController::GetConsoleSize();
//	if (imageCols == 0 || imageRows == 0||imageCols>console.X||imageRows>console.Y)
//	{
//		imageCols = imageRows = 0;
//		SetPositionStart();
//		SetPositionEnd();
//		return;
//	}
//
//
//	logoImage = new unsigned char* [imageCols * imageRows]{ nullptr };
//	logoImage[imageCols * imageRows - 1] = new unsigned char[imageCols * imageRows * RGB]{ '\0' };
//	for (int i = imageCols * imageRows - 2, j = 1; i > -1; i--, j++)
//	{
//		logoImage[i] = logoImage[imageCols * imageRows - 1] + j * RGB;
//	}
//	fin.read((char*)logoImage[imageCols * imageRows - 1], imageCols * imageRows * RGB);
//	fin.close();
//
//	objectPositionStart.X = (console.X - imageCols) / 2;
//	objectPositionStart.Y = (console.Y - imageCols) / 2;
//	objectPositionEnd = console;
//}
//
//
//void Logo::Print() const
//{
//	if (logoImage == nullptr)
//		return;
//	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
//	for (double gradation = 0.0;; gradation += 1.0 / 100.0)
//	{
//		if (gradation > 1.0)
//			gradation = 1.0;
//		COORD cursor = objectPositionStart;
//		cursor.Y--;
//		for (unsigned int i = 0; i < imageRows; i++)
//		{
//			cursor.Y++;
//			SetConsoleCursorPosition(hStdout, cursor);
//			for (unsigned int j = 0; j < imageCols; j++)
//			{
//				printf("\033[38;2;%d;%d;%dm%c\033[m", (int)(logoImage[i * imageCols + j][2]*gradation), (int)(logoImage[i * imageCols + j][1] * gradation), (int)(logoImage[i * imageCols + j][0] * gradation), ASCII_PIXEL);
//			}
//		}
//		if (gradation == 1.0)
//			break;
//		std::this_thread::sleep_for(std::chrono::milliseconds(20));
//	}
//	std::this_thread::sleep_for(std::chrono::milliseconds(500));
//}
//
//
//char** Logo::GetObjectImage(void) const
//{
//	Print();
//	return nullptr;
//}

Logo::Logo(const char* path, short int XStart, short int YStart, short int XEnd, short int YEnd, Rotation rotation) : Sprite( path,  XStart,  YStart,  XEnd,  YEnd,  rotation)
{

}
Logo::Logo(COORD positionStart, COORD positionEnd, Rotation rotation, const char* path) : Sprite(positionStart, positionEnd, rotation, path)
{

}


char** Logo::GetObjectImage(void) const
{
	Logo::Print();
	return nullptr;
}

void Logo::Print(void) const // print with wmooth light animation
{
	if (spriteImage == nullptr)
		return;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	for (double gradation = 0.0;; gradation += 1.0 / 40.0)
	{
		if (gradation > 1.0)
			gradation = 1.0;
		COORD cursor = objectPositionStart;
		cursor.Y--;
		for (unsigned int i = 0; i < imageRows; i++)
		{
			cursor.Y++;
			SetConsoleCursorPosition(hStdout, cursor);
			for (unsigned int j = 0; j < imageCols; j++)
			{
				printf("\033[38;2;%d;%d;%dm%c\033[m", (int)(spriteImage[i * imageCols + j][2] * gradation), (int)(spriteImage[i * imageCols + j][1] * gradation), (int)(spriteImage[i * imageCols + j][0] * gradation), ASCII_PIXEL);
			}
		}
		if (gradation == 1.0)
			break;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}