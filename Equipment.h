#pragma once
#include <string>
#include <vector>


enum WeaponType { MELEE, RANGED };

class Weapon
{
public:
	WeaponType Weapontype;
	std::string Name;
	int Damage;

	Weapon(std::string Name, WeaponType Weapontype, int Damage)
	{
		this->Name = Name;
		this->Weapontype = Weapontype;
		this->Damage = Damage;
	}
};

class Armor
{
public:
	std::string Name;
	int DefencePoints;

	Armor(std::string Name, int DefencePoints)
	{
		this->Name = Name;
		this->DefencePoints = DefencePoints;
	}
};

std::vector<Weapon> WeaponList { Weapon("Кинжал", WeaponType::MELEE, 3), Weapon("Огромный, мать его, топор", WeaponType::MELEE, 10),
								 Weapon("Магический посох", WeaponType::MELEE, 4), Weapon("Деревянный лук", WeaponType::RANGED, 7) };

std::vector<Armor> ArmorList{ Armor("Стальная кольчуга", 3), Armor("Алмазный сет", 7) };

Weapon* GetRandomWeapon() { return &WeaponList[rand() % WeaponList.size()]; }

Armor* GetRandomArmor() { return &ArmorList[rand() % ArmorList.size()]; }
