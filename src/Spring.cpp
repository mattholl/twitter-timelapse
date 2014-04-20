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
    // Any setup
}
// param.something

void Spring::setColour(ofColor color) {
    this->color = color;
}

void Spring::seek() {
    
    ofVec2f desiredVec = this->location - this->targetVector;
    desiredVec.normalize();
    desiredVec.scale(param.maxSpeed);
    
    ofVec2f steer = this->velocity - desiredVec;
    
    if(steer.length() > param.maxForce) {
        steer.normalize();
        steer.scale(param.maxForce);
    }
    
    this->applyForce(steer);
}

void Spring::applyForce(ofVec2f force) {
    this->acceleration += force;
}

void Spring::centrePull() {
    
    // Get centre as vec2
    ofVec2f centre(ofGetWindowWidth(), ofGetWindowHeight());
    
    ofVec2f force = location - centre;
    float distance = force.length();
    
    if (distance > 0) {
        float stretch = distance - param.restLength;

        // Calculate force according to Hooke's Law
        // F = k * stretch
        
        force.normalize();
        force.scale(-1.0 * param.k * stretch);
        this->applyForce(force);
    }
    
}

void Spring::update(float first, float second) {
    // Set target vector from first and second as x and y
    this->targetVector.set(first, second);
    
    // Call centrePull function
    this->centrePull();
    
    // Seek the target vector
    this->seek();
    
    // velocity.add acceleration
    this->velocity += this->acceleration;
    
    // Limit to maxspeed
    
    if(this->velocity.length() > param.maxSpeed) {
        this->velocity.normalize();
        this->velocity.scale(param.maxSpeed);
    }

    // Apply the velocity to location and scale back the acceleration for next update
    this->location += this->velocity;
    this->acceleration.scale(0);

}

void Spring::draw() {
    
    // set colour to this->colour
    
    ofColor color = ofColor::red;
    ofSetColor(color);
    
    int size = 5;
    
    // for test
//    location.set(100.0, 100.0);
    
    // draw circle at the end
    ofCircle(this->location, size);
    
    
    // draw line from window centre to point

    
}