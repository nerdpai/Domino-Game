#include"KeyBoardListener.hpp"


//set a base keyboard nawigation
unsigned char KeyBoardListener::keyUp[3]{EXTENDED_KEY,ARROW_UP};
unsigned char KeyBoardListener::keyDown[3]{ EXTENDED_KEY,ARROW_DOWN };
unsigned char KeyBoardListener::keyLeft[3]{ EXTENDED_KEY,ARROW_LEFT };
unsigned char KeyBoardListener::keyRight[3]{ EXTENDED_KEY,ARROW_RIGHT };
unsigned char KeyBoardListener::keyBack[3]{ ESC, 0};
unsigned char KeyBoardListener::keyNext[3]{ ENTER, 0};
unsigned char KeyBoardListener::keyRotate[3]{ R_ROTATION,0};
unsigned char KeyBoardListener::keyPull[3]{ P_PULL,0 };
//

const unsigned char* const KeyBoardListener::pullOfKeys[8] = { KeyBoardListener::keyDown, KeyBoardListener::keyLeft, KeyBoardListener::keyUp, KeyBoardListener::keyRight, KeyBoardListener::keyBack, KeyBoardListener::keyNext, KeyBoardListener::keyRotate, KeyBoardListener::keyPull };


const unsigned char* KeyBoardListener::GetLastKey(void)
{
	lastCharacter[0] = lastCharacter[1] = '\0';
	//unsigned char command;
	while (!_kbhit());
	while (_kbhit() > 2)
	{
		_getch();
	}
	lastCharacter[0] = _getch();
	if (lastCharacter[0] == EXTENDED_KEY || lastCharacter[0] == SYSTEM_KEY)
	{
		lastCharacter[1] = _getch();
		return lastCharacter;
	}
	if (_kbhit())
		lastCharacter[0] = _getch();
	return lastCharacter;
}

const unsigned char* KeyBoardListener::GetNewKey(void)
{
	while (_kbhit())
		_getch();
	lastCharacter[0] = _getch();
	if(_kbhit())
		lastCharacter[1] = _getch();
	return lastCharacter;
}

void KeyBoardListener::CleanInputStream(void)
{
	while (_kbhit())
		_getch();
}


KeyBoardListener::KeyBoardListener()
{
	lastCharacter[0] = lastCharacter[1] = lastCharacter[2] = '\0';
}