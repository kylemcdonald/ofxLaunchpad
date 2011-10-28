#include "ofxLaunchpadToggle.h"

ofxLaunchpadToggle::ofxLaunchpadToggle()
:toggleMode(HOLD_MODE) {
}

void ofxLaunchpadToggle::setup(int port, ofxLaunchpadListener* listener) {
	ofxLaunchpad::setup(port, listener);
	addListener(this);
}

void ofxLaunchpadToggle::launchpadEvent(ButtonEvent buttonEvent) {
	int row = buttonEvent.row;
	int col = buttonEvent.col;
	bool pressed = buttonEvent.pressed;
	float duration = buttonEvent.duration;
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
					setLedGrid(col, row, ofColor::green);
				} else if(cur.isGreen()) {
					setLedGrid(col, row, ofColor::black);
				} else {
					setLedGrid(col, row, ofColor::red);
				}
			}
			break;
		case HOLD_MODE:
			if(!pressed) {
				if(duration < .15) {
					setLedGrid(col, row, ofColor::black);
				} else if(duration < .75) {
					setLedGrid(col, row, ofColor::green);
				} else {
					setLedGrid(col, row, ofColor::red);
				}
			}
			break;
	}
}