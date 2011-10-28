#pragma once

#include "ofxLaunchpad.h"

class ofxLaunchpadToggle : public ofxLaunchpad, public ofxLaunchpadListener {
public:
	enum ToggleMode {
		MOMENTARY_MODE,
		TOGGLE_MODE,
		CYCLE_MODE};
	ofxLaunchpadToggle();
	void setup(int port, ofxLaunchpadListener* listener = NULL);
	void launchpadEvent(int col, int row, int pressed);
private:
	ToggleMode toggleMode;
};