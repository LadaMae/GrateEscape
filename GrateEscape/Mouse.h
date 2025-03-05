// Engine includes
#include "Object.h"
#include "EventCollision.h"
#include "Vector.h"

class Mouse : public df::Object {
private:
	int m_health;

public:
	Mouse();
	~Mouse();
	int eventHandler(const df::Event* p_e) override;
	void moveToStart();
	int draw();

	// Calculate velocity based on the current position and target position
	void findNewVelocity(df::Vector target);

	// Using the WM, find the player and return their position
	df::Vector findPlayer();

	void hit(const df::EventCollision* p_collision_event);

	int getHealth();
	void setHealth(int new_health);
};