//
//  RainDrop.cpp
//  tuioRain
//
//  Created by Eric Rieper on 3/31/16.
//
//

#include "RainDrop.hpp"

RainDrop::~RainDrop(){

}

void RainDrop::retire(){
	delete this;
}

RainDrop::RainDrop(ofPoint _startPosition, float _releaseTime, ofVec2f _swipeDir, bool _isSwipe, ofxTuioServer *_server){
	
	server =					_server;
	startTime =				ofGetElapsedTimef();
	releaseTime =			startTime + _releaseTime;
	bool isSwipe =		_isSwipe;
	cursorPosition =	_startPosition;
	
	moveDir.set(0,0);
	if (isSwipe){
		moveDir = _swipeDir;
	}
	
	cursor =					_server->addCursor(cursorPosition.x, cursorPosition.y);
	cursorIsActive =	true;
	
	
}

void RainDrop::update(){
	
	if (isSwipe){
		cursorPosition += moveDir;
	}
	
	if (ofGetElapsedTimef() > releaseTime){
		if (cursorIsActive){
			server->removeCursor(cursor);
			cursorIsActive = false;
		}
	}
}