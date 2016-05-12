//
//  FakeTouch.hpp
//  Putzer
//
//  Created by Eric Rieper on 3/31/16.
//
//

#ifndef FakeTouch_hpp
#define FakeTouch_hpp

#include "ofMain.h"
#include "ofxTuioServer.h"
#include "Globals.hpp"

class FakeTouch
{

public:
	~FakeTouch();
	FakeTouch(ofPoint _startPosition, float _releaseTime, ofVec2f _swipeDir, bool _isSwipe, ofxTuioServer *_server);

	void update();
	void draw();
	void retire();

	ofxTuioServer		*server;
	TuioCursor			*cursor;
	float						startTime, releaseTime;
	bool						isSwipe = false;
	ofPoint					cursorPosition;
	ofVec2f					moveDir;
	bool cursorIsActive;
};

#endif /* FakeTouch_hpp */
