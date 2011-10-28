#include "ofxLaunchpad.h"

// if you click off the window, there will be too many messages
// and the midi will fill up and have trouble
// need some kind of rate limiting buffer?
void ofxLaunchpad::setLed(int key, int velocity) {
	midiOut.sendNoteOn(1, key, velocity);
}

void ofxLaunchpad::reset() {
	midiOut.sendControlChange(1, 0, 0);
}

void ofxLaunchpad::setMappingMode(MappingMode mappingMode) {
	midiOut.sendControlChange(1, 0, mappingMode == LAUNCHPAD_XY_MODE ? 1 : 2);
}


void ofxLaunchpad::setLed(int row, int col, int red, int green, bool clear, bool copy) {
	int key = (row << 4) | col;
	int colorMask = 0x03;
	int velocity =
	((green & colorMask) << 4) |
	((clear ? 1 : 0) << 3) |
	((copy ? 1 : 0) << 2) |
	((red & colorMask) << 0);
	midiOut.sendNoteOn(1, key, velocity);
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

void ofxLaunchpad::setAll(int red, int green, bool clear, bool copy) {
	int colorMask = 0x03;
	int velocity =
	((green & colorMask) << 4) |
	((clear ? 1 : 0) << 3) |
	((copy ? 1 : 0) << 2) |
	((red & colorMask) << 0);
	for(int i = 0; i < 40; i++) {
		midiOut.sendNoteOn(3, velocity, velocity);
	}
}

void ofxLaunchpad::setup(int port) {
	midiIn.openPort(port);
	midiOut.openPort(port);
	reset();
	setMappingMode(LAUNCHPAD_XY_MODE);
}