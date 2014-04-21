//
//  Params.cpp
//  node_twitter_base
//
//  Created by mh on 19/04/2014.
//
//

#include "Params.h"
#include "ofMain.h"

Params param;

void Params::setup() {
    maxForce = 0.5; //2.4; // 0.9
    maxSpeed = 1.0; // 3.0;
    restLength = ofGetWindowWidth() * 0.24; // ofGetWindowWidth() * 0.14;
    k = 0.009; //0.009;
}