#pragma warning(disable: 4996)
#include "GameController.hpp"



COORD GameController::GetConsoleSize(void)
{
	CONSOLE_SCREEN_BUFFER_INFO console;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console);
	COORD consoleSize = console.dwMaximumWindowSize;
	consoleSize.X--; //consoleSize.Y--;
	return consoleSize;
}


CONSOLE_FONT_INFOEX GameController::GetStartConsoleFont(void)
{
	return startFont;
}


void GameController::ControllerClean(void)
{
	//delete all elements of game
	delete pull;
	pull = nullptr;
	delete table;
	table = nullptr;

	for (int i = players.size() - 1; i > -1; i--)
	{
		delete players[i];
		players.pop_back();
	}
	//
}

CONSOLE_FONT_INFOEX GameController::SetMinPixelsCount(unsigned int size)
{



	size = size + 2;//because win-api

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX font, startFont;
	startFont.cbSize = sizeof(startFont);

	GetCurrentConsoleFontEx(hStdout, 1, &startFont);


	font.cbSize = sizeof(font);
	font.nFont = 0;
	font.FontFamily = 48;
	font.FontWeight = 400;
	std::wcscpy(font.FaceName, L"Terminal");
	font.dwFontSize.X = font.dwFontSize.Y = 8;


	//make font normal size
	SetConsoleDisplayMode(hStdout, CONSOLE_FULLSCREEN_MODE, 0);
	SetCurrentConsoleFontEx(hStdout, 1, &font);
	CONSOLE_SCREEN_BUFFER_INFOEX console;
	console.cbSize = sizeof(console);
	GetConsoleScreenBufferInfoEx(hStdout, &console);
	font.dwFontSize.X = font.dwFontSize.Y = (SHORT)((console.dwMaximumWindowSize.Y < console.dwMaximumWindowSize.X ? console.dwMaximumWindowSize.Y : console.dwMaximumWindowSize.X) / (double)size * font.dwFontSize.Y);
	SetCurrentConsoleFontEx(hStdout, 1, &font);
	GetConsoleScreenBufferInfoEx(hStdout, &console);
	for (int i = font.dwFontSize.Y; unsigned(console.dwMaximumWindowSize.Y < console.dwMaximumWindowSize.X ? console.dwMaximumWindowSize.Y : console.dwMaximumWindowSize.X) <= size; i--)
	{
		font.dwFontSize.X = font.dwFontSize.Y = i;
		if (font.dwFontSize.Y == 0)
			exit(1);
		SetCurrentConsoleFontEx(hStdout,1, &font);
		GetConsoleScreenBufferInfoEx(hStdout, &console);
	}
	for (int i = font.dwFontSize.Y; unsigned(console.dwMaximumWindowSize.Y < console.dwMaximumWindowSize.X ? console.dwMaximumWindowSize.Y : console.dwMaximumWindowSize.X) >= size; i++)
	{
		font.dwFontSize.X = font.dwFontSize.Y = i;
		if (font.dwFontSize.Y == 0)
			exit(1);
		SetCurrentConsoleFontEx(hStdout, 1, &font);
		GetConsoleScreenBufferInfoEx(hStdout, &console);
	}
	if (font.dwFontSize.Y != 1)
		font.dwFontSize.X = --font.dwFontSize.Y;
	SetCurrentConsoleFontEx(hStdout, 1, &font);
	//


	//make buffer normal
	GetConsoleScreenBufferInfoEx(hStdout, &console);
	console.srWindow.Right = console.dwMaximumWindowSize.X - 3;
	console.srWindow.Bottom = console.dwMaximumWindowSize.Y;
	console.dwSize.X = console.dwMaximumWindowSize.X - 2;
	SetConsoleScreenBufferInfoEx(hStdout, &console);
	//


	//make cursor invisible and make font more light
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	cci.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	//

	//ASCII input and output
	SetConsoleCP(CP_ACP);
	SetConsoleOutputCP(CP_ACP);
	//
	return startFont;
}


CONSOLE_FONT_INFOEX GameController::startFont = GameController::SetMinPixelsCount();



void GameController::Print(const GameObject* object)
{
	//must i print?
	if (object == nullptr)
		return;
	char** objectImage = object->GetObjectImage();
	if (objectImage == nullptr)
		return;
	//


	//print in object area its image
	COORD startCor = object->GetPositionStart(), endCor = object->GetPositionEnd();
	COORD cursor = startCor;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	bool escape = false;
	for (int i = startCor.Y,iIm = 0; i <= endCor.Y; i++,iIm++)
	{
		cursor.Y = i;
		SetConsoleCursorPosition(hStdout, cursor);
		for (int j = startCor.X,jIm = 0; j <= endCor.X; j++,jIm++)
		{
			if (objectImage[iIm][jIm] == '\0')
			{
				escape = true;
				break;
			}
			/*cursor.X = j;
			SetConsoleCursorPosition(hStdout, cursor);*/
			putchar(objectImage[iIm][jIm]);
		}
		if (escape)
			break;
	}
	delete[] * objectImage;
	delete[] objectImage;
	//
}


void GameController::Clean(const GameObject* object)
{
	//clean object area
	COORD startCor = object->GetPositionStart(), endCor = object->GetPositionEnd();
	COORD cursor = startCor;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = startCor.Y; i <= endCor.Y; i++)
	{
		cursor.Y = i;
		SetConsoleCursorPosition(hStdout, cursor);
		for (int j = startCor.X; j <= endCor.X; j++)
		{
			putchar(' ');
		}
	}
	//
}
void GameController::Clean(void)
{
	//clean all screen
	COORD endCor = GetConsoleSize(), curosrStart = { 0,0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curosrStart);
	if (clean == nullptr)
	{
		clean = new char[(endCor.X + 1) * endCor.Y + 1]{ '\0' };
		memset(clean, ' ', (endCor.X + 1) * endCor.Y * sizeof(char));
	}
	printf("%s",clean);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curosrStart);
	//
}





GameController::GameController(short int XStart, short int YStart, short int XEnd, short int YEnd, Rotation rotation) :
scenes{ & GameController::SceneLogo,
& GameController::SceneSettingsOfKeys,
& GameController::SceneMenu,
& GameController::SceneRefreshConfig,
& GameController::SceneGameChoise,
& GameController::SceneGameTable,
& GameController::ScenePull,
& GameController::SceneScore,
& GameController::SceneRules },
GameObject(XStart, YStart, XEnd, YEnd, rotation)
{
	pull = nullptr;
	clean = nullptr;
	table = nullptr;
	keysHits[0].SetPositionStart(0, GetConsoleSize().Y - 3);
	keysHits[0].SetPositionEnd(GetConsoleSize().X, GetConsoleSize().Y-3);
	keysHits[1].SetPositionStart(0, GetConsoleSize().Y - 1);
	keysHits[1].SetPositionEnd(GetConsoleSize().X, GetConsoleSize().Y - 1);
}
GameController::GameController(COORD positionStart, COORD positionEnd, Rotation rotation) :
scenes{ & GameController::SceneLogo,
& GameController::SceneSettingsOfKeys,
& GameController::SceneMenu,
& GameController::SceneRefreshConfig,
& GameController::SceneGameChoise,
& GameController::SceneGameTable,
& GameController::ScenePull,
& GameController::SceneScore,
& GameController::SceneRules },
GameObject(positionStart, positionEnd, rotation)
{
	pull = nullptr;
	clean = nullptr;
	table = nullptr;
	keysHits[0].SetPositionStart(0, GetConsoleSize().Y - 3);
	keysHits[0].SetPositionEnd(GetConsoleSize().X, GetConsoleSize().Y - 3);
	keysHits[1].SetPositionStart(0, GetConsoleSize().Y - 1);
	keysHits[1].SetPositionEnd(GetConsoleSize().X, GetConsoleSize().Y - 1);
}



unsigned short GameController::SetPlayersName(void)
{
	//set first text position
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO console;
	COORD cursor = GetConsoleSize();
	cursor.X /= 2;
	cursor.Y /= 2;
	cursor.Y -= SHORT(players.size() / 2+1);
	if (cursor.Y < 0)
		cursor.Y = 0;
	cursor.X -= 7 /*text size*/		+	PLAYER_NAME_SIZE / 2;
	if (cursor.X < 0)
		cursor.X = 0;
	//

	//get unique name of player
	for (int i = 0; i <(int)players.size(); i++)
	{
		cursor.Y++;
		SetConsoleCursorPosition(hStdout, cursor);
		printf("Player%d name:", i);
		GetConsoleScreenBufferInfo(hStdout, &console);
		TextField playerName;
		playerName.SetPositionStart(console.dwCursorPosition);
		playerName.SetPositionEnd(playerName.GetPositionStart().X + PLAYER_NAME_SIZE - 1, playerName.GetPositionStart().Y);
		playerName.SetMaxTextSize(PLAYER_NAME_SIZE);
		for (int i = 0; i < PLAYER_NAME_SIZE; i++)
		{
			playerName.SetTextAppend("_");
		}
		playerName.Print();
		while (true)
		{
			const unsigned char* c = GetLastKey();

			if (c[0] == ENTER)
			{
				if (playerName.GetText()[0] != '_')
				{
					bool individual = true;
					std::string temp = playerName.GetText();
					for (int j = 0; j < i; j++)
					{
						if (temp == players[j]->GetPlayerName())
						{
							individual = false;
							break;
						}
					}
					if (individual)
						break;
				}
			}

			else if (c[0] == BACKSPACE)
			{
				playerName.DeleteLastElementPlayerName();
			}

			else if (c[0] != EXTENDED_KEY && isalpha((char)c[0]) && c[1] == '\0')
				playerName.SetLastElementPlayerName((char)c[0]);
			playerName.Print();
		}
		players[i]->SetPlayerName(playerName);
	}
	//
	Clean();

	//get unique name of player
	char name[PLAYER_NAME_SIZE+1] = { 0 };
	for (int i = 0; i < (int)players.size(); i++)
	{
		strcpy(name, players[i]->GetPlayerName());
		for (int j = 0; j <= PLAYER_NAME_SIZE; j++)
		{
			if (name[j] == '_')
			{
				name[j] = '\0';
				break;
			}
		}
		players[i]->SetPlayerName(name);
	}
	//

	return Hand::GetCountOfHands();
}




char** GameController::GetObjectImage(void) const
{
	return nullptr;
}



unsigned short GameController::SetPlayersCount(unsigned short count)
{
	//config hands
	COORD console = GetConsoleSize();
	if (count != 2 && count != 4)
	{
		return 0;
	}
	for (int i = 0; i < count; i++)
	{
		players.push_back(new Hand);
		players[i]->SetRotation((Rotation)i);
	}
	//

	//set first hand(bottom) position
	unsigned short handCorSize = SPACE + Hand::GetMaxCountOfVisibleKnucklesInHand() * (SPACE + KnuckleOfDomino::GetKnuckleImageCols());
	unsigned short startCorX = (console.X - handCorSize)/2, startCorY = (console.Y- (int)(console.Y*HOW_MANY_PERCENT_FOR_INFORMATION) - handCorSize) /2	/*it is for normalaize size*/;
	players[0]->SetPositionStart(startCorX, console.Y-(((int)(console.Y*HOW_MANY_PERCENT_FOR_INFORMATION))+KnuckleOfDomino::GetKnuckleImageRows()+SPACE)+1);
	players[0]->SetPositionEnd(startCorX+handCorSize-1,console.Y- ((int)(console.Y * HOW_MANY_PERCENT_FOR_INFORMATION)));
	players[0]->SetArrowsPosition(players[0]->GetPositionStart().X - 2, players[0]->GetPositionEnd().Y - 2, players[0]->GetPositionStart().X - 1, players[0]->GetPositionEnd().Y, HAND_ARROW_LEFT, " ##  #");
	players[0]->SetArrowsPosition(players[0]->GetPositionEnd().X + 1, players[0]->GetPositionEnd().Y - 2, players[0]->GetPositionEnd().X + 2, players[0]->GetPositionEnd().Y, HAND_ARROW_RIGHT, "#  ## ");
	//
	if (count == 2)
	{
		//set second hand(top) position
		players[1]->SetPositionStart(players[0]->GetPositionStart().X,0);
		players[1]->SetPositionEnd(players[0]->GetPositionEnd().X,KnuckleOfDomino::GetKnuckleImageRows() + SPACE-1);
		players[1]->SetRotation(FLIPPED);
		players[1]->SetArrowsPosition(players[1]->GetPositionStart().X - 2, players[1]->GetPositionStart().Y, players[1]->GetPositionStart().X - 1, players[1]->GetPositionStart().Y+2, HAND_ARROW_RIGHT, " ##  #");
		players[1]->SetArrowsPosition(players[1]->GetPositionEnd().X + 1, players[1]->GetPositionStart().Y, players[1]->GetPositionEnd().X + 2, players[1]->GetPositionStart().Y+2, HAND_ARROW_LEFT, "#  ## ");
		//
		
		return 2;
	}
	else
	{
		//set second hand(right) position
		players[1]->SetPositionStart(0,startCorY);
		players[1]->SetPositionEnd(KnuckleOfDomino::GetKnuckleImageRows() + SPACE-1, startCorY + handCorSize-1);
		players[1]->SetArrowsPosition(players[1]->GetPositionStart().X, players[1]->GetPositionStart().Y - 2, players[1]->GetPositionStart().X + 2, players[1]->GetPositionStart().Y - 1, HAND_ARROW_LEFT, " # # #");
		players[1]->SetArrowsPosition(players[1]->GetPositionStart().X, players[1]->GetPositionEnd().Y + 1, players[1]->GetPositionStart().X + 2, players[1]->GetPositionEnd().Y + 2, HAND_ARROW_RIGHT, "# # # ");
		//

		//set third hand(top) position
		players[2]->SetPositionStart(players[0]->GetPositionStart().X, 0);
		players[2]->SetPositionEnd(players[0]->GetPositionEnd().X, KnuckleOfDomino::GetKnuckleImageRows() + SPACE-1);
		players[2]->SetArrowsPosition(players[2]->GetPositionStart().X - 2, players[2]->GetPositionStart().Y, players[2]->GetPositionStart().X - 1, players[2]->GetPositionStart().Y + 2, HAND_ARROW_RIGHT, " ##  #");
		players[2]->SetArrowsPosition(players[2]->GetPositionEnd().X + 1, players[2]->GetPositionStart().Y, players[2]->GetPositionEnd().X + 2, players[2]->GetPositionStart().Y + 2, HAND_ARROW_LEFT, "#  ## ");
		//

		//set fourth hand(left) position
		players[3]->SetPositionStart(console.X - (KnuckleOfDomino::GetKnuckleImageRows() + SPACE)+1, startCorY);
		players[3]->SetPositionEnd(console.X, startCorY+handCorSize-1);
		players[3]->SetArrowsPosition(players[3]->GetPositionEnd().X - 2, players[3]->GetPositionStart().Y - 2, players[3]->GetPositionEnd().X, players[3]->GetPositionStart().Y - 1, HAND_ARROW_RIGHT, " # # #");
		players[3]->SetArrowsPosition(players[3]->GetPositionEnd().X - 2, players[3]->GetPositionEnd().Y+1, players[3]->GetPositionEnd().X, players[3]->GetPositionEnd().Y + 2, HAND_ARROW_LEFT, "# # # ");
		//

		return 4;
	}
	
}



int GameController::Scene(int index)
{
	return (this->*scenes[index])();
}


const PullOfDominoKnuckles* GameController::GetPull(void)
{
	if (pull == nullptr)
		return RefreshPull();
	return pull;
}


const PullOfDominoKnuckles* GameController::RefreshPull(void)
{
	delete pull;
	pull = new PullOfDominoKnuckles;
	return pull;
}

const Table* GameController::RefreshTable(void)
{
	delete table;
	table = new Table;
	return table;
}


void GameController::GameRefresher(void)
{
	//restart elements for regame
	RefreshPull();
	RefreshTable();
	for (int i = 0; i < Hand::GetCountOfHands(); i++)
	{
		players[i]->CleanHand();
	}
	//
}



GameController::~GameController()
{
	delete[] clean;
	clean = nullptr;

	ControllerClean();
}




//Area of Scenes
int GameController::SceneLogo(void)
{
	logo = new Logo("logoDomino.bmp");
	logo->Print();
	Clean();
	return SCENE_SETTINGS_OF_KEYS;
}


bool GameController::IsNewKeyUnique(const unsigned char* newKey, const unsigned char* withoutKey)
{
	bool unique = true;
	for (int i = 0; i < COUNT_OF_KEYS; i++)
	{
		if (pullOfKeys[i] == withoutKey)
			continue;
		if (strcmp((char*)newKey, (char*)pullOfKeys[i]) == 0)
		{
			unique = false;
			break;
		}
	}
	return unique;
}


int GameController::SceneSettingsOfKeys(void)
{
	printf("Do you want to play with standards keys?\n\n\n\t"
		"BACK - ESC\n\n\t"
		"NEXT - Enter\n\n\t"
		"LEFT - Arrow left\n\n\t"
		"RIGHT - Arrow right\n\n\t"
		"UP - Arrow up\n\n\t"
		"DOWN - Arrow down\n\n\t"
		"Special key - r\n\n\t"
		"PULL - p");

	while (true)
	{
		printf("\n\n\n\tdo you want to change it ?(y/n) ");
		CONSOLE_SCREEN_BUFFER_INFOEX consoleEx;
		consoleEx.cbSize = sizeof(consoleEx);
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfoEx(hStdout, &consoleEx);
		COORD startCor, endCor;
		startCor.X = consoleEx.dwCursorPosition.X;	startCor.Y = consoleEx.dwCursorPosition.Y;

		char c = '\0';

		CleanInputStream();

		//get correct answer key
		for (c = tolower(getchar()); c != 'y' && c != 'n'; c = tolower(getchar()))
		{
			while (getchar() != '\n');
			GetConsoleScreenBufferInfoEx(hStdout, &consoleEx);
			endCor.X = consoleEx.dwCursorPosition.X; endCor.Y = consoleEx.dwCursorPosition.Y;
			SetConsoleCursorPosition(hStdout, startCor);
			char* line = new char[(GetConsoleSize().X + 1)*(endCor.Y-startCor.Y+1)+1]{ '\0' };
			memset(line, ' ', (GetConsoleSize().X + 1) * (endCor.Y - startCor.Y + 1)*sizeof(char));
			printf("%s", line);
			delete[] line;
			SetConsoleCursorPosition(hStdout, startCor);
		}
		//
		while (getchar() != '\n');


		if (c == 'n')
			break;

		//set new keys
		Clean();

		const unsigned char* newKey;

		printf("\n\n\n\t\t\tPress key to reconfig:\n\n\t\tBACK: ");
		while (!IsNewKeyUnique(newKey = GetNewKey(), keyBack));
		memcpy(keyBack, newKey, 2 * sizeof(char));
		printf("\n\n\t\tNEXT: ");
		while (!IsNewKeyUnique(newKey = GetNewKey(), keyNext));
		memcpy(keyNext, newKey, 2 * sizeof(char));
		printf("\n\n\t\tLEFT: ");
		while (!IsNewKeyUnique(newKey = GetNewKey(), keyLeft));
		memcpy(keyLeft, newKey, 2 * sizeof(char));
		printf("\n\n\t\tRIGHT: ");
		while (!IsNewKeyUnique(newKey = GetNewKey(), keyRight));
		memcpy(keyRight, newKey, 2 * sizeof(char));
		printf("\n\n\t\tUP: ");
		while (!IsNewKeyUnique(newKey = GetNewKey(), keyUp));
		memcpy(keyUp, newKey, 2 * sizeof(char));
		printf("\n\n\t\tDOWN: ");
		while (!IsNewKeyUnique(newKey = GetNewKey(), keyDown));
		memcpy(keyDown, newKey, 2 * sizeof(char));
		printf("\n\n\t\tSpecial key: ");
		while (!IsNewKeyUnique(newKey = GetNewKey(), keyRotate));
		memcpy(keyRotate, newKey, 2 * sizeof(char));
		printf("\n\n\t\tPULL: ");
		while (!IsNewKeyUnique(newKey = GetNewKey(), keyPull));
		memcpy(keyPull, newKey, 2 * sizeof(char));
		//
	}
	Clean();

	return SCENE_MENU;
}

int GameController::SceneMenu(void)
{
	//what everyone kay actualy do
	const char* hits1 = "LEFT: choise 2 players game     RIGHT: choise 4 players game", *hits2 = "BACK: exit from the game";
	keysHits[0] = hits1;
	keysHits[0].SetPositionStart(short((keysHits[0].GetPositionEnd().X - strlen(hits1)) / 2), keysHits[0].GetPositionStart().Y);
	keysHits[0].Print();

	keysHits[1] = hits2;
	keysHits[1].SetPositionStart(short((keysHits[1].GetPositionEnd().X - strlen(hits2)) / 2), keysHits[1].GetPositionStart().Y);
	keysHits[1].Print();
	//

	//set position of buttons
	COORD console = GetConsoleSize();
	TextField choisePlayers[COUNT_OF_GAME_TYPES];
	const char* cStr[COUNT_OF_GAME_TYPES] = { "2 Players" , "4 Players","Rules","Key Settings"};
	for (short i = 0; i < COUNT_OF_GAME_TYPES/2; i++)
	{
		choisePlayers[i] = cStr[i];
		choisePlayers[i].SetPositionStart(short((console.X/2 - strlen(cStr[i])) / 2 + i * console.X / 2), short(console.Y / 2));
		choisePlayers[i].SetPositionEnd(console.X / 2 + i * console.X / 2, console.Y / 2);
	}

	for (short i = 2; i < COUNT_OF_GAME_TYPES; i++)
	{
		choisePlayers[i] = cStr[i];
		choisePlayers[i].SetPositionStart(short((console.X - strlen(cStr[i])) / 2), short(console.Y / 2 + (i%2==0?-1:1)*(console.Y/4)));
		choisePlayers[i].SetPositionEnd(console.X, short(console.Y / 2 + (i % 2 == 0 ? -1 : 1) * (console.Y / 4)));
	}
	//


	//get correct pressed key
	CleanInputStream();
	while (true)
	{
		for (short i = 0; i < COUNT_OF_GAME_TYPES; i++)
			choisePlayers[i].Print();
		const unsigned char* key = GetLastKey();
		if (memcmp(key, keyNext, 2 * sizeof(char)) == 0)
		{
			bool light = false;
			for (short i = 0; i < COUNT_OF_GAME_TYPES && light == false; i++)
			{
				light = choisePlayers[i].GetIsHighLight();
			}
			if (light)
				break;

		}
		else if (memcmp(key, keyLeft, 2 * sizeof(char)) == 0)
		{
			choisePlayers[0].SetHighLight(true);
			choisePlayers[1].SetHighLight(false);
			choisePlayers[2].SetHighLight(false);
			choisePlayers[3].SetHighLight(false);
		}
		else if (memcmp(key, keyRight, 2 * sizeof(char)) == 0)
		{
			choisePlayers[0].SetHighLight(false);
			choisePlayers[1].SetHighLight(true);
			choisePlayers[2].SetHighLight(false);
			choisePlayers[3].SetHighLight(false);
		}
		else if (memcmp(key, keyUp, 2 * sizeof(char)) == 0)
		{
			choisePlayers[0].SetHighLight(false);
			choisePlayers[1].SetHighLight(false);
			choisePlayers[2].SetHighLight(true);
			choisePlayers[3].SetHighLight(false);
		}
		else if (memcmp(key, keyDown, 2 * sizeof(char)) == 0)
		{
			choisePlayers[0].SetHighLight(false);
			choisePlayers[1].SetHighLight(false);
			choisePlayers[2].SetHighLight(false);
			choisePlayers[3].SetHighLight(true);
		}
		else if (memcmp(key, keyBack, 2 * sizeof(char)) == 0)
		{
			return SCENE_GAME_EXIT;
		}
	}
	//

	GameRefresher();

	if (choisePlayers[0].GetIsHighLight() == true)
	{
		SetPlayersCount(2);
	}

	else if (choisePlayers[1].GetIsHighLight() == true)
	{
		SetPlayersCount(4);
	}

	else if (choisePlayers[2].GetIsHighLight() == true)
	{
		Clean();
		return SCENE_RULES;
	}
	else if (choisePlayers[3].GetIsHighLight() == true)
	{
		Clean();
		return SCENE_SETTINGS_OF_KEYS;
	}

	for (unsigned int i = 0; i < Hand::GetCountOfHands(); i++)
	{
		players[i]->SetHandOfKnuckles(pull->GetHandOfKnuckels(7));
	}

	Clean();

	srand(time(NULL));
	Hand::SetCurrentHandId(rand() % Hand::GetCountOfHands());

	SetPlayersName();


	return SCENE_GAME_CHOISE;
}

int GameController::SceneRefreshConfig(void)
{
	GameRefresher();
	for (unsigned int i = 0; i < Hand::GetCountOfHands(); i++)
	{
			players[i]->SetHandOfKnuckles(pull->GetHandOfKnuckels(7));
	}
	srand(time(NULL));
	Hand::SetCurrentHandId(rand() % Hand::GetCountOfHands());
	return SCENE_GAME_CHOISE;
}

int GameController::SceneGameChoise(void)
{
	//what everyone kay actualy do
	const char* hits1;
	const char* hits2;
	if (pull->GetNumberOfRemainingKnuckles() == 0&&table->GetIsFirstKnuckleSet())
	{
		hits1 = "NEXT: take knuckle   PULL: show pull   SpecialKey: get knuckle from pull";
		hits2 = "LEFT: choise next knuckle   RIGHT: choise last knuckle   BACK: skip move";
	}
	else if (!table->GetIsFirstKnuckleSet())
	{
		hits1 = "NEXT: take knuckle   PULL: show pull";
		hits2 = "LEFT: choise next knuckle   RIGHT: choise last knuckle";
	}
	else
	{
		hits1 = "NEXT: take knuckle   PULL: show pull   SpecialKey: get knuckle from pull";
		hits2 = "LEFT: choise next knuckle   RIGHT: choise last knuckle";
	}


	keysHits[0] = hits1;
	keysHits[0].SetPositionStart(short((keysHits[0].GetPositionEnd().X - strlen(hits1)) / 2), keysHits[0].GetPositionStart().Y);
	keysHits[0].Print();
	keysHits[1] = hits2;
	keysHits[1].SetPositionStart(short((keysHits[1].GetPositionEnd().X - strlen(hits2)) / 2), keysHits[1].GetPositionStart().Y);
	keysHits[1].Print();

	//

	Print(table);



	//get correct pressed key
	CleanInputStream();
	for (short i = 0; i < Hand::GetCountOfHands(); i++)
		Print(players[i]);

	while (true)
	{
		const unsigned char* key = GetLastKey();
		if (memcmp(key, keyNext, 2 * sizeof(char)) == 0)
		{
			if (players[Hand::GetCurrentHandId()]->GetChosenKnuckleIndex() != None)
			{
				players[Hand::GetCurrentHandId()]->SetIsChosenKnuckleOnTheTable(true);
				players[Hand::GetCurrentHandId()]->SetArrowHighLight(HAND_ARROW_RIGHT, false);
				players[Hand::GetCurrentHandId()]->SetArrowHighLight(HAND_ARROW_LEFT, false);

				Print(players[Hand::GetCurrentHandId()]);
				//Hand::SetCurrentHandId(Hand::GetCurrentHandId() + 1);
				break;
			}
		}
		else if (memcmp(key, keyLeft, 2 * sizeof(char)) == 0)
		{
			players[Hand::GetCurrentHandId()]->SetArrowHighLight(HAND_ARROW_LEFT, true);
			players[Hand::GetCurrentHandId()]->SetArrowHighLight(HAND_ARROW_RIGHT, false);
			players[Hand::GetCurrentHandId()]->SetChosenKnuckleIndex(players[Hand::GetCurrentHandId()]->GetChosenKnuckleIndex() - 1);
			Print(players[Hand::GetCurrentHandId()]);
		}
		else if (memcmp(key, keyRight, 2 * sizeof(char)) == 0)
		{
			players[Hand::GetCurrentHandId()]->SetArrowHighLight(HAND_ARROW_LEFT, false);
			players[Hand::GetCurrentHandId()]->SetArrowHighLight(HAND_ARROW_RIGHT, true);
			players[Hand::GetCurrentHandId()]->SetChosenKnuckleIndex(players[Hand::GetCurrentHandId()]->GetChosenKnuckleIndex() + 1);
			Print(players[Hand::GetCurrentHandId()]);
		}
		else if (memcmp(key, keyRotate, 2 * sizeof(char)) == 0)
		{
			if (table->GetIsFirstKnuckleSet())
			{
				players[Hand::GetCurrentHandId()]->SetKnuckle(pull->GetKnuckle());
				players[Hand::GetCurrentHandId()]->SetChosenKnuckleIndex(-2);
				Print(players[Hand::GetCurrentHandId()]);
			}
		}

		else if (memcmp(key, keyBack, 2 * sizeof(char)) == 0)
		{
			if (pull->GetNumberOfRemainingKnuckles() == 0 && table->GetIsFirstKnuckleSet())
			{
				players[Hand::GetCurrentHandId()]->RefreshHand();
				Hand::SetCurrentHandId(Hand::GetCurrentHandId() + 1);
				return SCENE_GAME_CHOISE;
			}
		}
		else if (memcmp(key, keyPull, 2 * sizeof(char)) == 0)
		{
			Clean();
			return SCENE_PULL;
		}
	}
	//

	return SCENE_GAME_TABLE;
}

int GameController::SceneGameTable(void)
{
	//what everyone kay actually do
	keysHits[0].Clean();
	keysHits[1].Clean();

	const char* hits1;
	const char* hits2;
	if (table->GetIsFirstKnuckleSet())
	{
		hits1 = "KEY MOVES: LEFT, RIGHT, UP, DOWN";
		hits2 = "SpecialKey: change rotate   NEXT: put knuckle   BACK: rechoise knuckle";
	}
	else
	{
		hits1 = "NEXT: put knuckle   BACK: rechoise knuckle";
		hits2 = "";
	}

	keysHits[0] = hits1;
	keysHits[0].SetPositionStart(short((keysHits[0].GetPositionEnd().X - strlen(hits1)) / 2), keysHits[0].GetPositionStart().Y);
	keysHits[0].Print();
	keysHits[1] = hits2;
	keysHits[1].SetPositionStart(short((keysHits[1].GetPositionEnd().X - strlen(hits2)) / 2), keysHits[1].GetPositionStart().Y);
	keysHits[1].Print();
	//

	if (table->GetIsFirstKnuckleSet())
	{
		table->KnuckleRefresh();
		table->SetKnuckleParts(players[Hand::GetCurrentHandId()]->GetChosenKnuckle());
	}
	else
	{
		table->SetFirstKnuckle(players[Hand::GetCurrentHandId()]->GetChosenKnuckle());
	}

	//get correct pressed key
	CleanInputStream();
	while (true)
	{
		bool brk = false;
		Print(table);
		while (true)
		{
			const unsigned char* key = GetLastKey();

			if (memcmp(key, keyBack, 2 * sizeof(char)) == 0)
			{
				players[Hand::GetCurrentHandId()]->SetIsChosenKnuckleOnTheTable(false);
				if (!table->GetIsFirstKnuckleSet())
				{
					table->SetFirstKnuckle();
				}
				brk = true;
				break;
			}
			else if (memcmp(key, keyNext, 2 * sizeof(char)) == 0)
			{
				if (table->GetIsFirstKnuckleSet())
				{
					unsigned short network = table->AddKnuckleToTableImage(players[Hand::GetCurrentHandId()]->GetChosenKnuckle());
					if (network == 1)
					{
						brk = true;
						players[Hand::GetCurrentHandId()]->DeleteChosenKnuckle();
						players[Hand::GetCurrentHandId()]->AppendExtraPoints(table->GetExtraPoints());
						if (players[Hand::GetCurrentHandId()]->GetCountOfKnucklesInHand() == 0)
						{
							Clean();
							return SCENE_SCORE;
						}

						Hand::SetCurrentHandId(Hand::GetCurrentHandId() + 1);
					}
					else if (network == 2)
					{
						players[Hand::GetCurrentHandId()]->DeleteChosenKnuckle();
						Clean();
						return SCENE_SCORE;
					}
					break;
				}
				else
				{
					table->SetIsFirstKnuckleSet(true);
					unsigned int extraPoints = players[Hand::GetCurrentHandId()]->GetChosenKnuckle()->GetCountOfDotsOnBothPart();
					if (extraPoints % 5 == 0)
						players[Hand::GetCurrentHandId()]->AppendExtraPoints(extraPoints);
					players[Hand::GetCurrentHandId()]->DeleteChosenKnuckle();
					Hand::SetCurrentHandId(Hand::GetCurrentHandId() + 1);
					brk = true;
					break;
				}
			}
			if (table->GetIsFirstKnuckleSet())
			{
				if (memcmp(key, keyLeft, 2 * sizeof(char)) == 0)
				{
					table->KnuckleMove(LEFT);
					break;
				}
				else if (memcmp(key, keyRight, 2 * sizeof(char)) == 0)
				{
					table->KnuckleMove(RIGHT);
					break;
				}
				else if (memcmp(key, keyUp, 2 * sizeof(char)) == 0)
				{
					table->KnuckleMove(UP);
					break;
				}
				else if (memcmp(key, keyDown, 2 * sizeof(char)) == 0)
				{
					table->KnuckleMove(DOWN);
					break;
				}
				else if (memcmp(key, keyRotate, 2 * sizeof(char)) == 0)
				{
					table->KnuckleMove(ROTATE);
					break;
				}
			}
		}
		if (brk)
			break;

	}
	//
	table->KnuckleRefresh();


	keysHits[0].Clean();
	keysHits[1].Clean();

	return SCENE_GAME_CHOISE;
}

int GameController::ScenePull(void)
{
	//what everyone kay actually do
	const char* hits = "BACK: return to the game";
	keysHits[0] = hits;
	keysHits[0].SetPositionStart(short((keysHits[0].GetPositionEnd().X - strlen(hits)) / 2), keysHits[0].GetPositionStart().Y);
	keysHits[0].Print();
	//


	Print(pull);
	//get correct pressed key
	while (true)
	{
		const unsigned char* key = GetLastKey();
		if (memcmp(key, keyBack, 2 * sizeof(char)) == 0)
		{
			break;
		}
	}
	//
	Clean();
	return SCENE_GAME_CHOISE;
}

int GameController::SceneScore(void)
{
	const char* hits = "BACK: play new game     SpecialKey: replay this game";
	keysHits[0] = hits;
	keysHits[0].SetPositionStart(short((keysHits[0].GetPositionEnd().X - strlen(hits)) / 2), keysHits[0].GetPositionStart().Y);
	keysHits[0].Print();

	//get last correct score
	unsigned int score = 0;
	for (int i = 0; i < Hand::GetCountOfHands(); i++)
	{
		if (players[Hand::GetCurrentHandId()] != players[i])
			score += players[i]->GetOtherKnucklesDots();
	}
	score -= players[Hand::GetCurrentHandId()]->GetOtherKnucklesDots();
	score = unsigned int(ceil(score / 5.0)) * 5;
	players[Hand::GetCurrentHandId()]->SetPlayerScore(score);
	//

	//print score with animation
	int textSize = PLAYER_NAME_SIZE + 3 + 3;
	COORD cursorStartPrint{ (GetConsoleSize().X - textSize) / 2,GetConsoleSize().Y / 2 - Hand::GetCountOfHands() * 3 }, cursorStartHead = cursorStartPrint;
	cursorStartHead.X += SHORT((textSize - strlen("SCORE:")) / 2);

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFOEX consoleInfo;
	consoleInfo.cbSize = sizeof(consoleInfo);
	SetConsoleCursorPosition(hStdout, cursorStartHead);
	printf("SCORE:");
	for (int i = 0; i < Hand::GetCountOfHands(); i++)
	{
		cursorStartPrint.Y += 3;
		SetConsoleCursorPosition(hStdout, cursorStartPrint);
		for (int j = 0; j < int((textSize - strlen(players[i]->GetPlayerName())) / 2 - 1); j++)
			putchar(' ');
		printf("%s - ",players[i]->GetPlayerName());
		GetConsoleScreenBufferInfoEx(hStdout, &consoleInfo);
		for (unsigned int j = 0; j <= players[i]->GetPlayerScore(); j++)
		{
			SetConsoleCursorPosition(hStdout,consoleInfo.dwCursorPosition);
			printf("%u", j);
			if (players[i]->GetPlayerScore() > 0)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(unsigned int(20 - j / (double)players[i]->GetPlayerScore() * 20)));
			}
		}
	}
	//

	//get correct pressed key
	CleanInputStream();
	while (true)
	{
		const unsigned char* key = GetLastKey();
		if (memcmp(key, keyBack, 2 * sizeof(char)) == 0)
		{
			Clean();
			ControllerClean();
			Hand::SetCurrentHandId(0);
			return SCENE_MENU;
		}
		else if (memcmp(key, keyRotate, 2 * sizeof(char)) == 0)
		{
			Clean();
			return  SCENE_CONFIG;
		}
	}
	//
}


int GameController::SceneRules(void)
{
	//just text
	const char* text = "The game uses knuckles (dominoes) in the shape of rectangles, at both      "
		"ends of which there are a number of points from 0 to 6. The game set       "
		"includes knuckles with each combination of points and each                 "
		"combination occurs exactly once.At the beginning,                          "
		"each player receives 7 knuckles and one of the                             "
		"participants is randomly selected as thestarter.He lays out any of his     "
		"knuckles, starting the so - called a chain to the ends of which the players"
		"then add one domino one after the other, but in such a way that the        "
		"number of stitches at the ends of the touching knuckles must be the same.If"
		"a player does not have a knuckle that can be placed at one end of the      "
		"chain, he must take the dominoes from the unpaidand add them; if           "
		"the downloaded dominoes cannot be added, they will take another            "
		"one until they are used up or until the unspent knuckles are exhausted(if  "
		"they are exhaustedand unable to add anything, they lose the queue).        "
		"Players score points if, after placing a knuckle, the total number of      "
		"points at the ends of the chain is divisible by five; he then gets a       "
		"number of points equal to this sum.In the case of doubles, the             "
		"stitches on both ends of the domino count for the total.When the player has"
		"placed his last domino, the round is over; that                            "
		"player wins the roundand scores a number of points equal to the total      "
		"of the opponent\'s unused blocks, rounded to the nearest five.";

	const char* sizeSet = "knuckles, starting the so - called a chain to the ends of which the players";
	COORD console = GetConsoleSize();
	TextField rules;
	rules = text;
	rules.SetPositionStart(short((console.X - strlen(sizeSet)) / 2), short((console.Y -24) / 2));
	rules.SetPositionEnd(short((console.X - strlen(sizeSet)) / 2)+ strlen(sizeSet)-1,(short) console.Y);
	rules.Print();
	//
	
	//what everyone kay actually do
	const char* hits = "BACK: return to the menu";
	keysHits[0] = hits;
	keysHits[0].SetPositionStart(short((keysHits[0].GetPositionEnd().X - strlen(hits)) / 2), keysHits[0].GetPositionStart().Y);
	keysHits[0].Print();
	//

	//get correct pressed key
	while (true)
	{
		const unsigned char* key = GetLastKey();
		if (memcmp(key, keyBack, 2 * sizeof(char)) == 0)
		{
			Clean();
			return SCENE_MENU;
		}
	}
	//

}
//End of this Area
