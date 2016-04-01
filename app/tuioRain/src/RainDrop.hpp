//
//  RainDrop.hpp
//  tuioRain
//
//  Created by Eric Rieper on 3/31/16.
//
//

#ifndef RainDrop_hpp
#define RainDrop_hpp

#include "ofMain.h"
#include "ofxTuioServer.h"

class RainDrop {

public:
	
	~RainDrop();
	RainDrop(ofPoint _startPosition, float _releaseTime, ofVec2f _swipeDir, bool _isSwipe, ofxTuioServer *_server);
	
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




#endif /* RainDrop_hpp */
