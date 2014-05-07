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
                    
                    // Pass the incoming coords to the springs one at a time
                    springs[selectSpring].setTargetVec(incomingX, incomingY, incomingZ);
                    
                    // selectSpring will move through 0, 1, 2
                    selectSpring++;
                    selectSpring %= springs.size();
                    
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
        image.saveImage("frame_" + ofToString(ofGetUnixTime()) + "_high.png");
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
