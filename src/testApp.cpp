#include <iostream>

#include "testApp.h"

#define RECONNECT_TIME = 400;

//--------------------------------------------------------------
void testApp::setup() {
    
    // don't run too fast(?);
    ofSetVerticalSync(true);
    
    ofEnableSmoothing();
    
    // Disable automatic background clearing
    ofSetBackgroundAuto(false);
    
    // Clear the background to white once
    ofBackground(255, 255, 255);
    
    // TCP Connection
    // Initial values for x, y, z
    incomingX = 0.0;
    incomingY = 0.0;
    incomingZ = 0.0;
    
    // try to connect if it fails retry every few seconds
    weConnected = tcpClient.setup("127.0.0.1", 3001);
    tcpClient.setMessageDelimiter("\n");
    
    connectTime = 0;
    deltaTime = 0;
    
    tcpClient.setVerbose(true);
    
    tcpClient.send("client connected");
    
    // Spring objects setup
    // Set colour
    // Set spring parameters
    
    // spring1.setSpringProps(float maxForce, float maxSpeed, float restLength, float k);
    
    Spring spring1;
    ofColor colour1(0, 80, 108, 10); //#00506C - dark green-blue? //10 alpha
    spring1.setColour(colour1);
    spring1.setSpringProps(0.5, 1.0, ofGetWindowWidth() * 0.24, 0.009);
    springs.push_back(spring1);
    
    Spring spring2;
    ofColor colour2(161, 219, 217, 10); //A9DBD9
    spring2.setColour(colour2);
    spring1.setSpringProps(0.5, 1.0, ofGetWindowWidth() * 0.24, 0.009);
    springs.push_back(spring2);
    
    Spring spring3;
    ofColor colour3(0, 141, 150, 10); //#008D96 // alpha 10
    spring3.colour = colour3;
    spring1.setSpringProps(0.5, 1.0, ofGetWindowWidth() * 0.24, 0.009);
    springs.push_back(spring3);
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    // Update incoming values with JSON values from node.js TCP server
    
    if (weConnected) {
        if(tcpClient.isConnected()) {
            string str = tcpClient.receive();
            
            if(str.length() > 0) {
                
                bool parsed = geoData.parse(str);
                
                if (parsed) {
                    incomingX = geoData["x"].asFloat();
                    incomingY = geoData["y"].asFloat();
                    incomingZ = geoData["z"].asFloat();
                    
                    // pass incoming in to springs [1] [2] [3]
                    springs[0].setTargetVec(incomingX, incomingY);
                    springs[1].setTargetVec(incomingY, incomingZ);
                    springs[2].setTargetVec(incomingX, incomingZ);
                    
                }
            }
            
        } else {
            weConnected = false;
        }
        
    } else {
        deltaTime = ofGetElapsedTimeMillis() - connectTime;
        
        if(deltaTime > 5000) {
            weConnected = tcpClient.setup("127.0.0.1", 3001);
            connectTime = ofGetElapsedTimeMillis();
        }
    }
    
    // Call update for each spring
    for(int i = 0; i < springs.size(); i++) {
        springs[i].update();
    }

    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofEnableAlphaBlending();
    ofSetColor(20, 20, 20);

    if (!weConnected) {
        cout << "status: server not found" << endl;
    }
    
//    cout << "X : " + ofToString(incomingX, 17) << endl;
//    cout << "Y : " + ofToString(incomingY, 17) << endl;
//    cout << "Z : " + ofToString(incomingZ, 17) << endl;
    
    ofFill();
    for(int i = 0; i < springs.size(); i++) {
        springs[i].draw();
    }
    
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    // @TODO
    // for space save image
    if(key == 's') {
        ofImage image;
        image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        image.saveImage("frame_" + ofToString(ofGetFrameNum()) + "_high.png");
    }
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
