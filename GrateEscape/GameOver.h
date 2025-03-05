#pragma once
#pragma once
#include "ViewObject.h"

//establish game over object
class GameOver : public df::ViewObject {
private:
	int time_to_live;
	void step();

public:
	GameOver();
	~GameOver();
	int eventHandler(const df::Event* p_e) override;
	int draw() override;
};
