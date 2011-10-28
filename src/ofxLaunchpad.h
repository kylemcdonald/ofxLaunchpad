/*
 this is a fairly straightforward implementation of the spec for the novation
 launchpad, as described in the programmer's guide. the "low level methods" are
 taken almost directly from the documentation.
 
 for the purposes of the low level functions, "the grid" refers to the main 8x8
 grid as well as the 8 buttons on the right. the 8 buttons on the right are in
 column 8 while the 8 buttons on the left are in column 0.
 
 the row of buttons along the top are called the "automap" buttons.
 
 setAll() handles the LED test mode, and also incorporates the reset function
 with OFF_BRIGHTNESS_MODE.
*/

#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxLaunchpadListener.h"
#include "ofxLaunchpadColor.h"

class ofxLaunchpad : ofxMidiListener {
public:	
	enum MappingMode {XY_MAPPING_MODE, DRUM_MAPPING_MODE};
	enum BrightnessMode {OFF_BRIGHTNESS_MODE, LOW_BRIGHTNESS_MODE, MEDIUM_BRIGHTNESS_MODE, FULL_BRIGHTNESS_MODE};
	
	virtual void setup(int port, ofxLaunchpadListener* listener = NULL);
	void addListener(ofxLaunchpadListener* listener);
	void removeListener(ofxLaunchpadListener* listener);
	
	// high level methods
	void set(ofPixels& pix, bool clear = true, bool copy = true);
	void setBrightness(float brightness); // 0 to 1
	
	// low level methods
	void setMappingMode(MappingMode mappingMode);
	void setBufferingMode(bool copy, bool flash, int update, int display); // also resets the flash timer
	void setAll(BrightnessMode brightnessMode = OFF_BRIGHTNESS_MODE); // resets all other data
	void setDutyCycle(int numerator, int denominator);
	void setLedAutomap(int col, ofxLaunchpadColor color);
	void setLedGrid(int col, int row, ofxLaunchpadColor color);
	
	void newMidiMessage(ofxMidiEventArgs& args);
	ofEvent<ButtonEvent> automapButtonEvent, gridButtonEvent;
	
protected:
	ofxMidiOut midiOut;
	ofxMidiIn midiIn;
};

// <3 kyle
