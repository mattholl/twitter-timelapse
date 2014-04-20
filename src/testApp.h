#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxJSONElement.h"
#include "Params.h"
#include "Spring.h"

class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    // TCP Client
    ofxTCPClient tcpClient;
    
    ofTrueTypeFont mono;
    ofTrueTypeFont monosm;
    
    int connectTime;
    int deltaTime;
    bool weConnected;
    
    // Incoming JSON parse
    double incomingX, incomingY, incomingZ;
    ofxJSONElement geoData;
    
    // Set up spring objects
    vector<Spring> springs;
};
