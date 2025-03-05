// Engine includes
#include "EventKeyboard.h"
#include "GameManager.h"
#include "ResourceManager.h"

// Game includes
#include "GameStart.h"
#include "Mouse.h"
#include "Player.h"
#include "Speed.h"
#include "AtkSpeed.h"
#include "Health.h"

GameStart::GameStart() {

	// Play start music
	p_music = RM.getMusic("start music");
	playMusic();

	// Setup type, sprite, and location
	setType("GameStart");
	setSprite("gamestart");
	setLocation(df::CENTER_CENTER);

	// Register for keyboard events
	registerInterest(df::KEYBOARD_EVENT);
}

int GameStart::eventHandler(const df::Event* p_e) {
	df::Sound* p_sound = RM.getSound("quit");

	if (p_e->getType() == df::KEYBOARD_EVENT) {
		df::EventKeyboard* p_keyboard_event = (df::EventKeyboard*)p_e;
		switch (p_keyboard_event->getKey()) {
		case df::Keyboard::P:	// play
			start();
			break;
		case df::Keyboard::Q:	// quit
			if (p_sound) {
				p_sound->play();
			}
			GM.setGameOver();
			break;
		default:	// Key is not handled
			break;
		}
		
		return 1;
	}

	// If get here, have ignored this event
	return 0;
}

int GameStart::draw() {
	return df::Object::draw();
}

void GameStart::start() {

	df::Sound* p_sound = RM.getSound("select");
	if (p_sound) {
		p_sound->play();
	}

	// Spawn some mice to shoot
	for (int i = 0; i < 16; i++) {
		new Mouse;
	}
	new Player;
	new Health;
	new Speed;
	new AtkSpeed;

	// Setup heads-up display
	//new Points;									// points display
	//df::ViewObject* p_vo = new df::ViewObject;	// count of nukes
	//p_vo->setLocation(df::TOP_LEFT);
	//p_vo->setViewString("Nukes");
	//p_vo->setValue(1);
	//p_vo->setColor(df::YELLOW);

	// When game starts, become inactive
	setActive(false);

	// Pause start music
	p_music->pause();
}

void GameStart::playMusic() {
	if (p_music)
		p_music->play();
}