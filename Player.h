#pragma once
#include <map>
#include <string>
#include "Equipment.h"

Armor StartArmor = Armor("Грязные тряпки", 0);
Weapon StartWeapon = Weapon("Вилы", MELEE, 5);

class Player
{
public:

	int HP = 15;
	int MaxHP = 20;
	int STR = 3;
	int DEX = 3;
	std::map<std::string, int> Inventory = { {"Зелье здоровья", 0}, {"Бомба", 0}, {"Серебряный ключ", 0}, {"Золотой ключ", 0} };
	Weapon* PlayerWeapon = &StartWeapon;
	Armor* PlayerArmor = &StartArmor;

	void Heal(int amount) { HP = (HP + amount) > MaxHP ? MaxHP : HP + amount; }

	void AddSTR(int amount) { STR = (STR + amount) > 20 ? 20 : STR + amount; }

	void AddDEX(int amount) { DEX = (DEX + amount) > 20 ? 20 : DEX + amount; }

};