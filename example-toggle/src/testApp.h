#pragma once

#include "ofMain.h"
#include "ofxLaunchpadToggle.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofxLaunchpadToggle launchpad;
};
