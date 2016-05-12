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

	// OF Setup ///////////////////////////////
	ofBackground(200);
	ofSetFrameRate(60);
	
	// Gui Setup //////////////////////////////
	setupGui();
	
	// Window Setup ///////////////////////////
	ofSetWindowShape((gui->getHeight() + (gui->getPosition().y * 2)) * 2, gui->getHeight() + (gui->getPosition().y * 2));
	ofSetWindowTitle("Putzer!");

	// TUIO Setup /////////////////////////////
	myTuioServer = new ofxTuioServer;

	string str = *ipAddressLabel;
	char *cstr = &str[0u];

	myTuioServer->start(cstr, ofToInt(*portLabel));
	myTuioServer->setVerbose(false);

	// Set initial full touch region //////////
	resetTouchRegion();
	
}

//--------------------------------------------------------------
void ofApp::setupGui()
{
	// Arduous setup to actually style ofxGui...

	// GUI Setup
	shadowImage = new ofImage;
	shadowImage->load("shadowLeft.png");
	
	gui = new ofxPanel();
	gui->setup();

	ofColor guiColor(40, 40, 40);
	
	gui->setPosition(10,10);

	gui->setHeaderBackgroundColor(guiColor);
	gui->setBackgroundColor(guiColor);
	gui->setBorderColor(guiColor);

	gui->setDefaultHeaderBackgroundColor(guiColor);
	gui->setDefaultBackgroundColor(guiColor);
	gui->setDefaultBorderColor(guiColor);
	gui->setDefaultFillColor(ofColor::fromHex(0x4f7480));
	gui->setDefaultTextColor(210);
	gui->setDefaultWidth(200);
	gui->setDefaultTextPadding(10);
	gui->setUseTTF(true);
	gui->loadFont("Montserrat-Regular.ttf", 8, true, false);
	gui->setDefaultHeight(20);
//	gui->setDefaultTextPadding(10);

	infoGroup = new ofxGuiGroup();
	touchesGroup = new ofxGuiGroup();
	motionGroup = new ofxGuiGroup();

	touchFrequency = new ofxIntSlider();
	numTouches = new ofxIntSlider();
	resetRegionButton = new ofxButton();
	resetRegionButton->addListener(this, &ofApp::buttonPressed);
	swipeLikelihood = new ofxIntSlider();
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
	touchesGroup->add(maxTouchDir->setup("Max Touch Duration", 0.5, 0.02, 5));
	touchesGroup->add(swipeLikelihood->setup("Swipe Likelihood", 45, 0, 100));
	touchesGroup->add(maxSwipeLengthSlider->setup("Max Swipe Speed", 0.01, 0.0, 0.1));
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
	motionGroup->add(verticalSweepSpeed->setup("Y Motion Duration", 3.5, 0.1, 20));

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
				if (fakeTouches.size() < 256) {
					createtouch();
				}
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
	ofBackgroundGradient(ofColor(30), ofColor(20), OF_GRADIENT_CIRCULAR);

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
	ofPopStyle();

	// DRAW THE GUI ///////////////////////////
	shadowImage->draw(GUI_WIDTH,0,29,ofGetHeight());
	ofPushStyle();
	ofSetColor(40);
	ofDrawRectangle(0,0,GUI_WIDTH,ofGetHeight());
	ofPopStyle();
	gui->setPosition(10,10);
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

	// Note: Using normalized coordinates (0-1)

	// CREATE POSITION, DURATION, AND SWIPE PROPERTIES
	ofPoint newTouchPosition(ofRandom(touchRegionRect.getLeft(), touchRegionRect.getRight()),
													 ofRandom(touchRegionRect.getTop(), touchRegionRect.getBottom()));
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

	if (*swipeLikelihood == 0) {
		newTouchDoesSwipe = false;
	}
	else if (*swipeLikelihood == 100) {
		newTouchDoesSwipe = true;
	}
	else {
		if (rand() % 100 < *swipeLikelihood){
			newTouchDoesSwipe = true;
		}
	}

	// MODIFY POSITION BASED ON SWEEPING OR OSCILLATING
	if (*marchHorizontalToggle) {
		float hMillis = *horizontalSweepSpeed * 1000;
		newTouchPosition.x = (touchRegionRect.getLeft()) + ((touchRegionRect.getWidth()) * ((ofGetElapsedTimeMillis() % (int)hMillis / hMillis)));
	}

	if (*marchVerticalToggle) {
		float vMillis = *verticalSweepSpeed * 1000;
		newTouchPosition.y = (touchRegionRect.getTop()) + ((touchRegionRect.getHeight()) * ((ofGetElapsedTimeMillis() % (int)vMillis / vMillis)));
	}

	if (*sineHorizontalToggle) {
		newTouchPosition.x = (touchRegionRect.getLeft()) + ((touchRegionRect.getWidth()) * (0.5 + (sinf(ofGetElapsedTimef() / ((float)*horizontalSweepSpeed * 0.5)) * 0.5)));
	}
	if (*sineVerticalToggle) {
		newTouchPosition.y = (touchRegionRect.getTop()) + ((touchRegionRect.getHeight()) * (0.5 + (cosf(ofGetElapsedTimef() / ((float)*verticalSweepSpeed * 0.5)) * 0.5)));
	}
	
	// OFFSET FOR GUI
	newTouchPosition.x = ofMap(newTouchPosition.x, (float)GUI_WIDTH / (float)ofGetWidth(), 1, 0, 1);

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
		// Clamp for GUI offset
		x = ofClamp(x,GUI_WIDTH, ofGetWidth());
		y = ofClamp(y,0,ofGetHeight());
		ofPoint mappedPoint(x / (float)ofGetWidth(), y / (float)ofGetHeight());
		touchRegionRect.set(mappedPoint, oldBottomRight);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	if (button == OF_MOUSE_BUTTON_LEFT && x > GUI_WIDTH + 2) {

		isDraggingRect = true;
		
		// Clamp for GUI offset
		x = ofClamp(x,GUI_WIDTH, ofGetWidth());
		y = ofClamp(y,0,ofGetHeight());

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
		resetTouchRegion();
	}
}

//--------------------------------------------------------------
void ofApp::resetTouchRegion(){
		touchRegionRect.set(GUI_WIDTH / (float)ofGetWidth(), 0, 1 - GUI_WIDTH / (float)ofGetWidth(), 1);
}
