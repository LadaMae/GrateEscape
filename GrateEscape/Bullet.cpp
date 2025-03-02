#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "EventOut.h"

#include "Bullet.h"
#include "Mouse.h"

Bullet::Bullet(df::Vector hero_pos) {
	//set bullet type
	setSprite("bullet");
	//set bullet sprite
	setType("Bullet");
	//set bullet position
	df::Vector p(hero_pos.getX() + 3, hero_pos.getY());
	setPosition(p);
	//set bullet speed
	setSpeed(1);
	//set bullet solidness
	setSolidness(df::SOFT);
}

int Bullet::eventHandler(const df::Event* p_e) {
	//check if bullet is offscreen
	if (p_e->getType() == df::OUT_EVENT) {
		out();
		return 1;
	}

	//check if bullet collided with a saucer
	if (p_e->getType() == df::COLLISION_EVENT) {
		const df::EventCollision* p_collision_event =
			dynamic_cast <const df::EventCollision*> (p_e);
		hit(p_collision_event);
		return 1;
	}
	return 0;
}

//if bullet is offscreen, delete the bullet
void Bullet::out() {
	WM.markForDelete(this);
}

void Bullet::hit(const df::EventCollision* p_collision_event) {
	//if the bullet hits a saucer, delete the bullet and the saucer
	if ((p_collision_event->getObject1()->getType() == "Mouse") ||
		(p_collision_event->getObject2()->getType() == "Mouse")) {
		WM.markForDelete(p_collision_event->getObject1());
		WM.markForDelete(p_collision_event->getObject2());
	}
}
