#include "ofxLaunchpadToggle.h"

ofxLaunchpadToggle::ofxLaunchpadToggle()
:toggleMode(CYCLE_MODE) {
}

void ofxLaunchpadToggle::setup(int port, ofxLaunchpadListener* listener) {
	ofxLaunchpad::setup(port, listener);
	addListener(this);
}

void ofxLaunchpadToggle::launchpadEvent(int col, int row, int pressed) {
	ofxLaunchpadColor cur = getLedGrid(col, row);
	switch(toggleMode) {
		case MOMENTARY_MODE:
			setLedGrid(col, row, ofxLaunchpadColor(!cur.isOn()));
			break;
		case TOGGLE_MODE:
			if(pressed) {
				setLedGrid(col, row, ofxLaunchpadColor(!cur.isOn()));
			}
			break;
		case CYCLE_MODE:
			if(pressed) {
				if(cur.isRed()) {
					setLedGrid(col, row, ofxLaunchpadColor(0, 3));
				} else if(cur.isGreen()) {
					setLedGrid(col, row, ofxLaunchpadColor(0, 0));
				} else {
					setLedGrid(col, row, ofxLaunchpadColor(3, 0));
				}
			}
			break;
	}
}