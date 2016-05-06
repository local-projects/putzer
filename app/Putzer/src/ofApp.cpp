//
//  ofApp.cpp
//  Putzer
//
//  Created by Eric Rieper on 3/31/16.
//
//

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{

	// OF Setup
	ofBackground(200);
	ofSetFrameRate(60);

	// Window Setup
	ofSetWindowTitle("Putzer!");

	// Gui Setup
	setupGui();

	// TUIO Setup
	myTuioServer = new ofxTuioServer;

	string str = *ipAddressLabel;
	char *cstr = &str[0u];

	myTuioServer->start(cstr, ofToInt(*portLabel));
	myTuioServer->setVerbose(false);

	// Scale window to fit GUI
	ofSetWindowShape((gui->getHeight() + (gui->getPosition().y * 2)) * 2, gui->getHeight() + (gui->getPosition().y * 2));

	touchRegionRect.set(0, 0, 1, 1);
}

//--------------------------------------------------------------
void ofApp::setupGui()
{
	// Arduous setup to actually style ofxGui...

	// GUI Setup
	gui = new ofxPanel();
	gui->setup();

	ofColor guiColor(40, 40, 40);

	gui->setHeaderBackgroundColor(guiColor);
	gui->setBackgroundColor(guiColor);
	gui->setBorderColor(guiColor);

	gui->setDefaultHeaderBackgroundColor(guiColor);
	gui->setDefaultBackgroundColor(guiColor);
	gui->setDefaultBorderColor(guiColor);

	infoGroup = new ofxGuiGroup();
	touchesGroup = new ofxGuiGroup();
	motionGroup = new ofxGuiGroup();

	touchFrequency = new ofxIntSlider();
	numTouches = new ofxIntSlider();
	resetRegionButton = new ofxButton();
	resetRegionButton->addListener(this, &ofApp::buttonPressed);
	swipeLikelyhood = new ofxIntSlider();
	maxSwipeLengthSlider = new ofxFloatSlider();
	horizontalSwipes = new ofxToggle();
	verticalSwipes = new ofxToggle();
	minTouchDir = new ofxFloatSlider();
	maxTouchDir = new ofxFloatSlider();
	putzerEnabledToggle = new ofxToggle();
	marchHorizontalToggle = new ofxToggle();
	marchVerticalToggle = new ofxToggle();
	horizontalSweepSpeed = new ofxFloatSlider();
	verticalSweepSpeed = new ofxFloatSlider();
	sineHorizontalToggle = new ofxToggle();
	sineVerticalToggle = new ofxToggle();
	ipAddressLabel = new ofxLabel();
	portLabel = new ofxLabel();
	touchCountLabel = new ofxLabel();

	// General info group
	gui->add(infoGroup->setup("Info"));
	infoGroup->add(putzerEnabledToggle->setup("Putzer Enabled", false));
	infoGroup->add(ipAddressLabel->setup("IP", "127.0.0.1"));
	infoGroup->add(portLabel->setup("PORT", "3333"));
	infoGroup->add(touchCountLabel->setup("CURRENT TOUCHES", "0"));
	infoGroup->add(resetRegionButton->setup("RESET TOUCH REGION"));

	// Touch properties group
	touchFrequency = new ofxIntSlider();
	gui->add(touchesGroup->setup("Touches"));
	touchesGroup->add(touchFrequency->setup("Frames Between Touches", 5, 1, 120));
	touchesGroup->add(numTouches->setup("Number of Touches Added", 2, 1, 10));
	touchesGroup->add(minTouchDir->setup("Min Touch Duration", 0.05, 0.01, 4));
	touchesGroup->add(maxTouchDir->setup("Max Touch Duration", 1.0, 0.02, 5));
	touchesGroup->add(swipeLikelyhood->setup("Swipe Likleyhood", 10, 0, 100));
	touchesGroup->add(maxSwipeLengthSlider->setup("Max Swipe Speed", 5, 0.0, 25.0));
	touchesGroup->add(horizontalSwipes->setup("Horizontal Swipes", true));
	touchesGroup->add(verticalSwipes->setup("Vertical Swipes", true));

	// Touch area motion settings
	motionGroup = new ofxGuiGroup();
	gui->add(motionGroup->setup("Area Motion"));
	motionGroup->add(marchHorizontalToggle->setup("Sweep X", false));
	motionGroup->add(marchVerticalToggle->setup("Sweep Y", false));
	motionGroup->add(sineHorizontalToggle->setup("Oscillate X", false));
	motionGroup->add(sineVerticalToggle->setup("Oscillate Y", false));
	motionGroup->add(horizontalSweepSpeed->setup("X Motion Duration", 1, 0.1, 20));
	motionGroup->add(verticalSweepSpeed->setup("Y Motion Duration", 2, 0.1, 20));

	gui->loadFromFile("settings.xml");
}

//--------------------------------------------------------------

void ofApp::update()
{

	// CREATE NEW TOUCHES
	if (*putzerEnabledToggle && fakeTouches.size() < 256) { // Limits total number of touches to
		if (ofGetFrameNum() % *touchFrequency == 0) {					// prevent buffer out of memory error.

			int cappedNumTouches = *numTouches;
			bool hasFixedPosition = false;
			if ((*marchVerticalToggle || *sineVerticalToggle) && (*marchHorizontalToggle || *sineHorizontalToggle)) hasFixedPosition = true;
			if (hasFixedPosition) { // Only allow one touch because we have
				cappedNumTouches = 1; // a  fixed X and Y position.
			}

			for (int i = 0; i < cappedNumTouches; i++) { // Create the touches!
				createtouch();
			}
		}
	}

	// REMOVE OLD TOUCHES
	for (auto touch : fakeTouches) {
		if (!touch->cursorIsActive) {
			if (touch) {
				int removeIt = 0;
				for (auto touchToRemove : fakeTouches) {
					if (touchToRemove == touch) {
						delete fakeTouches[removeIt];
						fakeTouches.erase(fakeTouches.begin() + removeIt);
					}
					removeIt++;
				}
			}
		}
	}

	// UPDATE THE TUIO SERVER
	myTuioServer->run();

	// UPDATE EACH TOUCH
	for (auto touch : fakeTouches) {
		touch->update();
	}

	// UPDATE THE GUI
	if (ofGetFrameNum() % 15 == 0) {
		if (fakeTouches.size() >= 256) {
			touchCountLabel->setBackgroundColor(ofColor(100, 10, 40));
		}
		else {
			touchCountLabel->setBackgroundColor(gui->getBackgroundColor());
		}
		*touchCountLabel = ofToString(fakeTouches.size());
	}
}

//--------------------------------------------------------------

void ofApp::draw()
{

	// DRAW THE BACKGROUND ////////////////////
	ofBackgroundGradient(ofColor(24), ofColor(4), OF_GRADIENT_CIRCULAR);

	// DRAW THE TOUCHES
	for (auto touch : fakeTouches) {
		if (touch->cursorIsActive) {
			touch->draw();
		}
	}

	// DRAW TOUCH REGION RECTANGLE ////////////
	ofPushStyle();
	ofNoFill();
	ofSetColor(220);
	ofRectangle drawnRect(touchRegionRect);
	drawnRect.x *= ofGetWidth();
	drawnRect.y *= ofGetHeight();
	drawnRect.width *= ofGetWidth();
	drawnRect.height *= ofGetHeight();
	ofDrawRectangle(drawnRect);

	// DRAW REGION COORDINATES
	ofDrawBitmapString(ofToString(rounder(touchRegionRect.getTopLeft().x, 100)) + " , " + ofToString(rounder(touchRegionRect.getTopLeft().y, 100)), drawnRect.getTopLeft());
	ofDrawBitmapString(ofToString(rounder(touchRegionRect.getBottomRight().x, 100)) + " , " + ofToString(rounder(touchRegionRect.getBottomRight().y, 100)), drawnRect.getBottomRight());

	ofFill();
	ofPopStyle();

	// DRAW THE GUI ///////////////////////////
	gui->draw();

	// DRAW A FADE IN ON STARTUP //////////////
	if (shouldDrawFadeIn) {
		ofPushStyle();
		ofColor fadeColor = ofColor(200);
		fadeColor.a = 255 - ofClamp(ofMap(fadeInCounter, 0, 30, 0, 255), 0, 255);
		ofSetColor(fadeColor);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		ofPopStyle();
		fadeInCounter++;
		if (fadeInCounter > 30) shouldDrawFadeIn = false;
	}
}

//--------------------------------------------------------------

void ofApp::createtouch()
{

	// Note: ofxTuioServer::addCursor() takes screen coordinates.

	// CREATE POSITION, DURATION, AND SWIPE PROPERTIES
	ofPoint newTouchPosition(ofRandom(touchRegionRect.getLeft() * ofGetWidth(), touchRegionRect.getRight() * ofGetWidth()), ofRandom(touchRegionRect.getTop() * ofGetHeight(), touchRegionRect.getBottom() * ofGetHeight()));
	float newTouchDuration(ofRandom(*minTouchDir, *maxTouchDir));

	int whichDir = rand() % 3;
	ofVec2f newTouchSwipeDistance;

	if (whichDir == 2 && *horizontalSwipes == true) { // horizontal
		newTouchSwipeDistance.set(ofRandom(-*maxSwipeLengthSlider, *maxSwipeLengthSlider), 0);
	}
	else if (whichDir == 1 && *verticalSwipes == true) { // vertical
		newTouchSwipeDistance.set(0, ofRandom(-*maxSwipeLengthSlider, *maxSwipeLengthSlider));
	}
	else { // random
		newTouchSwipeDistance.set(ofRandom(-*maxSwipeLengthSlider, *maxSwipeLengthSlider), ofRandom(-*maxSwipeLengthSlider, *maxSwipeLengthSlider));
	}

	bool newTouchDoesSwipe = false;

	if (*swipeLikelyhood == 0) {
		newTouchDoesSwipe = false;
	}
	else if (*swipeLikelyhood == 100) {
		newTouchDoesSwipe = true;
	}
	else {
		newTouchDoesSwipe = (bool)(rand() % *swipeLikelyhood);
	}

	// MODIFY POSITION BASED ON SWEEPING OR OSCILLATING
	if (*marchHorizontalToggle) {
		float hMillis = *horizontalSweepSpeed * 1000;
		newTouchPosition.x = (touchRegionRect.getLeft() * ofGetWidth()) + ((touchRegionRect.getWidth() * ofGetWidth()) * ((ofGetElapsedTimeMillis() % (int)hMillis / hMillis)));
	}

	if (*marchVerticalToggle) {
		float vMillis = *verticalSweepSpeed * 1000;
		newTouchPosition.y = (touchRegionRect.getTop() * ofGetHeight()) + ((touchRegionRect.getHeight() * ofGetHeight()) * ((ofGetElapsedTimeMillis() % (int)vMillis / vMillis)));
	}

	if (*sineHorizontalToggle) {
		newTouchPosition.x = (touchRegionRect.getLeft() * ofGetWidth()) + ((touchRegionRect.getWidth() * ofGetWidth()) * (0.5 + (sinf(ofGetElapsedTimef() / ((float)*horizontalSweepSpeed * 0.5)) * 0.5)));
	}
	if (*sineVerticalToggle) {
		newTouchPosition.y = (touchRegionRect.getTop() * ofGetHeight()) + ((touchRegionRect.getHeight() * ofGetHeight()) * (0.5 + (cosf(ofGetElapsedTimef() / ((float)*verticalSweepSpeed * 0.5)) * 0.5)));
	}

	// CREATE AND PUSH touch INTO fakeTouches
	fakeTouches.push_back(new FakeTouch(newTouchPosition, newTouchDuration, newTouchSwipeDistance, newTouchDoesSwipe, myTuioServer));
}

double ofApp::rounder(float _in, int _precision) { return round(_in * _precision) / (float)_precision; }

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
	if (button == OF_MOUSE_BUTTON_LEFT && isDraggingRect) {
		ofPoint mappedPoint(x / (float)ofGetWidth(), y / (float)ofGetHeight());
		touchRegionRect.set(mappedPoint, oldBottomRight);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	if (button == OF_MOUSE_BUTTON_LEFT && !gui->getShape().inside(x, y)) {

		isDraggingRect = true;

		ofPoint mappedPoint(x / (float)ofGetWidth(), y / (float)ofGetHeight());
		touchRegionRect.set(mappedPoint, mappedPoint);

		oldTopLeft = touchRegionRect.getTopLeft();
		oldBottomRight = touchRegionRect.getBottomRight();
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
	oldTopLeft = touchRegionRect.getTopLeft();
	oldBottomRight = touchRegionRect.getTopRight();
	isDraggingRect = false;
}

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

//--------------------------------------------------------------
void ofApp::buttonPressed(const void *sender)
{
	if (sender == resetRegionButton) {
		touchRegionRect.set(0, 0, 1, 1);
	}
}
