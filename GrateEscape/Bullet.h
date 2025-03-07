#pragma once
#include "Object.h"
#include "EventCollision.h"

//establish bullet object
class Bullet : public df::Object {

private:
	void out();
	void hit(const df::EventCollision* p_collision_event);

public:
	Bullet(df::Vector hero_pos);
	int eventHandler(const df::Event* p_e) override;
};