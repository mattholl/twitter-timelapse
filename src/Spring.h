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
    ofColor colour;
    
    // Spring params
    float maxForce;
    float maxSpeed;
    float restLength;
    float k;
    
    ofVec2f location;
    ofVec2f velocity;
    ofVec2f acceleration;
    ofVec2f targetVector;
    
    void setSpringProps(float maxForce, float maxSpeed, float restLength, float k);
    void setColour(ofColor color);
    void seek();
    void applyForce(ofVec2f force);
    void centrePull();
    void setTargetVec(float first, float second);
    void update();
    void draw();
};