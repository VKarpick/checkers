#include "checkers_player.h"



CheckersPlayer::CheckersPlayer(char pChar, int direction, bool isUser) :
	playerChar(pChar),
	verticalDirection(direction),
	isUserControlled(isUser)
{}


bool CheckersPlayer::hasPiece(char piece) {
	return tolower(piece) == playerChar;
}
