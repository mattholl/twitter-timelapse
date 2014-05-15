#include <iostream>

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {

    ofHideCursor();

    // don't run too fast(?);
    ofSetVerticalSync(true);

    accumulatedFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

    // Clear the background to white once
    accumulatedFBO.begin();
        ofBackground(255, 255, 255);
    accumulatedFBO.end();

    // Initial values for x, y, z
    incomingX = 0.0;
    incomingY = 0.0;
    incomingZ = 0.0;

    // Set up to listen for OSC messages
    cout << "listening for osc messages on port " << PORT << "\n";
	oscReceiver.setup(PORT);

    // Set to start at the first spring
    selectSpring = 0;

    // Load the settings file for the spring parameters
    if( XML.loadFile("settings.xml") ){
        cout << "settings.xml loaded" << endl;
    }else{
        cout << "unable to load settings.xml check data/ folder" << endl;
    }

    // Spring objects setup
    // For each spring:
    //      Parse the parameter values from the XML settings file
    //      Create a colour object
    //      Set spring parameters
    //      Push into the springs array

    // spring1.setSpringProps(float maxForce, float maxSpeed, float restLength, float k);

    Spring spring1;

    int s1Red = XML.getValue("spring-data:spring1:colour:r", 0);
    int s1Blue = XML.getValue("spring-data:spring1:colour:g", 0);
    int s1Green = XML.getValue("spring-data:spring1:colour:b", 0);
    int s1Alpha = XML.getValue("spring-data:spring1:colour:a", 255);

    float s1MaxForce = XML.getValue("spring-data:spring1:maxForce", 0.0);
    float s1MaxSpeed = XML.getValue("spring-data:spring1:maxSpeed", 0.0);
    float s1RestLength = XML.getValue("spring-data:spring1:restLength", 0.0);
    float s1K = XML.getValue("spring-data:spring1:k", 0.0);

    ofColor colour1(s1Red, s1Blue, s1Green, s1Alpha); //#00506C - dark green-blue? //10 alpha
    spring1.setColour(colour1);
    spring1.setSpringProps(s1MaxForce, s1MaxSpeed, ofGetWindowWidth() * s1RestLength, s1K);
    springs.push_back(spring1);

    Spring spring2;

    int s2Red = XML.getValue("spring-data:spring2:colour:r", 0);
    int s2Blue = XML.getValue("spring-data:spring2:colour:g", 0);
    int s2Green = XML.getValue("spring-data:spring2:colour:b", 0);
    int s2Alpha = XML.getValue("spring-data:spring2:colour:a", 255);

    float s2MaxForce = XML.getValue("spring-data:spring2:maxForce", 0.0);
    float s2MaxSpeed = XML.getValue("spring-data:spring2:maxSpeed", 0.0);
    float s2RestLength = XML.getValue("spring-data:spring2:restLength", 0.0);
    float s2K = XML.getValue("spring-data:spring2:k", 0.0);

    ofColor colour2(s2Red, s2Green, s2Blue, s2Alpha); //A9DBD9
    spring2.setColour(colour2);
    spring2.setSpringProps(s2MaxForce, s2MaxSpeed, ofGetWindowWidth() * s2RestLength, s2K);
    springs.push_back(spring2);

    Spring spring3;

    int s3Red = XML.getValue("spring-data:spring3:colour:r", 0);
    int s3Blue = XML.getValue("spring-data:spring3:colour:g", 0);
    int s3Green = XML.getValue("spring-data:spring3:colour:b", 0);
    int s3Alpha = XML.getValue("spring-data:spring3:colour:a", 255);

    float s3MaxForce = XML.getValue("spring-data:spring3:maxForce", 0.0);
    float s3MaxSpeed = XML.getValue("spring-data:spring3:maxSpeed", 0.0);
    float s3RestLength = XML.getValue("spring-data:spring3:restLength", 0.0);
    float s3K = XML.getValue("spring-data:spring3:k", 0.0);

    ofColor colour3(s3Red, s3Green, s3Blue, s3Alpha); //#008D96 // alpha 10
    spring3.colour = colour3;
    spring3.setSpringProps(s3MaxForce, s3MaxSpeed, ofGetWindowWidth() * s3RestLength, s3K);
    springs.push_back(spring3);

    // Get the frequency with images should be saved from the settings.xml file
    saveTimeout = XML.getValue("spring-data:save-image-timeout", 0.0);

    // Set the initial time to zero, this will get updated each time an image is saved
    lastSaveTime = 0;

    // Get the dir path to save, might be a network drive or local
    saveImagePath = XML.getValue("spring-data:save-image-path", "");

    // Need to check whether the image is being saved - ofxNetwork throws errors on the raspberry pi
    isSavingImage = false;
}

//--------------------------------------------------------------
void testApp::update(){

    // Update incoming values with JSON values from node.js OSC sender

//    if(!isSavingImage) {

    while(oscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
		oscReceiver.getNextMessage(&m);

        if(m.getAddress() == "/coords") {

            if(m.getArgType(0) == OFXOSC_TYPE_FLOAT) {
                incomingX = m.getArgAsFloat(0);
            }

            if(m.getArgType(1) == OFXOSC_TYPE_FLOAT) {
                incomingY = m.getArgAsFloat(1);
            }

            if(m.getArgType(2) == OFXOSC_TYPE_FLOAT) {
                incomingY = m.getArgAsFloat(2);
            }

            // Pass the incoming coords to the springs one at a time
            springs[selectSpring].setTargetVec(incomingX, incomingY, incomingZ);

            // selectSpring will move through 0, 1, 2
            selectSpring++;
            selectSpring %= springs.size();
        }
    }

    // Call update for each spring
    for(unsigned int i = 0; i < springs.size(); i++) {
        springs[i].update();
    }

    accumulatedFBO.begin();
        ofEnableAlphaBlending();
        ofEnableSmoothing();

//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        ofSetColor(20, 20, 20, 255);

        for(unsigned int i = 0; i < springs.size(); i++) {
            springs[i].draw();
        }

        ofDisableSmoothing();
        ofDisableAlphaBlending();
    accumulatedFBO.end();

}

//--------------------------------------------------------------
void testApp::draw(){

//    cout << "X : " + ofToString(incomingX, 17) << endl;
//    cout << "Y : " + ofToString(incomingY, 17) << endl;
//    cout << "Z : " + ofToString(incomingZ, 17) << endl;

    ofBackground(255, 255, 255);
    ofSetColor(255, 255, 255, 255);

    // Draw the fbo to the screen
    accumulatedFBO.draw(0,0);

    // Check whether an image should be saved

    // use threaded image saver class
    saveImage();
}

//--------------------------------------------------------------

void testApp::saveImage() {

    // Check if current elapsed time in seconds equals lastSaveTime + saveTimeout

    int elapsedTime = (int)ofGetElapsedTimef();
    int timeDiff = elapsedTime - lastSaveTime;

    if(timeDiff == saveTimeout) {

        // Stop trying to read from the network
        isSavingImage = true;

        ofImage image;
        image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());

        // Save the image to the dir from XML settings, path should end in /
        string path = saveImagePath;

        image.saveImage(path + "screengrab_" + ofToString(ofGetUnixTime()) + ".png");
        cout << "Image saved : " + "screengrab_" + ofToString(ofGetUnixTime()) + ".png" << endl;

        // Clear the background after the image saves
        ofBackground(255, 255, 255);

        // Clear the fbo
        accumulatedFBO.begin();
            ofClear(255,255,255, 0);
        accumulatedFBO.end();

        lastSaveTime = elapsedTime;

        // Allow network communication again
        isSavingImage = false;
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
