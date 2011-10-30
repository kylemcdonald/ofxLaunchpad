#include "ofMain.h"
#include "testApp.h"

int main() {
	ofSetupOpenGL(32 * 9, 32 * 9, OF_WINDOW);
	ofRunApp(new testApp());
}
