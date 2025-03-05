#pragma once
#include <string>

class PowerUps {
private: 
	float pu_increment = 0;
	std::string pu_type = "";


public:
	PowerUps(float increment, std::string type);
	float getIncrement() const;
	void setIncrement(float new_increment);
	std::string getType() const;
	void setType(std::string new_type);

};
