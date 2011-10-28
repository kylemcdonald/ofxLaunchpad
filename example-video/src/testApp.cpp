#include "testApp.h"

void testApp::setup(){
	ofSetFrameRate(10);
	ofSetVerticalSync(true);
	ofBackground(255);
	launchpad.setup(1);
	camera.initGrabber(640, 480);
}

void testApp::update(){
/*
	int row = ofMap(mouseY, 0, ofGetHeight(), 0, 7, false);
	int col = ofMap(mouseX, 0, ofGetWidth(), 0, 8, true);
	if(ofGetMousePressed()) {
		launchpad.setLed(row, col, 3, 0);
	} else {
		launchpad.setLed(row, col, 0, 3);
	}
	*/
	int i = ofGetFrameNum();
	int red = i % 4;
	int green = 3 - red;
	//launchpad.setAll(red, green);
	//launchpad.setLed(0, 0, 0, 0);
	
	camera.update();
	if(camera.isFrameNew()) {
		pix = camera.getPixelsRef();
		pix.crop(80, 0, 480, 480);
		pix.resize(8, 8);
		pix.update();
		launchpad.set(pix.getPixelsRef());
	}
}

void testApp::draw(){
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
	ofScale(480 / 8, 480 / 8);
	pix.draw(0, 0);
}

void testApp::keyPressed(int key){
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
