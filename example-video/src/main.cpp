#include "ofMain.h"
#include "testApp.h"

int main() {
	ofSetupOpenGL(480, 480, OF_WINDOW);
	ofRunApp(new testApp());
}
