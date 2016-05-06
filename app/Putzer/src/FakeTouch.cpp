//
//  FakeTouch.cpp
//  Putzer
//
//  Created by Eric Rieper on 3/31/16.
//
//

#include "FakeTouch.hpp"

//--------------------------------------------------------------

FakeTouch::~FakeTouch() {}

//--------------------------------------------------------------

FakeTouch::FakeTouch(ofPoint _startPosition, float _releaseTime, ofVec2f _swipeDir, bool _isSwipe, ofxTuioServer *_server)
{
	server = _server;
	startTime = ofGetElapsedTimef();
	releaseTime = startTime + _releaseTime;
	isSwipe = _isSwipe;
	cursorPosition = _startPosition;

	moveDir.set(0, 0);
	if (isSwipe) {
		moveDir = _swipeDir;
	}

	// ADD THE NEW CURSOR TO THE SERVER ////////////////////
	cursor = _server->addCursor(cursorPosition.x, cursorPosition.y);

	cursorIsActive = true;
}

//--------------------------------------------------------------

void FakeTouch::update()
{

	// MOVE THE CURSOR IF IT IS A SWIPE ////////////////////
	if (cursorIsActive) {
		if (isSwipe) {
			cursorPosition += moveDir;
			cursorPosition.x = ofClamp(cursorPosition.x, 0, ofGetWidth());
			cursorPosition.y = ofClamp(cursorPosition.y, 0, ofGetHeight());
			server->updateCursor(cursor, cursorPosition.x, cursorPosition.y);
		}
	}

	// REMOVE THE CURSOR IF IT HAS EXPIRED /////////////////
	if (ofGetElapsedTimef() > releaseTime) {
		if (cursorIsActive) {
			server->removeCursor(cursor);
			cursorIsActive = false;
		}
	}
}

//--------------------------------------------------------------

void FakeTouch::draw()
{
	ofPushStyle();
	float radius = ofMap(releaseTime - ofGetElapsedTimef(), 0, releaseTime - startTime, 5, 8); // Slightly shrink circle
	ofSetColor(ofColor::white);																																 // over its life.
	ofDrawCircle(cursorPosition.x, cursorPosition.y, radius);
	ofPopStyle();
}