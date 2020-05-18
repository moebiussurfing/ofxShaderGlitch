//
//  GlitchManager.hpp
//  CellTypes
//
//  Created by Pierre Tardif on 27/03/2020.
//

#ifndef GlitchManager_hpp
#define GlitchManager_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxGui.h"

// for faster live coding
#include "ofxAutoReloadedShader.h"

// Cells
#include "CellManager/CellManager.hpp"

class GlitchManager{
    
    public :
    GlitchManager(){
        setup();
    }
    
    void setup();
    void begin();
    void end();
    
    bool guiON;
    ofParameterGroup typeTiling;
    ofParameter < int > type;
    ofParameter < ofVec2f > amntLinesColumns;
    ofParameter < ofVec2f > speedMoves;
    ofParameter < ofVec4f > props;
    ofParameter < float > alphaGradiant;
    ofParameter < ofColor > gradiantColor;
    ofParameter < bool > continuousMosh;
    
    void initGui();
    void drawGUI(){if(guiON)gui.draw();}
    
    void setUniform1f(string name, float floatName);
    
    ofxPanel gui;
    
    ofVec2f resImg;
    ofxAutoReloadedShader shader;
    CellManager cells;
    
    ofFbo f;
    ofxAutoReloadedShader gaussian;
    bool nonMaxAndContinuity;
    
};

#endif /* GlitchManager_hpp */