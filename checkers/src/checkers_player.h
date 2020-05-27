#pragma once

#include <string>



class CheckersPlayer {
public:
	CheckersPlayer(char pChar, int direction, bool isUser);
	

	char playerChar;
	int verticalDirection;
	bool isUserControlled;

	
	bool hasPiece(char piece);
};
