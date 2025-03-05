#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "DisplayManager.h"
#include "GameManager.h"
#include "EventStep.h"
#include "EventView.h"
#include "EventPowerUp.h"

#include "Player.h"
#include "Bullet.h"

Player::Player() {
	setSprite("player");

	registerInterest(df::KEYBOARD_EVENT);
	registerInterest(df::STEP_EVENT);
	registerInterest(POWER_UP_EVENT);
	
	setType("Player");
	
	df::Vector p(WM.getBoundary().getHorizontal() / 2, WM.getBoundary().getVertical() / 2);
	setPosition(p);

	p_reticle = new Reticle();
	p_reticle->draw();

	move_slowdown = 2;
	move_countdown = move_slowdown;
	fire_slowdown = 30;
	fire_countdown = fire_slowdown;

	int current_xp = 0;
}

Player::~Player() {
	WM.markForDelete(p_reticle);
}

int Player::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::KEYBOARD_EVENT) {
		const df::EventKeyboard* p_keyboard_event =
			dynamic_cast <const df::EventKeyboard*> (p_e);
		kbd(p_keyboard_event);
		return 1;
	}
	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}
	if (p_e->getType() == POWER_UP_EVENT) {
		levelUp();
		return 1;
	}

	return 0;
}

void Player::kbd(const df::EventKeyboard* p_keyboard_event) {
	switch (p_keyboard_event->getKey()) {
	case df::Keyboard::W:
		move(0,-1);
		break;
	case df::Keyboard::A:
		move(-1, 0);
		break;
	case df::Keyboard::S:
		move(0, +1);
		break;
	case df::Keyboard::D:
		move(+1, 0);
		break;
	default:
		return;
	}
}

void Player::move(int dx, int dy) {
	if (move_countdown > 0)
		return;
	move_countdown = move_slowdown;

	// If stays on window, allow move.
	df::Vector new_pos(getPosition().getX() + dx, getPosition().getY() + dy);
	if ((new_pos.getY() > 3) &&
		(new_pos.getY() < WM.getBoundary().getVertical()) &&
		(new_pos.getX() > 3) &&
		(new_pos.getX() < WM.getBoundary().getHorizontal()))
		WM.moveObject(this, new_pos);

}

void Player::fire(df::Vector target) {
	//checks if fire is on time cooldown
	if (fire_countdown > 0)
		return;
	fire_countdown = fire_slowdown;

	// Fire Bullet towards target.
	// Compute normalized vector to position, then scale by speed (1).
	df::Vector v = target - getPosition();
	v.normalize();
	v.scale(1);
	Bullet* p = new Bullet(getPosition());
	p->setVelocity(v);

	// Play "fire" sound.
	df::Sound* p_sound = RM.getSound("fire");
	if (p_sound)
		p_sound->play();
}

void Player::step() {
	// Move countdown.
	move_countdown--;
	if (move_countdown < 0)
		move_countdown = 0;

	// Fire countdown.
	fire_countdown--;
	if (fire_countdown < 0)
		fire_countdown = 0;
	if (fire_countdown == 0)
	{
		LM.writeLog("hit fire countdown");
		fire(p_reticle->getPosition());
	}
}

void Player::addXP(int xp) {

	// Add xp
	current_xp += xp;

	// Check for level up and send event if needed
	if (current_xp == MAX_XP) {
		EventPowerUp powerUp;
		WM.onEvent(&powerUp);
	}
}

void Player::levelUp() {

	// Reset xp
	current_xp = 0;

	// Play "level up" sound
	df::Sound* p_sound = RM.getSound("level up");
	if (p_sound) {
		p_sound->play();
	}
}