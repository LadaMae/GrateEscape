// GrateEscape.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Player.h"
#include "Mouse.h"
#include "GameStart.h"

void loadResources(void);

void loadResources(void) {
    RM.loadSprite("sprites/player-spr.txt", "player");
    RM.loadSprite("sprites/mouse-spr.txt", "mouse");
    RM.loadSprite("sprites/bullet-spr.txt", "bullet");
    RM.loadSprite("sprites/gamestart-spr.txt", "gamestart");
    RM.loadSprite("sprites/gameover-spr.txt", "gameover");

    RM.loadMusic("sounds/start-music.wav", "start music");
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

    // Spawn GameStart object (will spawn everything else)
    new GameStart();

    // Load Sounds
    RM.loadSound("sounds/fire.wav", "fire");
    RM.loadSound("sounds/level-up.wav", "level up");
    RM.loadSound("sounds/select.wav", "select");
    RM.loadSound("sounds/quit.wav", "quit");

    GM.run();

    GM.shutDown();
    return 0;
}
