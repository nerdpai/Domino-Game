#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include"Rotation.hpp"
#include<Windows.h>


class GameObject
{
public:

	GameObject(short int XStart = 0, short int YStart = 0, short int XEnd = 0, short int YEnd = 0, Rotation rotation = NORMAL);
	GameObject(COORD positionStart, COORD positionEnd, Rotation rotation = NORMAL);


	Rotation GetRotation(void) const;

	COORD GetPositionStart(void) const;
	COORD GetPositionEnd(void) const;


	virtual Rotation SetRotation(Rotation rotation);

	virtual COORD SetPositionStart(short int XStart = 0, short int YStart = 0);
	COORD SetPositionStart(COORD positionStart);

	virtual COORD SetPositionEnd(short int XEnd = 0, short int YEnd = 0);
	COORD SetPositionEnd(COORD positionEnd);


	virtual char** GetObjectImage(void) const = 0;

protected:
	COORD objectPositionStart;//left top point
	COORD objectPositionEnd;//right bottom point
	Rotation objectRotation;
};


#endif
