#ifndef LOGO_HPP
#define LOGO_HPP


#include"Sprite.hpp"



class Logo : public Sprite
{
public:
	Logo(const char* path = nullptr, short int XStart = 0, short int YStart = 0, short int XEnd = 0, short int YEnd = 0, Rotation rotation = NORMAL);
	Logo(COORD positionStart, COORD positionEnd, Rotation rotation = NORMAL, const char* path = nullptr);

	void Print(void) const;

	char** GetObjectImage(void) const;

private:
};

#endif

