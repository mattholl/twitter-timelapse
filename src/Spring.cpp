//
//  Spring.cpp
//  node_twitter_base
//
//  Created by mh on 19/04/2014.
//
//

#include "ofMain.h"
#include "Spring.h"
#include "Params.h"

Spring::Spring() {
    location.x = ofGetWindowWidth() / 2;
    location.y = ofGetWindowHeight() / 2;
}

void Spring::setColour(ofColor colour) {
    this->colour = colour;
}

void Spring::seek() {
    
    ofVec2f desiredVec = location - targetVector;
    desiredVec.normalize();
    desiredVec.scale(param.maxSpeed);
    
    ofVec2f steer = velocity - desiredVec;
    
    if(steer.length() > param.maxForce) {
        steer.normalize();
        steer.scale(param.maxForce);
    }
    
    applyForce(steer);
}

void Spring::applyForce(ofVec2f force) {
    acceleration += force;
}

void Spring::centrePull() {
    
    // Get centre as vec2
    ofVec2f centre(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
    
    ofVec2f force = location - centre;
    float distance = force.length();
    
    if (distance > 0) {
        float stretch = distance - param.restLength;

        // Calculate force according to Hooke's Law
        // F = k * stretch
        
        force.normalize();
        force.scale(-1.0 * param.k * stretch);
        applyForce(force);
    }
    
}

void Spring::setTargetVec(float first, float second) {
    // Set target vector from first and second as x and y
    targetVector.set(first, second);
}

void Spring::update() {
    
    // Call centrePull function
    centrePull();
    
    // Seek the target vector
    seek();
    
    // velocity.add acceleration
    velocity += acceleration;
    
    // Limit to maxspeed
    
    if(velocity.length() > param.maxSpeed) {
        velocity.normalize();
        velocity.scale(param.maxSpeed);
    }

    // Apply the velocity to location and scale back the acceleration for next update
    location += velocity;
    acceleration.scale(0);

}

void Spring::draw() {
    
    // set colour to this->colour
    
    ofSetColor(this->colour);
    
    int size = 5;
    
    ofVec2f centre(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
    
    // draw circle at the end
    ofCircle(location, size);
    ofLine(location, centre);
    
    // draw line from window centre to point

    
}