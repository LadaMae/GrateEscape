#pragma once
#include "Object.h"

//establish reticle object
#define RETICLE_CHAR '+'

class Reticle : public df::Object {

public:
	Reticle();
	int draw(void) override;
	int eventHandler(const df::Event* p_e) override;
};