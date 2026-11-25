#include "ofMain.h"
#include "ofApp.h"
#include "ofxNvApi.h"

//========================================================================
int main( ){

	bool ok = ofxNvApi::setThreadedOptimizationOff(ofxNvApi::Scope::AppSpecific);
	if (ok) ofLogNotice() << "Threaded optimisation off for this app";
	else ofLogNotice() << "Failed to set app-specific threaded optimisation";

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();

}
