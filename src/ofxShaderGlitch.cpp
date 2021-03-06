//
//  ofxShaderGlitch.cpp
//  Gradient
//
//  Created by Pierre Tardif on 10/05/2020.
//	modified fork by moebiusSurfing

#include "ofxShaderGlitch.h"

//--------------------------------------------------------------
void ofxShaderGlitch::exit() {

#ifdef USE_PRESETS_MANAGER
	presetsManager.exit();//only required to store some gui/class settings.
#endif

	ofRemoveListener(params.parameterChangedE(), this, &ofxShaderGlitch::Changed_Params);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxShaderGlitch::keyPressed);

	ofRemoveListener(ofEvents().update, this, &ofxShaderGlitch::update);
}

//--------------------------------------------------------------
void ofxShaderGlitch::setup() {
	setLogLevel(OF_LOG_NOTICE);

	// presetsManager
	setupPresetsManager();

	ofAddListener(params.parameterChangedE(), this, &ofxShaderGlitch::Changed_Params);
	ofAddListener(ofEvents().keyPressed, this, &ofxShaderGlitch::keyPressed);

	ofAddListener(ofEvents().update, this, &ofxShaderGlitch::update);//used only to debug
}

//--------------------------------------------------------------
void ofxShaderGlitch::update(ofEventArgs & args) {

#ifdef USE_PRESETS_MANAGER
	// debug presetsManager
	// simple callback when preset is loaded 
	if (presetsManager.isDoneLoad())
	{
		//ofLogNotice(__FUNCTION__) << "[ " << presetsManager.getGroupName() << "] ------------------------------------------------------------- PRESET LOADED";
	}
#endif
}

//--------------------------------------------------------------
void ofxShaderGlitch::begin() {
	glitch.begin();
}

//--------------------------------------------------------------
void ofxShaderGlitch::end() {
	glitch.end();
}

//--------------------------------------------------------------
void ofxShaderGlitch::drawGUI() {
	if (bVisibleGui) 
	{
#ifdef USE_GUI_INTERNAL
		gui.draw();
#endif
	}
}

//--------------------------------------------------------------
void ofxShaderGlitch::keyPressed(ofKeyEventArgs &keyArgs) {

	if (bEnabledKeys)
	{
		if (false) {}

#ifdef USE_GUI_INTERNAL
		else if (keyArgs.key == 'g') glitch.guiON = !glitch.guiON;
#endif

		else if (keyArgs.key == 'c') {
			glitch.nonMaxAndContinuity = !glitch.nonMaxAndContinuity;
			ofLogNotice(__FUNCTION__) << " maxContinuity = " << ofToString(glitch.nonMaxAndContinuity);
		}

#ifdef USE_PRESETS_MANAGER
		else if (keyArgs.key == ' ') {
			//presetsManager.setToggleRandomizerPreset();
		}

		else if (keyArgs.key == OF_KEY_BACKSPACE) {
			presetsManager.doRandomizePresetSelected();
			//presetsManager.doRandomizePresetFromFavs();
		}
#endif
	}
}

//--------------------------------------------------------------
void ofxShaderGlitch::setupPresetsManager()
{

#ifdef USE_PRESETS_MANAGER
	// customize
	presetsManager.setPath_UserKit_Folder(path_GLOBAL_Folder + "/ofxPresetsManager");
#endif

	//-

	// prepare the group and add subgroups that you want presetize:
	params = glitch.params;

#ifdef USE_GUI_INTERNAL
	gui.setup(params);
	gui.setPosition(10, 80);
#endif

	//-

	// presets

#ifdef USE_PRESETS_MANAGER
	//added ofParametegrGroup to presetsManager
	//define desired trigged keys
	presetsManager.add(params, { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' });
	presetsManager.setup();

	//customize layout
	presetsManager.setVisible_PresetClicker(true);
	int pad = 20;
	presetsManager.setPosition_PresetClicker(pad, ofGetHeight() - 50 - pad, 50);

	//final startup refresh
	//presetsManager.refresh();
#endif

	//nothing more!
	//all presets are stored on runtime to xml files.
	//there's a kind of faster 'memory mode' that stores presets on a vector, and to xml files only on exit. 
	//update, draw and autosave its handled by the addon. only must call exit method.
	//api can disable trigger keys, hide/show gui and customize layout...etc

	params_Control.setName("CONTROLS");
	params_Control.add(glitch.bEnable);

//#ifdef USE_PRESETS_MANAGER
	//params_Control.add(presetsManager.PRESET_selected);
	//params_Control.add(presetsManager.PLAY_RandomizeTimer);
//#endif

	params_Control.add(glitch.typeEffectName.set("TYPE", ""));
	params_Control.add(glitch.bEnableBlur);
	params_Control.add(glitch.bReset);
}

//--------------------------------------------------------------
void ofxShaderGlitch::setPath_GlobalFolder(string folder)
{
	ofLogNotice(__FUNCTION__) << folder;
	path_GLOBAL_Folder = folder;
	CheckFolder(folder);
}

//--------------------------------------------------------------
void ofxShaderGlitch::Changed_Params(ofAbstractParameter &e)
{
	string name = e.getName();

	if ((name != "exclude") &&
		(name != glitch.typeT.getName()) &&
		(name != glitch.typeE.getName())
		)
	{
		ofLogNotice(__FUNCTION__) << name << " : " << e;
	}

	//-

	//if (name == "ENABLE FEEDBACK")
	//{
	//	if (!glitch.feedbackEdge.activateFeedback.get()) {
	//		glitch.typeT = 0;
	//		glitch.typeE = 0;
	//	}
	//}

	//if (name == glitch.typeT.getName())
	//{
	//	refreshGUI();
	//}
}

#ifdef USE_GUI_INTERNAL
//--------------------------------------------------------------
void ofxShaderGlitch::refreshGUI()
{
	/*
		5 - > 9 : patterns generation - parameters to control are :
		Mask Layers for tyling 5,
		thresholdNoise for tyling 6,
		continuous for 7->9.
		Speed.y is controlling the speed of the effects for tyling 5 -> 9.
		RectangleSize controls the gradiant rect boundaries, and gradiantColor controls its color.
		10 : makes the effect apply to the whole screen.
		Tyling is processed in the main.frag by setting up the prop variable.
	*/
	//gui.minimizeAll();

	auto &gA = gui.getGroup(glitch.typeTiling.getName());
	auto &gB = gui.getGroup(glitch.typeEffect.getName());

	//mode 1
	switch (glitch.typeT.get())
	{
	case 1:
	case 2:
	case 3:
		gA.maximize();
		gA.getGroup("CELL").minimize();
		gB.minimize();
		break;
	case 4:
		gA.getGroup("CELL").maximize();
		gA.getGroup("CELL").getGroup("OFFSET").minimize();
		break;
	case 5://mask layers
	case 6:
	case 7:
	case 8:
	case 9:
		gA.minimize();
		gA.getGroup("CELL").minimize();
		gB.maximize();
		break;
	}
}
#endif