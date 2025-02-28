// GrateEscape.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"

void loadResources(void);

void loadResources(void) {
    RM.loadSprite("sprites/player-spr.txt", "player");
}

int main()
{
    if (GM.startUp()) {
        LM.writeLog("Error Starting Game Manager");
        GM.shutDown();
        return 1;
    }

    LM.setFlush();

    loadResources();

    GM.run();

    GM.shutDown();
    return 0;
}
