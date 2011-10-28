#include "testApp.h"

void testApp::setup(){
	// 10 fps is important, otherwise we can overload the device and it will stop
	// updating. it can handle 400 messages per second, and a full grid update
	// is 8x8/2 + 1 = 33 messages. 400 / 33 = 12 fps absolute max.
	ofSetFrameRate(10);
	ofSetVerticalSync(true);
	ofBackground(255);
	launchpad.setup(1);
	camera.initGrabber(640, 480);
	useColor = false;
}

void testApp::update(){
	camera.update();
	if(camera.isFrameNew()) {
		pix = camera.getPixelsRef();
		pix.crop(80, 0, 480, 480);
		pix.resize(8, 8);
		pix.update();
		if(!useColor) {
			pix.setImageType(OF_IMAGE_GRAYSCALE);
			pix.setImageType(OF_IMAGE_COLOR);
		}
		launchpad.set(pix.getPixelsRef());
	}
}

void testApp::draw(){
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
	ofScale(480 / 8, 480 / 8);
	pix.draw(0, 0);
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		useColor = !useColor;
	}
}