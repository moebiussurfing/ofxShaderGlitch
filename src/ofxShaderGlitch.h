//
//  ofxShaderGlitch.hpp
//  Gradient
//
//  Created by Pierre Tardif on 10/05/2020.
//
//	this fork is modified by moebiusSurfing:
//	includes an optional preset manager engine and some helpers to integrate in other projects or addons.
//	

///	TODO:
///	+	reset per zone
///	+	workflow helper and text info
///	+	make play toggle public to expose to gui
///	+	make 2 public groups to split into external gui

#pragma once

#include "ofMain.h"

//--------

// DEFINES

//#define USE_PRESETS_MANAGER
#define USE_GUI_INTERNAL

//--------

#include "ofxAutoReloadedShader.h"
#include "GlitchManager/GlitchManager.hpp"

#ifdef USE_PRESETS_MANAGER
#include "ofxPresetsManager.h"
#endif

#ifdef USE_GUI_INTERNAL
#include "ofxGui.h"
#endif

class ofxShaderGlitch {
public:

	ofxShaderGlitch() {
		//setPath_GlobalFolder("ofxShaderGlitch");
		//NOTE: this above line will force create the folder if do not exist at startup,
		//and it's not the desired behaviour if we are using the addon in another addon like ofxFboFxHelper
		//bc we will have an empty folder.. So:

		path_GLOBAL_Folder = "ofxShaderGlitch";

		bEnabledKeys = false;
	};		

	~ofxShaderGlitch() {};

	void setup();
	void exit();
	
	void keyPressed(ofKeyEventArgs &keyArgs);
	bool bEnabledKeys;
	void setKeysEnabled(bool b) {
		bEnabledKeys = b;

#ifdef USE_PRESETS_MANAGER
		presetsManager.setEnableKeys(b);
#endif
	}

	//shader
//private:
public:
	GlitchManager glitch;//TODO: would like to make public only required params!

public:
	ofParameterGroup params_Control;

	//--------------------------------------------------------------
	ofParameterGroup getParamGroup() {
		return glitch.params;
	}
	//--------------------------------------------------------------
	ofParameterGroup getParamGroup_Tiling() {
		return glitch.typeTiling;
	}
	//--------------------------------------------------------------
	ofParameterGroup getParamGroup_TypeEffect() {
		return glitch.typeEffect;
	}
	//--------------------------------------------------------------
	ofParameterGroup getParamGroup_Control() {
		return params_Control;
	}

	//--------------------------------------------------------------
	void setLogLevel(ofLogLevel level)
	{
		ofSetLogLevel("ofxShaderGlitch", level);
	}
	//--------------------------------------------------------------
	void doReset()
	{
		glitch.doReset();
	}
	
	//-

	// feed the fx
public:
	void begin();
	void end();

	//-

	// TODO:
	// optional to debug..
	void update(ofEventArgs & args);

#ifdef USE_GUI_INTERNAL
public:
	void drawGUI();
private:
	void refreshGUI();
#endif

private:
	string path_GLOBAL_Folder;

public:
	void setPath_GlobalFolder(string folder);// path for root container folder


#ifdef USE_PRESETS_MANAGER
public:
	// presetsManager
	ofxPresetsManager presetsManager;
#endif

public:
	ofParameterGroup params;

private:
	void setupPresetsManager();
	void Changed_Params(ofAbstractParameter &e);

private:
#ifdef USE_GUI_INTERNAL
	ofxPanel gui;// internal gui
#endif
	bool bVisibleGui = false;

public:
	//--------------------------------------------------------------
	void setVisibleGui(bool b) {
		bVisibleGui = b;

#ifdef USE_PRESETS_MANAGER
		//presetsManager.setVisible_GUI_ImGui(bVisibleGui);
		presetsManager.setVisible_PresetClicker(bVisibleGui);
#endif
	}

	//--------------------------------------------------------------
	void setToggleVisibleGui() {
		bVisibleGui = !bVisibleGui;
		
#ifdef USE_PRESETS_MANAGER
		//presetsManager.setVisible_GUI_ImGui(bVisibleGui);
		presetsManager.setVisible_PresetClicker(bVisibleGui);
#endif
	}

	//--

private:
	//--------------------------------------------------------------
	void CheckFolder(string _path)
	{
		ofLogNotice(__FUNCTION__) << _path;

		ofDirectory dataDirectory(ofToDataPath(_path, true));

		//check if folder path exist
		if (!dataDirectory.isDirectory())
		{
			ofLogError(__FUNCTION__) << "FOLDER NOT FOUND! TRYING TO CREATE...";

			//try to create folder
			bool b = dataDirectory.createDirectory(ofToDataPath(_path, true));

			//debug if creation has been succeded
			if (b) ofLogNotice(__FUNCTION__) << "CREATED '" << _path << "'  SUCCESSFULLY!";
			else ofLogError(__FUNCTION__) << "UNABLE TO CREATE '" << _path << "' FOLDER!";
		}
		else
		{
			ofLogNotice(__FUNCTION__) << "OK! LOCATED FOLDER: '" << _path << "'";//nothing to do
		}
	}
};

