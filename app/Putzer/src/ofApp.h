//
//  ofApp.h
//  Putzer
//
//  Created by Eric Rieper on 3/31/16.
//
//

#pragma once

#include "FakeTouch.hpp"
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxTuioServer.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofxTuioServer *myTuioServer;
	TuioCursor *cursor;
	vector<FakeTouch *> fakeTouches;

	ofxXmlSettings XML;
	
	void setupGui();
	ofxPanel *gui;
	ofxIntSlider		*touchFrequency, *numTouches;
	ofxFloatSlider	*maxSwipeLengthSlider, *minTouchDir, *maxTouchDir;
	ofxToggle				*putzerEnabledToggle;
	ofxToggle				*marchHorizontalToggle, *marchVerticalToggle;
	ofxFloatSlider	*horizontalSweepSpeed, *verticalSweepSpeed;
	ofxToggle				*sineHorizontalToggle, *sineVerticalToggle;
	ofxIntSlider		*swipeLikelyhood;
	ofxToggle				*horizontalSwipes, *verticalSwipes;
	ofxLabel				*ipAddressLabel, *portLabel, *touchCountLabel;
	ofxGuiGroup			*infoGroup, *touchesGroup, *motionGroup;
	
	bool shouldDrawFadeIn = true;
	float fadeInCounter = -15;
	

	void createtouch();
};
