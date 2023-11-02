#ifndef GAMECAM_HPP
#define GAMECAM_HPP

//
#include"KnuckleOfDomino.hpp"
//

#include"GameObject.hpp"
#include<cwchar>
#include"PullOfDominoKnuckles.hpp"
#include"KeyBoardListener.hpp"
#include"Hand.hpp"
#include"Table.hpp"
#include"Sprite.hpp"
#include"Logo.hpp"

#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<thread>
#include<chrono>


#define COUNT_OF_GAME_TYPES 4


#define SCENE_GAME_EXIT -1
#define SCENE_LOGO 0
#define SCENE_SETTINGS_OF_KEYS 1
#define SCENE_MENU 2
#define SCENE_CONFIG 3
#define SCENE_GAME_CHOISE 4
#define SCENE_GAME_TABLE 5
#define SCENE_PULL 6
#define SCENE_SCORE 7
#define SCENE_RULES 8




class GameController : public GameObject, public KeyBoardListener
{	
public:
	static COORD GetConsoleSize(void);

	static CONSOLE_FONT_INFOEX GetStartConsoleFont(void);


	void Print(const GameObject* object);

	void Clean(const GameObject* object);
	void Clean(void);



	GameController(short int XStart = 0, short int YStart = 0, short int XEnd = GetConsoleSize().X, short int YEnd = GetConsoleSize().Y, Rotation rotation = NORMAL);
	GameController(COORD positionStart, COORD positionEnd = GetConsoleSize(), Rotation rotation = NORMAL);

	char** GetObjectImage(void) const override;


	unsigned short SetPlayersCount(unsigned short count);

	int Scene(int index);

	const PullOfDominoKnuckles* GetPull(void);

	const PullOfDominoKnuckles* RefreshPull(void);
	const Table* RefreshTable(void);

	void GameRefresher(void);

	unsigned short SetPlayersName(void);

	~GameController();


private:
	typedef int (GameController::* const Scenes)(void);
	Scenes scenes[9];

	static CONSOLE_FONT_INFOEX SetMinPixelsCount(unsigned int size = 65);

	TextField keysHits[2];

	void ControllerClean(void);

	KnuckleOfDomino knuckle;//just that domino parts doesn't delete

	char* clean;

	std::vector<Hand*> players;

	static CONSOLE_FONT_INFOEX startFont;
	PullOfDominoKnuckles *pull;
	Table* table;


	bool IsNewKeyUnique(const unsigned char* newKey, const unsigned char* withoutKey);


	Logo* logo;


	Sprite* ruleGirl;

	//Area of Scenes
	int SceneLogo(void);
	int SceneSettingsOfKeys(void);
	int SceneMenu(void);
	int SceneRefreshConfig(void);
	int SceneGameChoise(void);
	int SceneGameTable(void);
	int ScenePull(void);
	int SceneScore(void);
	int SceneRules(void);
	
	//End of this Area
};


#endif