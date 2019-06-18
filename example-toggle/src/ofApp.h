#pragma once

#include "ofMain.h"
#include "ofxLaunchpadToggle.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void dragChange(int x, int y, int button, int reset);

	ofxLaunchpadToggle launchpad;
	ofColor curColor;
};
