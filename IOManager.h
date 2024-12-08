#pragma once
#include <string>
#include <vector>
#include <conio.h>
#include <iostream>
#include "Enemy.h"

enum SelectState { NONE, SELECTED, ABORTED };

class InputGrid
{
public:
	GameState* Gamestate;
	int InputGridSelector = 1;
	SelectState Selectstate = NONE;
	std::vector<std::string> Buttons;

	InputGrid() {}

	InputGrid(GameState* GS) { Gamestate = GS; }

	virtual void OnButtonSelected(std::string ButtonName) {}
	virtual void OnAborted() {}

	void ReadInput()
	{
		int Input = int(_getch());
		switch (Input)
		{
		case 115: //w
			InputGridSelector = InputGridSelector + 1 > Buttons.size() ? 1 : InputGridSelector + 1;
			Selectstate = NONE;
			break;

		case 119: //s
			InputGridSelector = InputGridSelector - 1 < 1 ? Buttons.size() : InputGridSelector - 1;
			Selectstate = NONE;
			break;
		case 13: //enter
			Selectstate = SELECTED;
			break;

		case 27: //esc
			Selectstate = ABORTED;
			break;

		}

		switch (Selectstate)
		{
		case SELECTED:
			OnButtonSelected(Buttons[InputGridSelector - 1]);
			break;
		case ABORTED:
			OnAborted();
			break;

		}
	}

	virtual void Print()
	{
		//print table
		for (int i = 0; i < Buttons.size(); i++)
		{
			std::cout << Buttons[i] + (InputGridSelector == i + 1 ? std::string(" <--") : std::string("")) << std::endl;
		}

		ReadInput();
	}
};

class RoomInputGrid : public InputGrid
{
public:
	RoomInputGrid(GameState* GS) { Buttons = { "a", "b", "c" }; Gamestate = GS; }

	virtual void Print()
	{
		std::cout << Gamestate->GetCurrentRoom()->Description << std::endl << std::endl;

		for (int i = 0; i < Buttons.size(); i++)
		{
			std::cout << Buttons[i] + (InputGridSelector == i + 1 ? std::string(" <--") : std::string("")) << std::endl;
		}

		ReadInput();
	}

	void UpdateButtons(Room* room)
	{
		Buttons.clear();
		if (room->RoomExits[0] == ExitType::NEXTROOM) Buttons.push_back("Войти в комнату слева");
		if (room->RoomExits[0] == ExitType::EXIT) Buttons.push_back("Подойти к золотой двери");
		if (room->RoomExits[1] == ExitType::NEXTROOM) Buttons.push_back("Войти в комнату спереди");
		if (room->RoomExits[1] == ExitType::EXIT) Buttons.push_back("Подойти к золотой двери");
		if (room->RoomExits[2] == ExitType::NEXTROOM) Buttons.push_back("Войти в комнату справа");
		if (room->RoomExits[2] == ExitType::EXIT) Buttons.push_back("Подойти к золотой двери");
		Buttons.push_back("Открыть Инветарь");
	}


	virtual void OnButtonSelected(std::string ButtonName)
	{
		if (ButtonName == "Войти в комнату слева" || ButtonName == "Войти в комнату спереди" || ButtonName == "Войти в комнату справа")
		{
			InputGridSelector = 1;
			Gamestate->GenerateNewRoom();
			this->UpdateButtons(Gamestate->GetCurrentRoom());
			if(Gamestate->GetCurrentRoom()->Events.size() != 0) Gamestate->Menufocus = MenuFocus::EVENT;
		}
		if (ButtonName == "Открыть Инветарь")
		{
			Gamestate->Menufocus = MenuFocus::INVENTORY;
		}
		if (ButtonName == "Подойти к золотой двери")
		{
			Gamestate->Menufocus = MenuFocus::EVENT;
			Gamestate->GetCurrentRoom()->Events.push_back(EventType::FOUNDEXIT);
		}
	}

};

class InventoryInputGrid : public InputGrid
{
public:
	InventoryInputGrid(GameState* GS) { Buttons = { "a", "b", "c" }; Gamestate = GS; }

	void UpdateButtons()
	{
		Buttons.clear();
		if (Gamestate->GetPlayer()->Inventory["Зелье здоровья"] != 0) Buttons.push_back("Зелье здоровья");
		if (Gamestate->GetPlayer()->Inventory["Бомба"] != 0) Buttons.push_back("Бомба");
		if (Gamestate->GetPlayer()->Inventory["Серебряный ключ"] != 0) Buttons.push_back("Серебряный ключ");
		if (Gamestate->GetPlayer()->Inventory["Золотой ключ"] != 0) Buttons.push_back("Золотой ключ");
	}

	virtual void OnAborted()
	{
		InputGridSelector = 1;
		Gamestate->Menufocus = MenuFocus::ROOM;
	}

	virtual void OnButtonSelected(std::string ButtonName)
	{
		if (ButtonName == "Зелье здоровья")
		{
			Gamestate->GetPlayer()->Inventory["Зелье здоровья"] -= 1;
			Gamestate->GetPlayer()->Heal(10);
		}
	}

	virtual void Print()
	{

		std::cout << "Ваш инвентарь:" << std::endl << std::endl;

		if (!(Gamestate->GetPlayer()->Inventory["Зелье здоровья"] == 0 && Gamestate->GetPlayer()->Inventory["Бомба"] == 0 &&
			Gamestate->GetPlayer()->Inventory["Серебряный ключ"] == 0 && Gamestate->GetPlayer()->Inventory["Золотой ключ"] == 0))
		{
			for (int i = 0; i < Buttons.size(); i++)
			{
				std::cout << Buttons[i] + std::string(18 - Buttons[i].length(), ' ') + std::to_string(Gamestate->GetPlayer()->Inventory[Buttons[i]]) + (InputGridSelector == i + 1 ? std::string(" <--") : std::string("")) << std::endl;
			}

			if (Buttons[InputGridSelector - 1] == "Зелье здоровья") std::cout << std::endl << "Выпейте, чтобы восстановить 10 единиц здоровья." << std::endl;
			if (Buttons[InputGridSelector - 1] == "Бомба") std::cout << std::endl << "Бросье во врага, чтобы моментально убить его." << std::endl;
			if (Buttons[InputGridSelector - 1] == "Серебряный ключ") std::cout << std::endl << "Открывает сундуки." << std::endl;
			if (Buttons[InputGridSelector - 1] == "Золотой ключ") std::cout << std::endl << "Открывает выход из подземелья." << std::endl;
		}
		else std::cout << "Ваши карманы пусты!" << std::endl;

		std::cout << std::endl << "Ваши характеристики:" << std::endl << std::endl
			<< "Здоровье: " << Gamestate->GetPlayer()->HP << "   (Макс. " << Gamestate->GetPlayer()->MaxHP << ")" << std::endl
			<< "Сила: " << Gamestate->GetPlayer()->STR << "   (Макс. 20)" << "   Ловкость: " << Gamestate->GetPlayer()->DEX << "   (Макс. 20)"
			<< std::endl << "Оружие: " << Gamestate->GetPlayer()->PlayerWeapon->Name << ", " << Gamestate->GetPlayer()->PlayerWeapon->Damage << " урона"
			<< (Gamestate->GetPlayer()->PlayerWeapon->Weapontype == WeaponType::MELEE ? " (Ближний бой)" : " (Дальний бой)") << std::endl
			<< "Броня: " << Gamestate->GetPlayer()->PlayerArmor->Name << ", " << Gamestate->GetPlayer()->PlayerArmor->DefencePoints << " защиты";

		if (!(Gamestate->GetPlayer()->Inventory["Зелье здоровья"] == 0 && Gamestate->GetPlayer()->Inventory["Бомба"] == 0 &&
			Gamestate->GetPlayer()->Inventory["Серебряный ключ"] == 0 && Gamestate->GetPlayer()->Inventory["Золотой ключ"] == 0))
		{
			ReadInput();
		}
		else
		{
			while (_getch() != 27) {}
			OnAborted();
		}
	}

};

class FightInputGrid : public InputGrid
{
public:
	FightInputGrid(GameState* GS) { Buttons = { "a", "b", "c" }; Gamestate = GS; }

	void UpdateButtons()
	{
		Buttons.clear();
		Buttons.push_back("Атаковать");
		if (Gamestate->GetPlayer()->Inventory["Зелье здоровья"] != 0) Buttons.push_back("Выпить зелье здоровья");
		if (Gamestate->GetPlayer()->Inventory["Бомба"] != 0) Buttons.push_back("Бросить бомбу");
	}

	virtual void OnButtonSelected(std::string ButtonName)
	{
		system("cls");

		if (ButtonName == "Атаковать")
		{
			std::cout << "Вы атакуете врага и наносите ему " <<
				Gamestate->GetPlayer()->PlayerWeapon->Damage + (Gamestate->GetPlayer()->PlayerWeapon->Weapontype == WeaponType::MELEE ?
					Gamestate->GetPlayer()->STR : Gamestate->GetPlayer()->DEX)
				<< " урона." << std::endl;
			Gamestate->enemy.HP -= Gamestate->GetPlayer()->PlayerWeapon->Damage + (Gamestate->GetPlayer()->PlayerWeapon->Weapontype == WeaponType::MELEE ?
				Gamestate->GetPlayer()->STR : Gamestate->GetPlayer()->DEX);
			if (Gamestate->enemy.HP <= 0) std::cout << std::endl << Gamestate->enemy.Name << " умирает." << std::endl;
		}
		if (ButtonName == "Выпить зелье здоровья")
		{
			std::cout << "Вы пьёте зелье здоровья и восстанавливаете 10 единиц здоровья." << std::endl;
			Gamestate->GetPlayer()->Heal(10);
			Gamestate->GetPlayer()->Inventory["Зелье здоровья"] -= 1;
		}
		if (ButtonName == "Бросить бомбу")
		{
			std::cout << "Вы бросаете бомбу врагу под ноги. " << Gamestate->enemy.Name << " разлетается на части." << std::endl;
			Gamestate->GetPlayer()->Inventory["Бомба"] -= 1;
			Gamestate->enemy.HP = 0;
		}

		std::cout << std::endl << "Продолжить <--";
		while (_getch() != 13) {}

		Gamestate->PlayerTurn = false;
	}

	virtual void Print()
	{
		if (Gamestate->PlayerTurn)
		{
			std::cout << "Что вы будете делать?" << std::endl;
			for (int i = 0; i < Buttons.size(); i++)
			{
				std::cout << Buttons[i] + (InputGridSelector == i + 1 ? std::string(" <--") : std::string("")) << std::endl;
			}
			std::cout << std::endl << "Ваше здоровье: " << Gamestate->GetPlayer()->HP << std::endl
				<< "Здоровье врага: " << Gamestate->enemy.HP << std::endl;
		}
		else
		{
			std::cout << Gamestate->enemy.Name << " атакует! И наносит вам " << Gamestate->enemy.Damage - Gamestate->GetPlayer()->PlayerArmor->DefencePoints << " урона!"
				<< std::endl << std::endl << "Продолжить <--";
			Gamestate->GetPlayer()->HP -= Gamestate->enemy.Damage - Gamestate->GetPlayer()->PlayerArmor->DefencePoints;
			while (_getch() != 13) {}

			Gamestate->PlayerTurn = true;
			return;
		}

		if (Gamestate->PlayerTurn) ReadInput();
	}
};

class IOManager
{
public:

	GameState* Gamestate;
	RoomInputGrid RoomMenu = RoomInputGrid(Gamestate);
	InventoryInputGrid InventoryMenu = InventoryInputGrid(Gamestate);
	FightInputGrid FightMenu = FightInputGrid(Gamestate);

	IOManager(GameState* GS) 
	{ 
		Gamestate = GS; 

		RoomMenu = RoomInputGrid(Gamestate);
		RoomMenu.UpdateButtons(Gamestate->GetCurrentRoom());
		InventoryMenu = InventoryInputGrid(Gamestate);
		FightMenu = FightInputGrid(Gamestate);

	}

	void StartGame()
	{
		system("chcp 1251");
		system("cls");

		std::cout << "MyBestTRPG v1.0.0" << std::endl << std::endl << "Made by MorunovAD" << std::endl << std::endl
			<< "Для управления используйте клавиши W, S, Enter и Esc" << std::endl
			<< "Используйте английскую раскладку клавиатуры!" <<
			std::endl << std::endl << "Начать <--";

		while (_getch() != 13) {}

		system("cls");

		std::cout << "Вы приходите в сознание в незнакомой вам тёмной комнате. " << std::endl
			<< "Рядом с вами лежат ваши верные вилы, которыми вы работали в поле пока не оказались здесь." << std::endl << std::endl
			<< "Взять вилы и встать на ноги <--";

		while (_getch() != 13) {}

		system("cls");

		std::cout << "Вы осматриваетесь и понимаете, что находитесь глубоко в неизвестном вам подземелье." << std::endl
			<< "Комната в которой вы находитесь не заперта." << std::endl << std::endl;

		switch (rand() % 4)
		{
		case 0:
			std::cout << "А ещё вы нашли одно зелье здоровья!" << std::endl << std::endl;
			Gamestate->GetPlayer()->Inventory["Зелье здоровья"] += 1;
			break;
		case 1:
			std::cout << "А ещё вы нашли одну бомбу!" << std::endl << std::endl;
			Gamestate->GetPlayer()->Inventory["Бомба"] += 1;
			break;
		case 2:
			std::cout << "А ещё вы нашли один серебряный ключ!" << std::endl << std::endl;
			Gamestate->GetPlayer()->Inventory["Серебряный ключ"] += 1;
			break;
		}

		std::cout << "Вам нужно выбираться." << std::endl << std::endl << "Войти в следующую комнату <--";

		while (_getch() != 13) {}

	}

	void KillPlayer()
	{
		system("cls");

		std::cout << "Ваши раны оказались слишком тяжелыми. Вы погибаете, не сумев выбраться из подземелья." << std::endl
			<< "В свои последние моменты вы думаете о том, могло ли всё закончится иначе?" << std::endl << std::endl
			<< "Вы теряете сознание, не успев дать ответа на свой вопрос." << std::endl << std::endl;

		exit(0);
	}

	void PlayEvent(EventType ET)
	{
		system("cls");

		switch (ET)
		{
		case EventType::CHEST:
			std::cout << "Вы углу комнаты стоит большой деревянный сундук. На нём висит серебряный замок." << std::endl << std::endl;

			if (Gamestate->GetPlayer()->Inventory["Серебряный ключ"] == 0)
			{
				std::cout << "У вас нет ключа, который мог бы открыть этот замок." << std::endl << std::endl
					<< "Отойти от сундука <--";

				while (_getch() != 13) {}

				break;
			}
			else
			{
				Gamestate->GetPlayer()->Inventory["Серебряный ключ"] -= 1;
				std::cout << "Вы открываете его серебряным ключом и внутри находите: " << std::endl << std::endl;

				switch (rand() % 3)
				{
				case 0:
					std::cout << "Золотой ключ!" << std::endl << std::endl << "Забрать ключ и отойти от сундука <--";
					Gamestate->GetPlayer()->Inventory["Золотой ключ"] += 1;

					while (_getch() != 13) {}

					break;

				case 1:
					if (rand() % 2)
					{
						Armor* ArmorLoot = GetRandomArmor();
						std::cout << "Броню! А именно " << ArmorLoot->Name << ", " << ArmorLoot->DefencePoints << " защиты.";
						if (ArmorLoot->DefencePoints > Gamestate->GetPlayer()->PlayerArmor->DefencePoints)
						{
							std::cout << std::endl << "Это явно лучше ваших прежних тряпок." << std::endl << std::endl
								<< "Надеть доспех и отойти от сундука <--";
							Gamestate->GetPlayer()->PlayerArmor = ArmorLoot;
							while (_getch() != 13) {}
						}
						else
						{
							std::cout << std::endl << "Вы не оценили качество вашей добычи, и пожалели о потраченном ключе." << std::endl << std::endl
								<< "Оставить доспех и отойти от сундука <--";
							while (_getch() != 13) {}
						}
					}
					else
					{
						Weapon* WeaponLoot = GetRandomWeapon();
						std::cout << "Оружие! А именно " << WeaponLoot->Name << ", " << WeaponLoot->Damage << " урона.";
						if (WeaponLoot->Damage > Gamestate->GetPlayer()->PlayerWeapon->Damage)
						{
							std::cout << std::endl << "Это явно лучше вашей прежней палки." << std::endl << std::endl
								<< "Взять оружие и отойти от сундука <--";
							Gamestate->GetPlayer()->PlayerWeapon = WeaponLoot;
							while (_getch() != 13) {}
						}
						else
						{
							std::cout << std::endl << "Вы не оценили качество вашей добычи, и пожалели о потраченном ключе." << std::endl << std::endl
								<< "Оставить оружие и отойти от сундука <--";
							while (_getch() != 13) {}
						}
					}
					break;

				case 2:
					if (rand() % 2)
					{
						std::cout << "Амулет силы! +3 силы!" << std::endl << std::endl
							<< "Надеть амулет и отойти от сундука <--";
						Gamestate->GetPlayer()->AddSTR(3);
						while (_getch() != 13) {}
					}
					else
					{
						std::cout << "Амулет ловкости! +3 ловкости!" << std::endl << std::endl
							<< "Надеть амулет и отойти от сундука <--";
						Gamestate->GetPlayer()->AddDEX(3);
						while (_getch() != 13) {}
					}
					break;
				}
			}
			break;

		case EventType::TRAP:
			std::cout << "Вы наступили на ловушку! Ржавые стрелы летят в вашу сторону!" << std::endl << std::endl
				<< "Попытаться увернуться <--";

			while (_getch() != 13) {}

			if ((((rand() % 20) + 1) + Gamestate->GetPlayer()->DEX) > 10)
			{
				std::cout << std::endl << std::endl << "Вы успешно увернулось от всех летящих прямо в вас стрел. Невероятно!" << std::endl << std::endl
					<< "Продолжить <--";
				while (_getch() != 13) {}
			}
			else
			{
				std::cout << std::endl << std::endl << "Ну... Теперь в вашем теле на несколько дыр больше. Наверняка вы найдёте им достойное применение."
					<< std::endl << std::endl << "Потеряно 5 единиц здоровья." << std::endl << std::endl
					<< "Продолжить <--";
				Gamestate->GetPlayer()->HP -= 5;

				while (_getch() != 13) {}

				if (Gamestate->GetPlayer()->HP <= 0) this->KillPlayer();
			}

			break;

		case EventType::FIGHT:
			Gamestate->enemy = GetRandomEnemy();
			std::cout << Gamestate->enemy.Name << " преградил вам путь! Время боя!" << std::endl;
			Gamestate->PlayerTurn = Gamestate->GetPlayer()->PlayerWeapon->Weapontype == WeaponType::RANGED;
			if (!Gamestate->PlayerTurn) Gamestate->PlayerTurn = rand() % 2;
			if (Gamestate->PlayerTurn) std::cout << "Вы делаете первый ход!" << std::endl << std::endl;
			else std::cout << "Противник ходит первым!" << std::endl << std::endl;
			std::cout << "Начать бой <--";
			while (_getch() != 13) {}
			
			FightMenu.InputGridSelector = 1;
			while (true)
			{
				system("cls");
				FightMenu.UpdateButtons();
				FightMenu.Print();
				if (Gamestate->GetPlayer()->HP <= 0) KillPlayer();
				if (Gamestate->enemy.HP <= 0) break;
			}

			switch (rand() % 4)
			{
			case 0:
				system("cls");
				std::cout << "Побеждённый враг обронил предмет! - Зелье здоровья!" << std::endl << std::endl
					<< "Подобрать зелье и пойти дальше <--";
				Gamestate->GetPlayer()->Inventory["Зелье здоровья"] += 1;
				while (_getch() != 13) {}
				break;
			case 1:
				system("cls");
				std::cout << "Побеждённый враг обронил предмет! - Серебряные ключи! Целых 2!" << std::endl << std::endl
					<< "Подобрать ключи и пойти дальше <--";
				Gamestate->GetPlayer()->Inventory["Серебряный ключ"] += 2;
				while (_getch() != 13) {}
				break;
			case 2:
				system("cls");
				std::cout << "Побеждённый враг обронил предмет! - Бомба!" << std::endl << std::endl
					<< "Подобрать бомбу и пойти дальше <--";
				Gamestate->GetPlayer()->Inventory["Бомба"] += 1;
				while (_getch() != 13) {}
				break;
			case 3:
				system("cls");
				std::cout << "Побеждённый враг обронил предмет! - Серебряный ключ!" << std::endl << std::endl
					<< "Подобрать ключ и пойти дальше <--";
				Gamestate->GetPlayer()->Inventory["Серебряный ключ"] += 1;
				while (_getch() != 13) {}
				break;
			}

			break;

		case EventType::FOUNDEXIT:
			
			std::cout << "Вы подходите к большой двери, выделанной драгоценными камнями. Вот бы утащить её с собой." << std::endl
				<< "В центре двери вы видете замочную скважину. Это точно выход отсюда." << std::endl << std::endl;

			if (Gamestate->GetPlayer()->Inventory["Золотой ключ"] == 0)
			{
				std::cout << "У вас нет ключа, который мог бы открыть эту дверь." << std::endl << std::endl
					<< "Отойти от двери <--";

				while (_getch() != 13) {}

				break;
			}
			else
			{
				std::cout << "Вы открываете дверь золотым ключом. Когда вы распахиваете дверь, вас ослепляет солнечный свет."
					<< std::endl << std::endl << "Вы оказываетесь у подножья горы. Это место кажется вам знакомым." << std::endl
					<< std::endl << "Вы вынимаете золотой ключ из двери и ложите его в карман, планируя обменять его на новые вилы, и уходите в поле."
					<< std::endl << std::endl << "Вы спаслись из подземелья. Жизнь налаживается!" << std::endl << std::endl;

				exit(0);
			}
		}
	}

};
