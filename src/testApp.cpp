#include "testApp.h"

#define RECONNECT_TIME = 400;

//--------------------------------------------------------------
void testApp::setup(){
    
    // don't run too fast(?);
    ofSetVerticalSync(true);
    
    // something typed
    typed = false;
    // typing position
    pos = 0;
    
    // send and receive strings
    msgTx = "";
    msgRx = "";
    
    // try to connect if it fails retry every few seconds
    weConnected = tcpClient.setup("127.0.0.1", 3001);
    tcpClient.setMessageDelimiter("\n");
    
    connectTime = 0;
    deltaTime = 0;
    
    tcpClient.setVerbose(true);
}

//--------------------------------------------------------------
void testApp::update(){
    
    ofBackground(230, 230, 230);
    
    if (weConnected) {
        if(tcpClient.send(msgTx)) {
            
//            cout << ofToString(str) << endl;
            
            string str = tcpClient.receive();
            
            if(str.length() > 0) {
                
                msgRx = str;
            }
        } else if(!tcpClient.isConnected()) {
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
    
    if(typed) {
        ofDrawBitmapString("sending: ", 15, 55);
        ofDrawBitmapString(msgTx, 85, 55);
        
    } else {
        if (weConnected) {
            ofDrawBitmapString("status: type something to send to port 3001", 15, 55);
        } else {
            ofDrawBitmapString("status: server not found\n\nReconnecting in " + ofToString((5000 - deltaTime) / 1000) + " seconds.", 15, 55);
            
        }
    
    }
    
    ofDrawBitmapString("From server: " + msgRx, 15, 270);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if(weConnected) {
        
        if(key == 13) {
            key = '\n';
        }
        
        if(key == 8 || key == 127) {
            if(pos != 0) {
                pos--;
                msgTx = msgTx.substr(0, pos);
            } else {
                msgTx = "";
            }
        } else {
            msgTx.append(1, (char) key);
            pos++;
        }
        
        typed = true;
    }
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
