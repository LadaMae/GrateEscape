#include "Health.h"
#include "Event.h"
#include "EventStep.h"
#include "LogManager.h"

Health::Health() {
	//sets location, string, and color of ui
	setLocation(df::TOP_CENTER);
	setViewString(HEALTH_STRING);
	setColor(df::GREEN);

	setValue(20);
}
