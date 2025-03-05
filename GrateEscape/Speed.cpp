#include "Speed.h"
#include "Event.h"
#include "EventStep.h"
#include "LogManager.h"

Speed::Speed() {
	//sets location, string, and color of ui
	setLocation(df::TOP_RIGHT);
	setViewString(SPEED_STRING);
	setColor(df::YELLOW);

	setValue(1);
}
