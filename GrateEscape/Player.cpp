#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "DisplayManager.h"
#include "GameManager.h"
#include "EventStep.h"
#include "EventView.h"
#include "EventPowerUp.h"
#include "GameOver.h"

#include "Player.h"
#include "Bullet.h"
#include "Speed.h"
#include "AtkSpeed.h"
#include "Health.h"

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

	//variables for player power ups
	//NOTE: health and atk are stretch goals
	p_health = 20;
	p_atk = 5;
	p_speed = 0.0;
	p_atkSpeed = 1.0;
	atkSpeedIncrement = 1;

	move_slowdown = 2;
	move_countdown = move_slowdown;
	fire_slowdown = 30;
	fire_slowdown = fire_slowdown / p_atkSpeed;
	fire_countdown = fire_slowdown;

	current_xp = 0;
}

Player::~Player() {
	//causing issue on game over
	new GameOver;
	WM.markForDelete(p_reticle);
	df::addParticles(df::SPARKS, getPosition(), 4, df::RED);
	DM.shake(20, 20, 10);
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
		LM.writeLog("in powerup event");
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
	//adjusts speed here
	// NOTE: adjusts in 0.1 increments
	df::Vector new_pos(getPosition().getX() + ((float)dx + (dx * p_speed)), getPosition().getY() + ((float)dy + (dy * p_speed)));
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

	LM.writeLog("Leveled up event");

	// Reset xp
	current_xp = 0;

	// Play "level up" sound
	df::Sound* p_sound = RM.getSound("level up");
	if (p_sound) {
		p_sound->play();
	}

	//give player powerup
	PowerUps chosenPU = getRandomPU();
	//apply PU
	applyPU(chosenPU);
}


int Player::getHealth() const {
	return p_health;
}

void Player::setHealth(int new_health) {
	df::addParticles(df::FIREWORKS, getPosition(), 2, df::GREEN);
	df::addParticles(df::SPARKS, getPosition(), 1, df::RED);
	p_health = new_health;
	df::EventView ev("Health", p_health, false);
	WM.onEvent(&ev);
}

int Player::getAtk() const {
	return p_atk;
}

void Player::setAtk(int new_atk) {
	p_atk = new_atk;
}

float Player::getSpeed() const {
	return p_speed;
}

void Player::setSpeed(float new_speed) {
	df::addParticles(df::FIREWORKS, getPosition(), 2, df::WHITE);
	df::addParticles(df::SPARKS, getPosition(), 1, df::RED);
	p_speed = new_speed;
	int speedIncrement = 1 + (p_speed * 10);
	df::EventView ev("Speed", speedIncrement, false);
	WM.onEvent(&ev);

}

float Player::getAtkSpeed() const {
	return p_atkSpeed;
}

void Player::setAtkSpeed(float new_atkSpeed) {
	//adjusts atk speed here
	//	NOTE: adjusts in 0.1 increments
	df::addParticles(df::FIREWORKS, getPosition(), 2, df::YELLOW);
	df::addParticles(df::SPARKS, getPosition(), 1, df::RED);
	p_atkSpeed = new_atkSpeed;
	fire_slowdown = fire_slowdown / p_atkSpeed;
	atkSpeedIncrement++;
	//UPDATES ATKSPEED HERE
	df::EventView ev("AtkSpeed", atkSpeedIncrement, false);
	WM.onEvent(&ev);
}

PowerUps Player::getRandomPU() {
	LM.writeLog("getting random power up");
	int random_index = std::rand() % allPUs.size();

	// Access the random element using the random index
	PowerUps random_element = allPUs[random_index];
	return random_element;
}

void Player::applyPU(PowerUps powerUp) {
	float increment = powerUp.getIncrement();
	std::string type = powerUp.getType();
	LM.writeLog("Applying new PU: %s, %f", type, increment);

	if (type == "atk")
	{
		LM.writeLog("got atk PU");
		setAtk(p_atk + increment);
	}
	else if (type == "health")
	{
		LM.writeLog("got health PU");
		setHealth(p_health + increment);
	}
	else if (type == "speed")
	{
		LM.writeLog("got speed PU");
		setSpeed(p_speed + increment);
	}
	else if (type == "atkSpeed")
	{
		LM.writeLog("got atkSpeed PU");
		setAtkSpeed(p_atkSpeed + increment);
	}

}

