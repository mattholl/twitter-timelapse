#include <iostream>

#include "testApp.h"

#define RECONNECT_TIME = 400;

//--------------------------------------------------------------
void testApp::setup(){
    
    // don't run too fast(?);
    ofSetVerticalSync(true);
    
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
    
    // each one needs a different colour
    param.setup();
    
    cout << "REST LENGTH: " + ofToString(param.restLength) << endl;
    cout << "MAX FORCE: " + ofToString(param.maxForce) << endl;
    
    // Spring objects setup
}

//--------------------------------------------------------------
void testApp::update(){
    
    ofBackground(230, 230, 230);
    
    // Update incoming values with JSON values from node.js TCP server
    
    if (weConnected) {
        if(tcpClient.isConnected()) {
            string str = tcpClient.receive();
            
            if(str.length() > 0) {
                
                bool parsed = geoData.parse(str);
                
                if (parsed) {
                    incomingX = geoData["x"].asDouble();
                    incomingY = geoData["y"].asDouble();
                    incomingZ = geoData["z"].asDouble();
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
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofSetColor(20, 20, 20);
    ofDrawBitmapString("openFrameworks TCP send example", 15, 30);
    
    if (!weConnected) {
        ofDrawBitmapString("status: server not found\n\nReconnecting in " + ofToString((5000 - deltaTime) / 1000) + " seconds.", 15, 55);
    }
    
    ofDrawBitmapString("From server X: " + ofToString(incomingX, 17), 15, 270);
    ofDrawBitmapString("From server Y: " + ofToString(incomingY, 17), 15, 290);
    ofDrawBitmapString("From server Z: " + ofToString(incomingZ, 17), 15, 310);
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
