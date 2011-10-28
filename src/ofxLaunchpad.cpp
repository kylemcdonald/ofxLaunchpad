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
	
void ofxLaunchpad::setup(int port, ofxLaunchpadListener* listener) {
	midiIn.listPorts();
	midiIn.openPort(port);
	midiIn.addListener(this);
	
	midiOut.listPorts();
	midiOut.openPort(port);
	
	setAll(OFF_BRIGHTNESS_MODE);
	setMappingMode(XY_MAPPING_MODE);
	
	if(listener != NULL) {
		addListener(listener);
	}
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

void ofxLaunchpad::setLedAutomap(int col, int red, int green, bool clear, bool copy) {
	int key = automapBegin + col;
	midiOut.sendControlChange(1, key, ofxLaunchpadColor(red, green, clear, copy));
}

void ofxLaunchpad::setLedGrid(int col, int row, int red, int green, bool clear, bool copy) {
	if(row == -1) {
		setLedAutomap(col, red, green, clear, copy);
	}
	int key = ((row & rowMask) << 4) | ((col & colMask) << 0);
	midiOut.sendNoteOn(1, key, ofxLaunchpadColor(red, green, clear, copy));
}

void ofxLaunchpad::set(ofPixels& pix, bool clear, bool copy) {
	for(int y = 0; y < 8; y++) {
		for(int x = 0; x < 8; x += 2) {
			midiOut.sendNoteOn(3,
				ofxLaunchpadColor(pix.getColor(x + 0, y)),
				ofxLaunchpadColor(pix.getColor(x + 1, y)));
		}
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

void ofxLaunchpad::newMidiMessage(ofxMidiEventArgs& args) {
	int pressed = args.byteTwo > 0;
	int grid = args.status == MIDI_NOTE_ON;
	if(grid) {
		int row = (args.byteOne >> 4) & rowMask;
		int col = (args.byteOne >> 0) & colMask;
		ButtonEvent event(col, row, pressed);
		ofNotifyEvent(gridButtonEvent, event);
	} else {
		int col = args.byteOne - automapBegin;
		ButtonEvent event(col, -1, pressed);
		ofNotifyEvent(automapButtonEvent, event);
	}
}