#include "testApp.h"

void testApp::setup(){
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	ofBackground(255);
	
	launchpad.setup(1, this);
	
	camera.initGrabber(640, 480);
}

void testApp::update(){

	int col = ofMap(mouseX, 0, ofGetWidth(), 0, 8);
	int row = ofMap(mouseY, 0, ofGetHeight(), 0, 8);
	if(ofGetMousePressed()) {
		if(row == 8) {
			launchpad.setLedAutomap(col, 3, 0);
		} else {
			launchpad.setLedGrid(col, row, 3, 0);
		}
	} else {
		if(row == 8) {
			launchpad.setLedAutomap(col, 0, 3);
		} else {
			launchpad.setLedGrid(col, row, 0, 3);
		}
	}
	
	
	//launchpad.setLed(0, 0, 0, 0);
	
	camera.update();
	if(camera.isFrameNew()) {
		pix = camera.getPixelsRef();
		pix.crop(80, 0, 480, 480);
		pix.resize(8, 8);
		pix.update();
		//launchpad.set(pix.getPixelsRef());
	}
}

void testApp::draw(){
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
	ofScale(480 / 8, 480 / 8);
	pix.draw(0, 0);
}