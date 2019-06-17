#pragma once

#include "ofMain.h"
#include "ofxLaunchpad.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);

	ofxLaunchpad launchpad;
	ofVideoGrabber camera;
	ofImage pix;
	bool useColor;
};
