#ifndef EMBEDDEDDOMINO_HPP
#define EMBEDDEDDOMINO_HPP
#include<string.h>

//if file with parts of domino doesn't open


char** GetDominoParts(void);

unsigned short GetDominoCols(void);

unsigned short GetDominoRows(void);

unsigned short GetMaxCountOfKnuckles(void);

const char* GetDominoFileName(void);

#endif
