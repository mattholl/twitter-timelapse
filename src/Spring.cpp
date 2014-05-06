//
//  Spring.cpp
//  node_twitter_base
//
//  Created by mh on 19/04/2014.
//
//

#include "ofMain.h"
#include "Spring.h"

Spring::Spring() {
    
    // Start in the middle
    location.x = ofGetWindowWidth() / 2;
    location.y = ofGetWindowHeight() / 2;
    location.z = 0;
    
    // Set initial values to 0
    maxForce = 0.0;
    maxSpeed = 0.0;
    restLength = 0.0;
    k = 0.0;
    
}

void Spring::setSpringProps(float maxForce, float maxSpeed, float restLength, float k) {
    this->maxForce = maxForce;
    this->maxSpeed = maxSpeed;
    this->restLength = restLength;
    this->k = k;
}

void Spring::setColour(ofColor colour) {
    this->colour = colour;
}

void Spring::seek() {
    
    ofVec3f desiredVec = location - targetVector;
    desiredVec.normalize();
    desiredVec.scale(maxSpeed);
    
    ofVec3f steer = velocity - desiredVec;
    
    if(steer.length() > maxForce) {
        steer.normalize();
        steer.scale(maxForce);
    }
    
    applyForce(steer);
}

void Spring::applyForce(ofVec3f force) {
    acceleration += force;
}

void Spring::centrePull() {
    
    // Get centre as vec2
    ofVec3f centre(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0);
    
    ofVec3f force = location - centre;
    float distance = force.length();
    
    if (distance > 0) {
        float stretch = distance - restLength;

        // Calculate force according to Hooke's Law
        // F = k * stretch
        
        force.normalize();
        force.scale(-1.0 * k * stretch);
        applyForce(force);
    }
    
}

void Spring::setTargetVec(float x, float y, float z) {
    // Set target vector from first and second as x and y
    targetVector.set(x, y, z);
}

void Spring::update() {
    
    // Call centrePull function
    centrePull();
    
    // Seek the target vector
    seek();
    
    // velocity.add acceleration
    velocity += acceleration;
    
    // Limit to maxspeed
    
    if(velocity.length() > maxSpeed) {
        velocity.normalize();
        velocity.scale(maxSpeed);
    }

    // Apply the velocity to location and scale back the acceleration for next update
    location += velocity;
    acceleration.scale(0);

}

void Spring::draw() {
    
    // set colour to this->colour
    
    ofSetColor(this->colour);
    
    int size = 1;
    
    ofVec3f centre(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0);
    
    // draw circle at the end
    ofCircle(location, size);
    
    // draw line from window centre to point
    ofLine(location, centre);
}