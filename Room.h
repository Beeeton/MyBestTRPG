#pragma once
#include <string>
#include <vector>

enum ExitType { WALL, NEXTROOM, EXIT };

enum EventType { FIGHT, TRAP, CHEST, FOUNDEXIT };

std::vector<std::string> RoomDescriptions = 
					{"Эта комната выглядит даже хуже предыдущей. Да и пахнет так же.", 
					"В этой комнате почему-то теплее, чем во всех остальных.",
					"В этой комнате почему-то холоднее, чем во всех остальных.",
					"На полу этой комнаты видны свежие следы. Кто-то здесь недавно бывал.",
					"Находясь в этой комнате, вы размышляете о том, как оказались здесь.\n Вы пытаетесь вспомнить хоть что-то, но безуспешно."};

class Room
{
public:
	std::string Description;
	std::vector<int> RoomExits{ 0, 0, 0 };
	std::vector<EventType> Events;

	Room()
	{
		Description = RoomDescriptions[rand() % RoomDescriptions.size()];
		RoomExits[0] = rand() % 2;
		RoomExits[1] = rand() % 2;
		RoomExits[2] = rand() % 2;

		if ((rand() % 10) == 0) RoomExits[1] = EXIT;

		if (RoomExits[0] == WALL && RoomExits[1] != NEXTROOM && RoomExits[2] == WALL) RoomExits[0] = NEXTROOM;

		if ((rand() % 4) == 0) Events.push_back(EventType::TRAP);
		if ((rand() % 3) == 0) Events.push_back(EventType::FIGHT);
		if ((rand() % 4) == 0) Events.push_back(EventType::CHEST);

	}
};