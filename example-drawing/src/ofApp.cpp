#include "ofApp.h"

void ofApp::setup(){
	// 10 fps is important, otherwise we can overload the device and it will stop
	// updating. it can handle 400 messages per second, and a full grid update
	// is 8x8/2 + 1 = 33 messages. 400 / 33 = 12 fps absolute max.
	ofSetFrameRate(10);
	ofSetVerticalSync(true);
	launchpad.setup(0);
}

void ofApp::launchpadDraw() {
	ofClear(0, 255);
	float r = ofMap(sin(ofGetElapsedTimef() * 4), -1, 1, 0, 4);
	ofFill();
	ofSetColor(ofColor::green);
	ofDrawCircle(4, 4, r);
	ofNoFill();
	ofSetColor(ofColor::red);
	ofDrawCircle(4, 4, r);
}

void ofApp::update(){
	launchpad.begin();
	launchpadDraw();
	launchpad.end();
}

void ofApp::draw(){
	launchpad.draw(0, 0);
}
