#pragma once
#include "Object.h"
#include "EventKeyboard.h"

class Player : public df::Object {
private:
	void kbd(const df::EventKeyboard* p_keyboard_event);
	void move(int dx, int dy);
	void step();
	int move_slowdown = 2;
	int move_countdown = move_slowdown;

public:
	Player();
	~Player();
	int eventHandler(const df::Event* p_e) override;

};