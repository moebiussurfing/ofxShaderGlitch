#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetBackgroundColor(16);
	indexInput = 0;

	indexImage = 0;
	pathImage = "images/TheDavidFace.png";
	image.loadImage(pathImage);

	font.loadFont(ofToDataPath("Fonts/DIN.otf"), 8);

	shaderGlitch.setup();

	shaderGlitch.setVisibleGui(bShowGui);
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (indexInput == 1) { // webcam
		vidGrabber.update();

		//if (vidGrabber.isFrameNew()) {
		//	t = vidGrabber.getTexture();
		//}
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	shaderGlitch.begin();
	{
		// draw scene

		// image
		if (indexInput == 0)
		{
			ofRectangle r(0, 0, image.getWidth(), image.getHeight());
			r.scaleTo(ofGetWindowRect());
			image.draw(r.x, r.y, r.width, r.height);
		}
		// webcam
		else if (indexInput == 1) {
			vidGrabber.draw(0, 0, ofGetWidth(), ofGetHeight());
			//t.draw(0, 0);
		}
	}
	shaderGlitch.end();

	//--

	if (bShowGui)
	{
		ofVec2f pos(10, 20);

		string _info;
		_info += "g: SHOW GUI\n";
		_info += "i: CHANGE INPUT\n";
		_info += "down: NEXT IMAGE " + pathImage + "\n";
		_info += ofToString(ofGetFrameRate(), 0) + " FPS";

		ofSetColor(0);
		if (font.isLoaded())font.drawString(_info, pos.x + 1, pos.y + 1);
		else ofDrawBitmapString(_info, pos.x + 1, pos.y + 1);
		ofSetColor(255);
		if (font.isLoaded())font.drawString(_info, pos.x, pos.y);
		else ofDrawBitmapString(_info, pos.x, pos.y);

		shaderGlitch.drawGUI();//use addon internal gui panel
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// input camera or image
	if (key == 'i') switchInput();

	// gui
	if (key == 'g') {
		bShowGui = !bShowGui;

		shaderGlitch.setVisibleGui(bShowGui);
	}

	// browse images
	if (key == OF_KEY_DOWN) {
		indexImage++;
		if (indexImage == 3) indexImage = 0;
		switch (indexImage)
		{
		case 0:
			pathImage = "images/TheDavidFace.png";
			break;
		case 1:
			pathImage = "images/david.jpg";
			break;
		case 2:
			pathImage = "images/jjfGOPRO.png";
			break;
		}
		image.loadImage(pathImage);
	}
}

//--------------------------------------------------------------
void ofApp::exit()
{
	shaderGlitch.exit();
}

//--------------------------------------------------------------
void ofApp::switchInput() {
	indexInput += 1;
	if (indexInput > 1) indexInput = 0;

	if (indexInput == 1 && !vidGrabber.isInitialized()) {
		int _d = 0;
		vidGrabber.setDeviceID(_d);
		//vidGrabber.setDesiredFrameRate(60);
		vidGrabber.initGrabber(1920, 1080);
	}
}
