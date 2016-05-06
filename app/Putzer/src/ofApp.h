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
	
	// MAIN
	void setup();
	void update();
	void draw();
	void createtouch();

	// EVENTS
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
	void buttonPressed(const void * sender);

	// TUIO
	ofxTuioServer				*myTuioServer;
	TuioCursor					*cursor;
	vector<FakeTouch *> fakeTouches;

	// GUI
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
	ofxButton				*resetRegionButton;
	ofxLabel				*ipAddressLabel, *portLabel, *touchCountLabel;
	ofxGuiGroup			*infoGroup, *touchesGroup, *motionGroup;
	
	// MISC
	double					rounder(float _in, int _precision);
	ofRectangle			touchRegionRect;
	bool						isDraggingRect = false;
	ofPoint					oldTopLeft, oldBottomRight;
	bool						shouldDrawFadeIn = true;
	float						fadeInCounter = -15;
};
