#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

class ofxLaunchpad {
public:	
	enum MappingMode {LAUNCHPAD_XY_MODE, LAUNCHPAD_DRUM_MODE};
	
	void setup(int port);
	
	void setLed(int key, int velocity);
	void reset();
	void setMappingMode(MappingMode mappingMode);
	void setLed(int row, int col, int red, int green, bool clear = true, bool copy = true);
	void setAll(int red, int green, bool clear = true, bool copy = true);
	void set(ofPixels& pix, bool clear = true, bool copy = true);	
	
protected:
	ofxMidiOut midiOut;
	ofxMidiIn midiIn;
};
