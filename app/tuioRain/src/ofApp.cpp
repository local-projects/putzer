//
//  ofApp.cpp
//  tuioRain
//
//  Created by Eric Rieper on 3/31/16.
//
//

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

  // OF Setup
  ofBackground(200, 200, 200);
  ofSetFrameRate(60);

  // Window Setup
  ofSetWindowTitle("☔️");
  ofSetWindowShape(720, 400);

  // GUI Setup
  gui.setup();

	gui.add(infoGroup.setup("Info"));
  infoGroup.add(ipAddressLabel.setup("IP", "127.0.0.1"));
  infoGroup.add(portLabel.setup("PORT", "3333"));
	infoGroup.add(touchCountLabel.setup("CURRENT TOUCHES", "0"));
	
	gui.add(touchesGroup.setup("Touches"));
	touchesGroup.add(makeItRainToggle.setup("Make It Rain", true));
  touchesGroup.add(touchFrequency.setup("Frames Between Touches", 5, 1, 120));
	touchesGroup.add(numTouches.setup("Number of Touches",  2, 1, 10));
	touchesGroup.add(minTouchDir.setup("Min Touch Dur", 0.05, 0.01, 10));
	touchesGroup.add(maxTouchDir.setup("Max Touch Dur", 1.0, 0.01, 10));
	touchesGroup.add(maxSwipeLengthSlider.setup("Max Swipe Distance", 5, 0.0, 10.0));
	
	gui.add(motionGroup.setup("Area Motion"));
  motionGroup.add(marchHorizontalToggle.setup("Sweep Horizontally", false));
  motionGroup.add(marchVerticalToggle.setup("Sweep Vertically", false));
	motionGroup.add(sweepLengthSlider.setup("Sweep Duration",10000,100,40000));

  gui.loadFromFile("settings.xml");

  // TUIO Setup
  myTuioServer = new ofxTuioServer;

  string str = ipAddressLabel;
  char *cstr = &str[0u];

  myTuioServer->start(cstr, ofToInt(portLabel));
  myTuioServer->setVerbose(false);
	
}

//--------------------------------------------------------------
void ofApp::update() {

	for (auto drop : droplets) {
    drop->update();
  }
  myTuioServer->run();
	
	if (ofGetFrameNum() % 15 == 0){ // Update four times per second
		if (droplets.size() >= 256){
			touchCountLabel.setBackgroundColor(ofColor(100,10,40));
		}
		else {
			touchCountLabel.setBackgroundColor(gui.getBackgroundColor());
		}
		touchCountLabel = ofToString(droplets.size());
	}
	
}

//--------------------------------------------------------------
void ofApp::draw() {

  ofBackgroundGradient(ofColor(50, 60, 80), ofColor(20, 20, 30));
	
	if (makeItRainToggle && droplets.size() < 256){ // Limit total number of touches to prevent buffer out of memory error.
		if (ofGetFrameNum()%touchFrequency == 0){
			for (int i = 0; i < numTouches; i++){
				createDrop();
			}
		}
	}

  ofSetColor(ofColor::white);
  ofPushStyle();
  for (auto drop : droplets) {
    if (drop->cursorIsActive) {
      ofEnableBlendMode(OF_BLENDMODE_SCREEN);
      ofSetColor(ofColor(255, 255, 255, 200));
      ofDrawCircle(drop->cursorPosition.x, drop->cursorPosition.y, 8);
      ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    } else {
      if (drop) {
				int removeIt = 0;
				for (auto dropToRemove : droplets){
					if (dropToRemove == drop){
						droplets.erase(droplets.begin()+removeIt);
					}
					removeIt++;
				}
      }
    }
  }
  ofPopStyle();

  gui.draw();
}

// -------

void ofApp::createDrop(){
	ofPoint		newDropPosition(ofRandom(0, ofGetWidth()),ofRandom(0, ofGetHeight()));
	float			newDropDuration(ofRandom(minTouchDir, maxTouchDir));
	ofVec2f		newDropSwipeDistance(ofRandom(-maxSwipeLengthSlider,maxSwipeLengthSlider),ofRandom(-maxSwipeLengthSlider,maxSwipeLengthSlider));
	bool			newDropDoesSwipe = random()%3;
	
	if (marchHorizontalToggle) {
		newDropPosition.x = ofGetWidth() *  (((int)ofGetElapsedTimeMillis()%(int)sweepLengthSlider)/(float)sweepLengthSlider);
	}
	if (marchVerticalToggle) {
		newDropPosition.y = ofGetHeight() * (((int)ofGetElapsedTimeMillis()%(int)sweepLengthSlider)/(float)sweepLengthSlider);
	}
	
	droplets.push_back(new RainDrop(newDropPosition,
																	newDropDuration,
																	newDropSwipeDistance,
																	newDropDoesSwipe,
																	myTuioServer));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
