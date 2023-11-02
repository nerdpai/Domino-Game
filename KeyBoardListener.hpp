#ifndef KEYBOARDLISTENER_HPP
#define KEYBOARDLISTENER_HPP

#include<conio.h>


#define EXTENDED_KEY 224
#define SYSTEM_KEY 0
#define ESC 27
#define ENTER 13
#define BACKSPACE 8
#define R_ROTATION 114
#define P_PULL 112
#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77

#define COUNT_OF_KEYS 8


class KeyBoardListener
{
public:
	KeyBoardListener();

	const unsigned char* GetLastKey(void);
	const unsigned char* GetNewKey(void);
	void CleanInputStream(void);

	static const unsigned char* const pullOfKeys[8];

protected:
	unsigned char lastCharacter[3];
	static unsigned char keyUp[3];
	static unsigned char keyDown[3];
	static unsigned char keyLeft[3];
	static unsigned char keyRight[3];
	static unsigned char keyBack[3];
	static unsigned char keyNext[3];
	static unsigned char keyRotate[3];
	static unsigned char keyPull[3];
};



//char* GetLastKey(void)
//{
//
//}



#endif