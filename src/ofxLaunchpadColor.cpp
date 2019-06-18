#include "ofxLaunchpadColor.h"
#include "ofMath.h"

ofxLaunchpadColor::ofxLaunchpadColor(int red, int green, bool clear, bool copy)
:red(red)
,green(green)
,clear(clear)
,copy(copy) {
}

ofxLaunchpadColor::ofxLaunchpadColor(BrightnessMode brightnessMode)
:clear(true)
,copy(true) {
	switch(brightnessMode) {
		case OFF_BRIGHTNESS_MODE: red = green = 0; break;
		case LOW_BRIGHTNESS_MODE: red = green = 1; break;
		case MEDIUM_BRIGHTNESS_MODE: red = green = 2; break;
		case FULL_BRIGHTNESS_MODE: red = green = 3; break;
	}
}

ofxLaunchpadColor::ofxLaunchpadColor(bool on, bool clear, bool copy)
:clear(clear)
,copy(copy) {
	red = green = (on ? 3 : 0);
}

ofxLaunchpadColor::ofxLaunchpadColor(ofColor color) {
	red = ofMap(color.r, 0, 255, 0, 3, true);
	green = ofMap(color.g, 0, 255, 0, 3, true);
}

ofxLaunchpadColor::operator ofColor() const {
	ofColor color;
	color.r = ofMap(red, 0, 3, 0, 255);
	color.g = ofMap(green, 0, 3, 0, 255);
	color.b = 0;
	return color;
}

int ofxLaunchpadColor::getMidi() const {
	static const int colorMask = 3;
	return
		((green & colorMask) << 4) |
		((clear ? 1 : 0) << 3) |
		((copy ? 1 : 0) << 2) |
		((red & colorMask) << 0);
}

bool ofxLaunchpadColor::isOn() const {
	return red > 0 || green > 0;
}

bool ofxLaunchpadColor::isRed() const {
	return red == 3 && green == 0;
}

bool ofxLaunchpadColor::isGreen() const {
	return red == 0 && green == 3;
}

bool ofxLaunchpadColor::operator==(ofxLaunchpadColor& color) const {
	return getMidi() == color.getMidi();
}

bool ofxLaunchpadColor::operator!=(ofxLaunchpadColor& color) const {
	return getMidi() != color.getMidi();
}
