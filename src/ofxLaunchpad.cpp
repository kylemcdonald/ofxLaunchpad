#include "ofxLaunchpad.h"

// if you click off the window, there will be too many messages
// and the midi will fill up and have trouble
// need some kind of rate limiting buffer?

const int numeratorMin = 1, numeratorMax = 16;
const int denominatorMin = 3, denominatorMax = 18;
const int colorMask = 0x03;
const int bufferMask = 0x01;
	
void ofxLaunchpad::setup(int port) {
	midiIn.openPort(port);
	midiOut.openPort(port);
	setAll(OFF_BRIGHTNESS_MODE);
	setMappingMode(XY_MAPPING_MODE);
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

int getMode(int red, int green, bool clear, bool copy) {
	return 
		((green & colorMask) << 4) |
		((clear ? 1 : 0) << 3) |
		((copy ? 1 : 0) << 2) |
		((red & colorMask) << 0);
}

void ofxLaunchpad::setGridLed(int row, int col, int red, int green, bool clear, bool copy) {
	int key = (row << 4) | (col << 0);
	midiOut.sendNoteOn(1, key, getMode(red, green, clear, copy));
}

void ofxLaunchpad::setAutomapLed(int col, int red, int green, bool clear, bool copy) {
	int key = 104 + col;
	midiOut.sendControlChange(1, key, getMode(red, green, clear, copy));
}

void ofxLaunchpad::set(ofPixels& pix, bool clear, bool copy) {
	int i = 0;
	for(int y = 0; y < 8; y++) {
		for(int x = 0; x < 8;) {
			vector<int> velocity;
			for(int j = 0; j < 2; j++) {
				ofColor cur = pix.getColor(x, y);
				int red = ofMap(cur.r, 0, 255, 0, 3, true);
				int green = ofMap(cur.g, 0, 255, 0, 3, true);
				int colorMask = 0x03;
				velocity.push_back(
					((green & colorMask) << 4) |
					((clear ? 1 : 0) << 3) |
					((copy ? 1 : 0) << 2) |
					((red & colorMask) << 0));
				x++;
			}
			midiOut.sendNoteOn(3, velocity[0], velocity[1]);
		}
	}
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

void ofxLaunchpad::setAll(BrightnessMode brightnessMode) {
	int mode;
	switch(brightnessMode) {
		case OFF_BRIGHTNESS_MODE: mode = 0; break;
		case LOW_BRIGHTNESS_MODE: mode = 125; break;
		case MEDIUM_BRIGHTNESS_MODE: mode = 126; break;
		case FULL_BRIGHTNESS_MODE: mode = 127; break;
	}
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
