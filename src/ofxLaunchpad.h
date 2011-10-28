/* 
 "the grid" refers to the main 8x8 grid as well as the 8 buttons on the right.
 the 8 buttons on the right are in column 8 while the 8 buttons on the left are
 in column 0. the row of buttons along the top are called the "automap" buttons.
 they can be set using setLedAutomap(), or using setLedGrid() at row 8.
 
 setAll() handles the LED test mode, and also incorporates the reset function
 via OFF_BRIGHTNESS_MODE.
*/

#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxLaunchpadListener.h"
#include "ofxLaunchpadColor.h"

class ofxLaunchpad : public ofxMidiListener {
public:	
	virtual void setup(int port, ofxLaunchpadListener* listener = NULL);
	void addListener(ofxLaunchpadListener* listener);
	void removeListener(ofxLaunchpadListener* listener);
	
	void draw(int x, int y, int size) const;
	
	void set(ofPixels& pix, bool clear = true, bool copy = true);
	void setBrightness(float brightness); // 0 to 1
	void setLedAutomap(int col, ofxLaunchpadColor color);
	void setLedGrid(int col, int row, ofxLaunchpadColor color);
	ofxLaunchpadColor getLedGrid(int col, int row) const;
	void setBufferingMode(bool copy, bool flash, int update, int display); // also resets the flash timer
	void setAll(ofxLaunchpadColor::BrightnessMode brightnessMode = ofxLaunchpadColor::OFF_BRIGHTNESS_MODE); // resets all other data
	
	void newMidiMessage(ofxMidiEventArgs& args);
	ofEvent<ButtonEvent> automapButtonEvent, gridButtonEvent;
	
protected:
	ofxMidiOut midiOut;
	ofxMidiIn midiIn;
	
	enum MappingMode {XY_MAPPING_MODE, DRUM_MAPPING_MODE};
	void setMappingMode(MappingMode mappingMode = XY_MAPPING_MODE);
	void setDutyCycle(int numerator, int denominator);
	
	vector<ofxLaunchpadColor> buffer;
};

// <3 kyle
