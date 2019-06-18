#pragma once

#include "ofColor.h"

class ofxLaunchpadColor {
public:
	enum BrightnessMode {OFF_BRIGHTNESS_MODE, LOW_BRIGHTNESS_MODE, MEDIUM_BRIGHTNESS_MODE, FULL_BRIGHTNESS_MODE};

	int red, green;
	bool clear, copy;

	ofxLaunchpadColor(int red = 0, int green = 0, bool clear = true, bool copy = true);
	ofxLaunchpadColor(bool on, bool clear = true, bool copy = true);
	ofxLaunchpadColor(BrightnessMode brightnessMode);
	ofxLaunchpadColor(ofColor color);
	operator ofColor() const;
	int getMidi() const;
	bool isOn() const;
	bool isRed() const;
	bool isGreen() const;

	bool operator==(ofxLaunchpadColor& color) const;
	bool operator!=(ofxLaunchpadColor& color) const;
};
