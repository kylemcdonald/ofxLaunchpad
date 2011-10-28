#pragma once

#include "ofxLaunchpad.h"

class ofxLaunchpadToggle : public ofxLaunchpad, public ofxLaunchpadListener {
public:
	enum ToggleMode {
		MOMENTARY_MODE,
		TOGGLE_MODE,
		CYCLE_MODE,
		HOLD_MODE};
	ofxLaunchpadToggle();
	void setup(int port, ofxLaunchpadListener* listener = NULL);
	void launchpadEvent(ButtonEvent buttonEvent);
private:
	ToggleMode toggleMode;
};