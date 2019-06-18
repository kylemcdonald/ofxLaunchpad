#pragma once

#include "ofMain.h"

class ButtonEvent : public ofEventArgs {
public:
	int col, row;
	bool pressed;
	float timestamp, duration;
	ButtonEvent(int col = 0, int row = 0, bool pressed = false, ButtonEvent* previous = NULL)
	:col(col), row(row), pressed(pressed) {
		timestamp = ofGetElapsedTimef();
		if(previous != NULL) {
			duration = timestamp - previous->timestamp;
		} else {
			duration = 0;
		}
	}
};

class ofxLaunchpadListener {
public:
	// override these methods to get events from the launchpad.
	virtual void automapButtonPressed(int col) {}
	virtual void automapButtonReleased(int col) {}
	virtual void gridButtonPressed(int col, int row) {}
	virtual void gridButtonReleased(int col, int row) {}

	// this is a general version of the four methods above. automap is row 8.
	virtual void launchpadEvent(ButtonEvent event) {}

	// these methods handle the events for you
	virtual void automapButton(ButtonEvent& buttonEvent) {
		if(buttonEvent.pressed) {
			automapButtonPressed(buttonEvent.col);
		} else {
			automapButtonReleased(buttonEvent.col);
		}
		launchpadEvent(buttonEvent);
	}
	virtual void gridButton(ButtonEvent& buttonEvent) {
		if(buttonEvent.pressed) {
			gridButtonPressed(buttonEvent.col, buttonEvent.row);
		} else {
			gridButtonReleased(buttonEvent.col, buttonEvent.row);
		}
		launchpadEvent(buttonEvent);
	}
};
