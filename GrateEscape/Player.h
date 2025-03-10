#pragma once
#include "Object.h"
#include "EventKeyboard.h"
#include "Reticle.h"
#include "PowerUps.h"
#include <array>

#define MAX_XP 10

const PowerUps healthPowerUp = PowerUps(1, "health");
const PowerUps atkPowerUp = PowerUps(1, "atk");
const PowerUps speedPowerUp = PowerUps(0.1, "speed");
const PowerUps atkSpeedPowerUp = PowerUps(0.1, "atkSpeed");

const std::array<PowerUps, 3> allPUs = {
	healthPowerUp,
	//atkPowerUp,
	speedPowerUp,
	atkSpeedPowerUp
};

class Player : public df::Object {
private:
	Reticle* p_reticle;
	void kbd(const df::EventKeyboard* p_keyboard_event);
	void move(int dx, int dy);
	void step();
	void fire(df::Vector target);
	void levelUp();
	int move_slowdown = 2;
	int move_countdown = move_slowdown;
	int fire_slowdown = 15;
	int fire_countdown = fire_slowdown;
	int p_health = 20;
	int p_atk = 5;
	float p_speed = 0;
	float p_atkSpeed = 0;
	int atkSpeedIncrement = 1;


public:
	int current_xp;
	Player();
	~Player();
	int getHealth() const;
	void setHealth(int new_health);
	int getAtk() const;
	void setAtk(int new_atk);
	float getSpeed() const;
	void setSpeed(float new_speed);
	float getAtkSpeed() const;
	void setAtkSpeed(float new_atkSpeed);
	int eventHandler(const df::Event* p_e) override;
	void addXP(int xp);
	PowerUps getRandomPU();
	void applyPU(PowerUps powerUp);
};