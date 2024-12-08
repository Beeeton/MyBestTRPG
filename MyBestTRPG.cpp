#include <ctime>
#include "GameState.h"
#include "IOManager.h"


int main()
{
    srand(time(0));

    GameState Game;

    IOManager IOmanager(&Game);

    IOmanager.StartGame();

    while (true)
    {
        system("cls");

        switch (Game.Menufocus)
        {
        case MenuFocus::ROOM:
            IOmanager.RoomMenu.Print();
            break;
        
        case MenuFocus::INVENTORY:
            IOmanager.InventoryMenu.UpdateButtons();
            IOmanager.InventoryMenu.Print();
            break;

        case MenuFocus::EVENT:
            for (EventType ET : Game.GetCurrentRoom()->Events) IOmanager.PlayEvent(ET);
            Game.GetCurrentRoom()->Events.clear();
            Game.Menufocus = MenuFocus::ROOM;
            break;
        }
    }

}

