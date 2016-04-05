//
//  FakeTouch.cpp
//  Putzer
//
//  Created by Eric Rieper on 3/31/16.
//
//

#include "FakeTouch.hpp"

FakeTouch::~FakeTouch(){
}

void FakeTouch::retire(){
	delete this;
}

FakeTouch::FakeTouch(ofPoint _startPosition, float _releaseTime, ofVec2f _swipeDir, bool _isSwipe, ofxTuioServer *_server){
	
	server =					_server;
	startTime =				ofGetElapsedTimef();
	releaseTime =			startTime + _releaseTime;
	bool isSwipe =		_isSwipe;
	cursorPosition =	_startPosition;
	
	moveDir.set(0,0);
	if (isSwipe){
		moveDir = _swipeDir;
	}
	
	// ADD THE NEW CURSOR TO THE SERVER ////////////////////
	cursor =					_server->addCursor(cursorPosition.x, cursorPosition.y);
	cursorIsActive =	true;
	
	
}

void FakeTouch::update(){
	
	// MOVE THE CURSOR IF IT IS A SWIPE ////////////////////
	if (isSwipe){
		cursorPosition += moveDir;
	}
	
	// REMOVE THE CURSOR IF IT HAS EXPIRED /////////////////
	if (ofGetElapsedTimef() > releaseTime){
		if (cursorIsActive){
			server->removeCursor(cursor);
			cursorIsActive = false;
		}
	}
	
}