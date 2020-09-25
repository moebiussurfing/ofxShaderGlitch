#pragma once

#include "ofMain.h"

#include "ofxShaderGlitch.h"

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);

	//source
	void switchInput();
	ofImage image;
	ofVideoGrabber vidGrabber;
	//ofTexture t;

	//--

	ofxShaderGlitch shaderGlitch;
	
	bool bShowGui = true;
	int indexInput;
	int indexImage;
	string pathImage;
	ofTrueTypeFont 	font;
};

