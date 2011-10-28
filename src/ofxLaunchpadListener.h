#pragma once

#include "ofMain.h"

class ButtonEvent : public ofEventArgs {
public:
	int col, row;
	bool pressed;
	ButtonEvent(int col = 0, int row = 0, bool pressed = false)
	:col(col), row(row), pressed(pressed) {
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
	virtual void launchpadEvent(int col, int row, int pressed) {}
	
	// these methods handle the events for you
	virtual void automapButton(ButtonEvent& buttonEvent) {
		if(buttonEvent.pressed) {
			automapButtonPressed(buttonEvent.col);
		} else {
			automapButtonReleased(buttonEvent.col);
		}
		launchpadEvent(buttonEvent.col, buttonEvent.row, buttonEvent.pressed);
	}
	virtual void gridButton(ButtonEvent& buttonEvent) {
		if(buttonEvent.pressed) {
			gridButtonPressed(buttonEvent.col, buttonEvent.row);
		} else {
			gridButtonReleased(buttonEvent.col, buttonEvent.row);
		}
		launchpadEvent(buttonEvent.col, buttonEvent.row, buttonEvent.pressed);
	}
};