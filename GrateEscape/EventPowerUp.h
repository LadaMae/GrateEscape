// Engine includes
#include "Event.h"

// Game includes
#include <string>

const std::string POWER_UP_EVENT = "power up";

class EventPowerUp : public df::Event {

public:
	EventPowerUp();
};