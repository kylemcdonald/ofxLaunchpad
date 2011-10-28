#include "testApp.h"

void testApp::setup(){
	ofSetFrameRate(30);
	ofSetVerticalSync(true);
	ofBackground(255);
	launchpad.setup(1);
	camera.initGrabber(640, 480);
}

void testApp::update(){

	int row = ofMap(mouseY, 0, ofGetHeight(), 0, 8);
	int col = ofMap(mouseX, 0, ofGetWidth(), 0, 8);
	if(ofGetMousePressed()) {
		if(row == 8) {
			launchpad.setLedAutomap(col, 3, 0);
		} else {
			launchpad.setLedGrid(row, col, 3, 0);
		}
	} else {
		if(row == 8) {
			launchpad.setLedAutomap(col, 0, 3);
		} else {
			launchpad.setLedGrid(row, col, 0, 3);
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

void testApp::keyPressed(int key){
	if(key == 'a') {
		launchpad.setAll(ofxLaunchpad::LOW_BRIGHTNESS_MODE);
	} else if(key == 's') {
		launchpad.setAll(ofxLaunchpad::MEDIUM_BRIGHTNESS_MODE);
	} else if(key == 'd') {
		launchpad.setAll(ofxLaunchpad::FULL_BRIGHTNESS_MODE);
	} else if(key == 'f') {
		launchpad.setAll(ofxLaunchpad::OFF_BRIGHTNESS_MODE);
	}
}

void testApp::keyReleased(int key){
}

void testApp::mouseMoved(int x, int y ){
}

void testApp::mouseDragged(int x, int y, int button){
}

void testApp::mousePressed(int x, int y, int button){	
}

void testApp::mouseReleased(){
}
