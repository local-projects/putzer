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

class FakeTouch {

public:
	
	~FakeTouch();
	FakeTouch(ofPoint _startPosition, float _releaseTime, ofVec2f _swipeDir, bool _isSwipe, ofxTuioServer *_server);
	
	void update();
	void retire();
	
	ofxTuioServer *server;
	TuioCursor		*cursor;
	float					startTime;
	float					releaseTime;
	bool					isSwipe;
	ofPoint				cursorPosition;
	ofVec2f				moveDir;
	bool					cursorIsActive;

};




#endif /* FakeTouch_hpp */