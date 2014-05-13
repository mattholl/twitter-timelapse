#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "Spring.h"
#include "ofxOsc.h"

#define PORT 3001

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
    
    // OSC Receiver
    ofxOscReceiver oscReceiver;
    
    // Used to pass each incoming coords to a different spring and cycle through them
    int selectSpring;
    
    // Incoming JSON parse
    double incomingX, incomingY, incomingZ;
    
    // Set up spring objects
    vector<Spring> springs;
    
    // Load the settings from bin/data/settings.xml
    ofxXmlSettings XML;
    
    // Periodic image saving
    int saveTimeout;        // Read this from settings.xml
    int lastSaveTime;       // Update each time an image is saved
    void saveImage();
    
    // Get dir where the image should be saved
    string saveImagePath;
    
    // FBO for accumulated drawing ofSetBackgroundAuto(false); doesn't work on the raspberry pi
    ofFbo accumulatedFBO;
    
    // Prevent errors from ofxNetwork
    bool isSavingImage;
};
