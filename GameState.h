#pragma once

#include "Player.h"
#include "Room.h"
#include "Enemy.h"

enum MenuFocus { ROOM, INVENTORY, EVENT };

class GameState
{
private:
	Player player_;
	Room currentroom_;

public:

	Enemy enemy = GetRandomEnemy();
	bool PlayerTurn;

	MenuFocus Menufocus = ROOM;
	
	Player* GetPlayer() { return &player_; }

	Room* GetCurrentRoom() { return &currentroom_; }

	void GenerateNewRoom() { currentroom_ = Room(); }

};