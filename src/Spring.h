//
//  Spring.h
//  node_twitter_base
//
//  Created by mh on 19/04/2014.
//
//

#ifndef __node_twitter_base__Spring__
#define __node_twitter_base__Spring__

#include <iostream>

#endif /* defined(__node_twitter_base__Spring__) */

#include "ofMain.h"

class Spring {
public:
    Spring();
    ofColor color;
    ofVec2f location;
    ofVec2f velocity;
    ofVec2f acceleration;
    ofVec2f targetVector;
    
    void setColour(ofColor color);
    void seek(ofVec2f targetVector);
    void applyForce(ofVec2f force);
    void centrePull();
    void update();
    void draw();
};