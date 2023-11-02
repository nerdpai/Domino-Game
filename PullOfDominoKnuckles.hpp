#ifndef PULLOFDOMINOKNUCKLES_HPP
#define PULLOFDOMINOKNUCKLES_HPP

#include"GameObject.hpp"
#include"KnuckleOfDomino.hpp"
#include"EmbeddedDomino.hpp"
#include<stdio.h>
#include<vector>
#include<stdlib.h>
#include<time.h>
#include<math.h>


class PullOfDominoKnuckles : public GameObject
{
public:
	PullOfDominoKnuckles(short int XStart = 0, short int YStart = 0, short int XEnd = 0, short int YEnd = 0, Rotation rotation = NORMAL);
	PullOfDominoKnuckles(COORD positionStart, COORD positionEnd, Rotation rotation = NORMAL);

	std::vector<KnuckleOfDomino*> GetHandOfKnuckels(unsigned int handSize);
	KnuckleOfDomino* GetKnuckle(void);

	unsigned int GetNumberOfRemainingKnuckles(void) const;

	char** GetObjectImage(void) const override;

	~PullOfDominoKnuckles();

private:
	std::vector<KnuckleOfDomino*> CreatePull(const char* cstr = GetDominoFileName());
	
	
	unsigned short numberOfRemainingKnuckles;
	std::vector<KnuckleOfDomino*> knuckles;
};


#endif

