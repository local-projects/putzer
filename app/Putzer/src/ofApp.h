//
//  ofApp.h
//  Putzer
//
//  Created by Eric Rieper on 3/31/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxTuioServer.h"
#include "FakeTouch.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		ofxTuioServer			*myTuioServer;
		TuioCursor				*cursor;
		vector<FakeTouch*> fakeTouches;
	
		ofxPanel				gui;
		ofxXmlSettings	XML;
		ofxIntSlider		touchFrequency, numTouches;
		ofxFloatSlider	maxSwipeLengthSlider, minTouchDir, maxTouchDir;
		ofxToggle				putzerEnabledToggle;
		ofxToggle				marchHorizontalToggle, marchVerticalToggle;
		ofxFloatSlider	horizontalSweepSpeed, verticalSweepSpeed;
		ofxToggle				sineHorizontalToggle, sineVerticalToggle;
		ofxLabel				ipAddressLabel, portLabel, touchCountLabel;
		ofxGuiGroup			infoGroup, touchesGroup, motionGroup;
	
		void						createtouch();
	
};
