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
    
    ofVec3f location;
    ofVec3f velocity;
    ofVec3f acceleration;
    ofVec3f targetVector;
    
    void setSpringProps(float maxForce, float maxSpeed, float restLength, float k);
    void setColour(ofColor color);
    void seek();
    void applyForce(ofVec3f force);
    void centrePull();
    void setTargetVec(float x, float y, float z);
    void update();
    void draw();
};