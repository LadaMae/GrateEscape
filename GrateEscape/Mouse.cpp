// Engine includes
#include "WorldManager.h"
#include "DisplayManager.h"
#include "Vector.h"
#include "ObjectList.h"
#include "Object.h"
#include "EventStep.h"

// Game includes
#include "Mouse.h"
#include "Player.h"
#include <stdio.h>
#include <random>
#include <LogManager.h>

// Mouse constructor
Mouse::Mouse() {

	// Set attributes
	setSprite("mouse");
	setType("Mouse");
	setSpeed(0.06);

	// Register interest in events
	registerInterest(df::STEP_EVENT);

	m_health = 10;

	moveToStart();
}

// Destructor
Mouse::~Mouse(){

	// Add XP to the player
	df::addParticles(df::SPARKS, getPosition(), 2, df::WHITE);
	df::addParticles(df::SPARKS, getPosition(), 1, df::YELLOW);
	df::ObjectList player_list = WM.objectsOfType("Player");
	df::Object* p_obj = player_list[0];
	Player* p_player = dynamic_cast <Player*> (p_obj);
	p_player->addXP(5);
}

int Mouse::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::STEP_EVENT) {
		findNewVelocity(findPlayer());
		return 1;
	}

	if (p_e->getType() == df::COLLISION_EVENT) {
		const df::EventCollision* p_collision_event =
			dynamic_cast <const df::EventCollision*> (p_e);
		hit(p_collision_event);
		return 1;
	}

	return 0;
}

// Move Mouse to random starting point
void Mouse::moveToStart() {

	df::Vector temp_pos;

	float world_horiz = WM.getBoundary().getHorizontal();
	float world_vert = WM.getBoundary().getVertical();

	// x is anywhere between -82 and 162
	int lower_bound = -82;
	int upper_bound = 162;

	std::random_device rd;
	std::mt19937 gen(rd());

	// Define the distribution range
	std::uniform_int_distribution<> distr(lower_bound, upper_bound);

	// Set X and Y
	temp_pos.setX(distr(gen));
	temp_pos.setY(rand() % (int)(world_vert) + 4.0f);

	// Y should be greater than 24 and less than 0 if X is on the screen
	if (temp_pos.getX() >= 0 || temp_pos.getX() <= 80) {
		int up_down = rand() % 2;
		if (up_down == 0) {
			temp_pos.setY(0 - rand() % (int)(world_vert) - 5.0f);
		}
		else {
			temp_pos.setY(world_vert + rand() % (int)(world_vert) + 5.0f);
		}
	}

	// If collision, move right slightly until empty space
	df::ObjectList collision_list = WM.getCollisions(this, temp_pos);
	while (collision_list.getCount() != 0) {
		temp_pos.setX(temp_pos.getX() + 1);
		collision_list = WM.getCollisions(this, temp_pos);
	}

	WM.moveObject(this, temp_pos);
}

void Mouse::findNewVelocity(df::Vector target) {

	// Vector that will hold the new direction of the object
	df::Vector new_direction;

	// Find x and y distance between the Mouse and the target
	float delta_x = target.getX() - getPosition().getX();
	float delta_y = target.getY() - getPosition().getY();
	df::Vector distance(delta_x, delta_y);

	// Calculate the magnitude of the vector between the Mouse and the target
	float magnitude = distance.getMagnitude();

	// Normalize the vector to get the direction
	if (magnitude != 0) {
		new_direction.setX(delta_x / magnitude);
		new_direction.setY(delta_y / magnitude);
	}

	// Scale according to speed
	new_direction.scale(getSpeed());
	setVelocity(new_direction);
}

// Using the WM, find the player and return their position
df::Vector Mouse::findPlayer() {

	// Get player object
	df::ObjectList player_list = WM.objectsOfType("Player");
	df::Object* p_player = player_list[0];

	return p_player->getPosition();
}

// Get rid of this once sprite is ready
/*int Mouse::draw() {
	return DM.drawCh(getPosition(), '*', df::Color::WHITE);
}*/

//when the mouse hits the player, decreases health
void Mouse::hit(const df::EventCollision* p_collision_event) {
	if ((p_collision_event->getObject1()->getType() == "Player") ||
		(p_collision_event->getObject2()->getType() == "Player"))
	{
		WM.markForDelete(p_collision_event->getObject1());
		WM.markForDelete(p_collision_event->getObject2());
		//code to decrement player health
		//ERROR: for some reason erroring when player is deleted?
		/*df::ObjectList player_list = WM.objectsOfType("Player");
		df::Object* p_obj = player_list[0];
		Player* p_player = dynamic_cast <Player*> (p_obj);

		p_player->setHealth(p_player->getHealth() - 5);
		if (p_player->getHealth() <= 0)
		{
			WM.markForDelete(p_player);
			//call game over here
		}*/
	}
}

//for health mechanic
// BACKLOGGED
int Mouse::getHealth() {
	return m_health;
}

void Mouse::setHealth(int new_health) {
	m_health = new_health;
}