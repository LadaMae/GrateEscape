#include "PowerUps.h"
#include "Player.h"
#include <string>


PowerUps::PowerUps(float increment, std::string type) {
	pu_increment = increment;
	pu_type = type;
}

float PowerUps::getIncrement() const {
	return pu_increment;
}

void PowerUps::setIncrement(float new_increment) {
	pu_increment = new_increment;
}

std::string PowerUps::getType() const {
	return pu_type;
}

void PowerUps::setType(std::string new_type) {
	pu_type = new_type;
}