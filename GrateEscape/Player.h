#pragma once
#include "Object.h"
#include "EventKeyboard.h"
#include "Reticle.h"

class Player : public df::Object {
private:
	Reticle* p_reticle;
	void kbd(const df::EventKeyboard* p_keyboard_event);
	void move(int dx, int dy);
	void step();
	void fire(df::Vector target);
	int move_slowdown = 2;
	int move_countdown = move_slowdown;
	int fire_slowdown = 15;
	int fire_countdown = fire_slowdown;

public:
	Player();
	~Player();
	int eventHandler(const df::Event* p_e) override;

};