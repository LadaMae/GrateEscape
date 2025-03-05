#include "AtkSpeed.h"
#include "Event.h"
#include "EventStep.h"
#include "LogManager.h"

AtkSpeed::AtkSpeed() {
	//sets location, string, and color of ui
	setLocation(df::TOP_LEFT);
	setViewString(ATKSPEED_STRING);
	setColor(df::YELLOW);

	setValue(1);
}
