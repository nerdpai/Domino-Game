#include"GameObject.hpp"



GameObject::GameObject(short int XStart, short int YStart, short int XEnd, short int YEnd, Rotation rotation)
{
	SetPositionStart(XStart, YStart);
	SetPositionEnd(XEnd, YEnd);
	SetRotation(rotation);
}
GameObject::GameObject(COORD positionStart, COORD positionEnd, Rotation rotation)
{
	SetPositionStart(positionStart);
	SetPositionEnd(positionEnd);
	SetRotation(rotation);
}

Rotation GameObject::GetRotation(void) const
{
	return objectRotation;
}
COORD GameObject::GetPositionStart(void) const
{
	return objectPositionStart;
}
COORD GameObject::GetPositionEnd(void) const
{
	return objectPositionEnd;
}

Rotation GameObject::SetRotation(Rotation rotation)
{
	return this->objectRotation = rotation;
}
COORD GameObject::SetPositionStart(short int XStart, short int YStart)
{
	objectPositionStart.X = XStart;
	objectPositionStart.Y = YStart;
	return objectPositionStart;
}
COORD GameObject::SetPositionStart(COORD positionStart)
{
	return SetPositionStart(positionStart.X, positionStart.Y);
}

COORD GameObject::SetPositionEnd(short int XEnd, short int YEnd)
{
	objectPositionEnd.X = XEnd;
	objectPositionEnd.Y = YEnd;
	return objectPositionEnd;
}
COORD GameObject::SetPositionEnd(COORD positionEnd)
{
	return SetPositionEnd(positionEnd.X, positionEnd.Y);
}