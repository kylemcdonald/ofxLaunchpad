#include "ofxLaunchpadToggle.h"

ofxLaunchpadToggle::ofxLaunchpadToggle()
:toggleMode(YELLOW_MODE) {
}

void ofxLaunchpadToggle::setup(int port, ofxLaunchpadListener* listener) {
	ofxLaunchpad::setup(port, listener);
	addListener(this);
}

void ofxLaunchpadToggle::launchpadEvent(int col, int row, int pressed) {
	setLedGrid(col, row, ofxLaunchpadColor(3, 0));
}