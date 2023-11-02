#ifndef TEXTFIELD_HPP
#define TEXTFIELD_HPP

#define UNLIMITED -1

#include<string>
#include"GameObject.hpp"
#include<limits.h>
#include<string.h>


class GameController;

class TextField : public GameObject
{
public:
	TextField(short int XStart = 0, short int YStart = 0, short int XEnd = 0, short int YEnd = 0, Rotation rotation = NORMAL);
	TextField(COORD positionStart, COORD positionEnd, Rotation rotation = NORMAL);

	const char* SetText(const char* text);
	const char* SetText(std::string &text);

	const char* SetTextAppend(const char* text);
	const char* SetTextAppend(std::string& text);

	short SetMaxTextSize(short maxSize);

	const TextField& operator=(const TextField&);
	const TextField& operator=(const char* cstr);

	unsigned short GetTextSize(void);

	const char* GetText(void);

	short GetMaxTextSize(void);

	bool GetIsHighLight(void);

	void DeleteLastElementPlayerName(void);

	void SetLastElementPlayerName(char c);

	bool SetHighLight(bool isHighlight);

	void Print(void) const;
	void Clean(void) const;


	char** GetObjectImage(void) const override;

private:
	std::string text;
	short maxSize;
	bool isHighlight;
};

#endif
