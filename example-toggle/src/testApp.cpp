#include "testApp.h"

void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(255);
	launchpad.setup(1);
}

void testApp::update(){
}

void testApp::draw(){
	launchpad.draw(0, 0, 480);
}
