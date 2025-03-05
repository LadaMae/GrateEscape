#include "GameOver.h"
#include "GameStart.h"
#include "ResourceManager.h"
#include "EventStep.h"
#include "WorldManager.h"
#include "GameManager.h"

GameOver::GameOver() {
	//set type and sprite
	setType("GameOver");
	setSprite("gameover");

	// Link to "message" sprite.
	if (setSprite("gameover") == 0)
		time_to_live = getAnimation().getSprite()->getFrameCount() * getAnimation().getSprite()->getSlowdown();
	else
		time_to_live = 0;

	//put in center of window.
	setLocation(df::CENTER_CENTER);

	//register  step event.
	registerInterest(df::STEP_EVENT);

}

//step event
//steps through game start animation
int GameOver::eventHandler(const df::Event* p_e) {

	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}

	return 0;
}

//count down frames in game over
void GameOver::step() {
	time_to_live--;
	if (time_to_live <= 0)
		WM.markForDelete(this);
}

// When object exits, indicate game over.
GameOver::~GameOver() {
	// Remove Saucers and ViewObjects, re-activate GameStart.
	df::ObjectList object_list = WM.getAllObjects(true);
	for (int i = 0; i < object_list.getCount(); i++) {
		df::Object* p_o = object_list[i];
		if (p_o->getType() == "Mouse" || p_o->getType() == "ViewObject")
			WM.markForDelete(p_o);
		if (p_o->getType() == "GameStart") {
			p_o->setActive(true);
			//resume music
			dynamic_cast <GameStart*> (p_o)->playMusic();
		}
	}
}

// Override default draw so as not to display "value".
int GameOver::draw() {
	return df::Object::draw();
}