/*
 this is a fairly straightforward implementation of the spec for the novation
 launchpad, as described in the programmer's guide. the 'low level methods' are
 taken almost directly from the documentation.
 
 setAll() handles the LED test mode, and also incorporates the reset function
 with OFF_BRIGHTNESS_MODE.
*/

#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

class ofxLaunchpad {
public:	
	enum MappingMode {XY_MAPPING_MODE, DRUM_MAPPING_MODE};
	enum BrightnessMode {OFF_BRIGHTNESS_MODE, LOW_BRIGHTNESS_MODE, MEDIUM_BRIGHTNESS_MODE, FULL_BRIGHTNESS_MODE};
	
	void setup(int port);
	
	// high level methods
	void set(ofPixels& pix, bool clear = true, bool copy = true);
	void setBrightness(float brightness); // 0 to 1
	
	// low level methods
	void setLed(int row, int col, int red, int green, bool clear = true, bool copy = true);
	void reset();
	void setMappingMode(MappingMode mappingMode);
	void setBufferingMode(bool copy, bool flash, int update, int display); // also resets the flash timer
	void setAll(BrightnessMode brightnessMode = OFF_BRIGHTNESS_MODE); // resets all other data
	void setDutyCycle(int numerator, int denominator);
	
protected:
	ofxMidiOut midiOut;
	ofxMidiIn midiIn;
};
