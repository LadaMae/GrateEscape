#include "EventMouse.h"
#include "DisplayManager.h"
#include "WorldManager.h"
#include "Reticle.h"

Reticle::Reticle() {
    //set type
    setType("Reticle");

    //set solidness
    setSolidness(df::SPECTRAL);

    //set z layer (above every other object)
    setAltitude(df::MAX_ALTITUDE); // Make Reticle in foreground.
    //register mouse event
    registerInterest(df::MSE_EVENT);

    //set position
    df::Vector p(WM.getBoundary().getHorizontal() / 2,
        WM.getBoundary().getVertical() / 2);
    setPosition(p);

}

int Reticle::eventHandler(const df::Event* p_e) {

    //when mouse is moved
    if (p_e->getType() == df::MSE_EVENT) {
        const df::EventMouse* p_mouse_event =
            dynamic_cast <const df::EventMouse*> (p_e);
        if (p_mouse_event->getMouseAction() == df::MOVED) {
            // Change location to new mouse position.
            setPosition(p_mouse_event->getMousePosition());
            return 1;
        }
    }

    // If get here, have ignored this event.
    return 0;
}

//draws reticle
int Reticle::draw() {
    return DM.drawCh(getPosition(), RETICLE_CHAR, df::RED);
}