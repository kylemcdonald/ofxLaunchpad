#pragma once

class ofxLaunchpadColor {
public:
	int red, green;
	bool clear, copy;
	ofxLaunchpadColor(int red = 0, int green = 0, bool clear = true, bool copy = true)
	:red(red)
	,green(green)
	,clear(clear)
	,copy(copy) {
	}
	ofxLaunchpadColor(ofColor color) {
		red = ofMap(color.r, 0, 255, 0, 3, true);
		green = ofMap(color.g, 0, 255, 0, 3, true);
	}
	operator ofColor() const {
		ofColor color;
		color.r = ofMap(red, 0, 3, 0, 255);
		color.g = ofMap(green, 0, 3, 0, 255);
		color.b = 0;
		return color;
	}
	operator int() const {
		static const int colorMask = 3;
		return 
			((green & colorMask) << 4) |
			((clear ? 1 : 0) << 3) |
			((copy ? 1 : 0) << 2) |
			((red & colorMask) << 0);
	}
};