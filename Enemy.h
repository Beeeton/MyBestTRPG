#pragma once
#include <string>
#include <vector>

class Enemy
{
public:
	std::string Name;
	int HP;
	int Damage;

	Enemy(std::string Name, int HP, int Damage) 
	{
		this->Name = Name;
		this->HP = HP;
		this->Damage = Damage;
	}
};

std::vector<Enemy> EnemyList{ Enemy("Скелет", 10, 5), Enemy("Большой червяк", 5, 2),
							  Enemy("Минотавр", 20, 8), Enemy("Слизень", 6, 3) };

Enemy GetRandomEnemy() { return EnemyList[rand() % EnemyList.size()]; }