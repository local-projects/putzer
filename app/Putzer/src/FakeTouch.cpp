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
	cursor = _server->tuioServer->addTuioCursor(cursorPosition.x, cursorPosition.y);

	cursorIsActive = true;
}

//--------------------------------------------------------------

void FakeTouch::update()
{

	// MOVE THE CURSOR IF IT IS A SWIPE ////////////////////
	if (cursorIsActive) {
		if (isSwipe) {
			
			// Wiggle
			if (moveDir.x != 0 && moveDir.y != 0){
				moveDir.x += ofRandom(-0.001,		0.001);
				moveDir.y += ofRandom(-0.001,		0.001);
			}
			
			// Bounce
			if (cursorPosition.x < 0.0001 || cursorPosition.x > 0.9999){
				moveDir.x *= -1;
			}
			if (cursorPosition.y < 0.0001 || cursorPosition.y > 0.9999){
				moveDir.y *= -1;
			}

			moveDir.x = ofClamp(moveDir.x, -0.1, 0.1);
			moveDir.y = ofClamp(moveDir.y, -0.1, 0.1);
			
			cursorPosition += moveDir;
			cursorPosition.x = ofClamp(cursorPosition.x, 0, 1);
			cursorPosition.y = ofClamp(cursorPosition.y, 0, 1);
			server->tuioServer->updateTuioCursor(cursor, cursorPosition.x, cursorPosition.y);
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
	ofPoint mappedPosition = cursorPosition;
	mappedPosition.x = ofMap(cursorPosition.x, 0, 1, 220, ofGetWidth());
	mappedPosition.y = ofMap(cursorPosition.y, 0, 1, 0, ofGetHeight());
	
	
	
	ofDrawCircle(mappedPosition.x, mappedPosition.y, radius);
	ofPopStyle();
}