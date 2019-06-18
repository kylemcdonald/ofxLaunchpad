#include "ofxLaunchpad.h"

// if you click off the window, there will be too many messages
// and the midi will fill up and have trouble
// need some kind of rate limiting buffer?

const int numeratorMin = 1, numeratorMax = 16;
const int denominatorMin = 3, denominatorMax = 18;
const int bufferMask = 1;
const int automapBegin = 104;
const int rowMask = 7;
const int colMask = 15;
const int cols = 9;
const int automapRow = 8;
const int totalButtons = 80;

void ofxLaunchpad::setup(int port, ofxLaunchpadListener* listener) {
	midiOut.listOutPorts();
	midiOut.openPort(port);

	setMappingMode();
	setAll();

	midiIn.listInPorts();
	midiIn.openPort(port);
	midiIn.addListener(this);

	if(listener != NULL) {
		addListener(listener);
	}

	fbo.allocate(256, 256);
}

ofColor boostBrightness(ofColor color) {
	return color / 2 + ofColor(128);
}

void ofxLaunchpad::draw(float x, float y, float width, float height) const {
	ofPushStyle();
	ofPushMatrix();
	ofSetCircleResolution(12);
	ofSetLineWidth(MIN(width, height) / (cols * 10));
	ofTranslate(x, y);
	ofScale(width / cols, height / cols);

	ofColor outlineColor(64);

	ofFill();
	ofSetColor(0);
	ofDrawRectangle(0, 0, 9, 9);
	ofNoFill();
	ofSetColor(outlineColor);
	ofDrawRectangle(0, 0, 9, 9);

	ofPushMatrix();
	ofTranslate(.5, .5);
	for(int col = 0; col < 8; col++) {
		ofFill();
		ofSetColor(boostBrightness(getLedGrid(col, automapRow)));
		ofDrawCircle(col, 0, .3);
		ofNoFill();
		ofSetColor(outlineColor);
		ofDrawCircle(col, 0, .3);
	}
	ofPopMatrix();

	ofTranslate(0, 1);
	for(int row = 0; row < 8; row++) {
		for(int col = 0; col < 8; col++) {
			ofPushMatrix();
			ofTranslate(col, row);
			ofFill();
			ofSetColor(boostBrightness(getLedGrid(col, row)));
			ofDrawRectangle(.1, .1, .8, .8);
			ofNoFill();
			ofSetColor(outlineColor);
			ofDrawRectangle(.1, .1, .8, .8);
			ofPopMatrix();
		}
	}

	ofPushMatrix();
	ofTranslate(4, 4);
	ofRotateDeg(45);
	ofFill();
	ofSetColor(0);
	ofDrawRectangle(-.25, -.25, .5, .5);
	ofPopMatrix();

	ofTranslate(8, 0);
	ofTranslate(.5, .5);
	for(int row = 0; row < 8; row++) {
		ofFill();
		ofSetColor(boostBrightness(getLedGrid(8, row)));
		ofDrawCircle(0, row, .3);
		ofNoFill();
		ofSetColor(outlineColor);
		ofDrawCircle(0, row, .3);
	}

	ofPopMatrix();
	ofPopStyle();
}

void ofxLaunchpad::draw(float x, float y) const {
	draw(x, y, getWidth(), getHeight());
}

float ofxLaunchpad::getWidth() const {
	return 32 * cols;
}

float ofxLaunchpad::getHeight() const {
	return 32 * cols;
}

void ofxLaunchpad::begin() {
	fbo.begin();
	ofPushStyle();
	ofPushMatrix();
}

void ofxLaunchpad::end() {
	ofPopMatrix();
	ofPopStyle();
	fbo.end();
	fbo.readToPixels(pix);
	pix.crop(0, 0, 8, 8);
	set(pix);
}

void ofxLaunchpad::addListener(ofxLaunchpadListener* listener) {
	ofAddListener(automapButtonEvent, listener, &ofxLaunchpadListener::automapButton);
	ofAddListener(gridButtonEvent, listener, &ofxLaunchpadListener::gridButton);
}

void ofxLaunchpad::removeListener(ofxLaunchpadListener* listener) {
	ofRemoveListener(automapButtonEvent, listener, &ofxLaunchpadListener::automapButton);
	ofRemoveListener(gridButtonEvent, listener, &ofxLaunchpadListener::gridButton);
}

void ofxLaunchpad::setBrightness(float brightness) {
	float dutyCycle = brightness / 2;
	float bestDistance;
	int bestDenominator, bestNumerator;
	for(int denominator = denominatorMin; denominator <= denominatorMax; denominator++) {
		for(int numerator = numeratorMin; numerator <= denominator; numerator++) {
			float curDutyCycle = (float) numerator / (float) denominator;
			float curDistance = abs(dutyCycle - curDutyCycle);
			if((curDistance < bestDistance) || (denominator == denominatorMin && numerator == numeratorMin)) {
				bestDenominator = denominator;
				bestNumerator = numerator;
				bestDistance = curDistance;
			}
		}
	}
	setDutyCycle(bestNumerator, bestDenominator);
}

void ofxLaunchpad::setMappingMode(MappingMode mappingMode) {
	midiOut.sendControlChange(1, 0, mappingMode == XY_MAPPING_MODE ? 1 : 2);
}

void ofxLaunchpad::setLedAutomap(int col, ofxLaunchpadColor color) {
	int key = automapBegin + col;
	int i = automapRow * cols + col;
	if(buffer[i] != color) {
		midiOut.sendControlChange(1, key, color.getMidi());
		buffer[i] = color;
	}
}

void ofxLaunchpad::setLedGrid(int col, int row, ofxLaunchpadColor color) {
	if(row == automapRow) {
		setLedAutomap(col, color);
	} else {
		int key = ((row & rowMask) << 4) | ((col & colMask) << 0);
		int i = row * cols + col;
		if(buffer[i] != color) {
			midiOut.sendNoteOn(1, key, color.getMidi());
			buffer[i] = color;
		}
	}
}

ofxLaunchpadColor ofxLaunchpad::getLedGrid(int col, int row) const {
	return buffer[row * cols + col];
}

void ofxLaunchpad::set(ofPixels& pix, bool clear, bool copy) {
	int i = 0;
	for(int y = 0; y < 8; y++) {
		for(int x = 0; x < 8; x += 2) {
			ofxLaunchpadColor first(pix.getColor(x, y));
			ofxLaunchpadColor second(pix.getColor(x + 1, y));
			midiOut.sendNoteOn(3, first.getMidi(), second.getMidi());
			buffer[i++] = first;
			buffer[i++] = second;
		}
		i++;
	}
	 // any note on signifies that we're done with rapid update
	midiOut.sendNoteOn(1, 127, 0);
}

void ofxLaunchpad::setBufferingMode(bool copy, bool flash, int update, int display) {
	int bufferingMode =
		(0 << 6) |
		(1 << 5) |
		((copy ? 1 : 0) << 4) |
		((flash ? 1 : 0) << 3) |
		((update & bufferMask) << 2) |
		(0 << 1) |
		((display & bufferMask) << 0);
	midiOut.sendControlChange(1, 0, bufferingMode);
}

void ofxLaunchpad::setAll(ofxLaunchpadColor::BrightnessMode brightnessMode) {
	int mode;
	switch(brightnessMode) {
		case ofxLaunchpadColor::OFF_BRIGHTNESS_MODE: mode = 0; break;
		case ofxLaunchpadColor::LOW_BRIGHTNESS_MODE: mode = 125; break;
		case ofxLaunchpadColor::MEDIUM_BRIGHTNESS_MODE: mode = 126; break;
		case ofxLaunchpadColor::FULL_BRIGHTNESS_MODE: mode = 127; break;
	}

	buffer.clear();
	buffer.resize(totalButtons, ofxLaunchpadColor(mode));

	lastEvent.clear();
	lastEvent.resize(totalButtons);

	midiOut.sendControlChange(1, 0, mode);
}

void ofxLaunchpad::setDutyCycle(int numerator, int denominator) {
	numerator = ofClamp(numerator, numeratorMin, numeratorMax);
	denominator = ofClamp(denominator, denominatorMin, denominatorMax);
	int data = (denominator - 3) << 0;
	if(numerator < 9) {
		data |= (numerator - 1) << 4;
		midiOut.sendControlChange(1, 30, data);
	} else {
		data |= (numerator - 9) << 4;
		midiOut.sendControlChange(1, 31, data);
	}
}

void ofxLaunchpad::newMidiMessage(ofxMidiMessage& args) {
	int pressed = args.velocity > 0;
	int grid = args.status == MIDI_NOTE_ON;
	if(grid) {
		int row = (args.pitch >> 4) & rowMask;
		int col = (args.pitch >> 0) & colMask;
		int i = row * cols + col;
		ButtonEvent event(col, row, pressed, &lastEvent[i]);
		ofNotifyEvent(gridButtonEvent, event);
		lastEvent[i] = event;
	} else {
		int row = automapRow;
		int col = (args.pitch - automapBegin) & colMask;
		int i = row * cols + col;
		ButtonEvent event(col, row, pressed, &lastEvent[i]);
		ofNotifyEvent(automapButtonEvent, event);
		lastEvent[i] = event;
	}
}
