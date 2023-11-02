#ifndef SPRITE_HPP
#define SPRITE_HPP

#include"GameObject.hpp"
#include<math.h>
#include<string.h>
#include<stdio.h>
#include<fstream>
#include<chrono>
#include<thread>

#define RGB 3
#define ASCII_PIXEL 219






class Sprite : public GameObject
{
public:
	Sprite(const char* path = nullptr, short int XStart = 0, short int YStart = 0, short int XEnd = 0, short int YEnd = 0, Rotation rotation = NORMAL);
	Sprite(COORD positionStart, COORD positionEnd, Rotation rotation = NORMAL, const char* path = nullptr);

	void SetBmpImage(const char* path, bool downloadWithBackground = true);

	void Print(void) const;

	char** GetObjectImage(void) const override;

	~Sprite();

protected:
	void DeleteImage(void);

	unsigned short imageCols;
	unsigned short imageRows;
	unsigned char** spriteImage;
	bool withBackground;
};

#endif

