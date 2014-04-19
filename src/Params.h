//
//  Params.h
//  node_twitter_base
//
//  Created by mh on 19/04/2014.
//
//

#ifndef __node_twitter_base__Params__
#define __node_twitter_base__Params__

#include <iostream>

#endif /* defined(__node_twitter_base__Params__) */

class Params {
public:
    void setup();
    float maxForce;
    float maxSpeed;
    float restLength;   // canvasWidth * 0.14; // Math.sqrt(canvasWidth) * 6; //200 at 1440
    float k;            // Spring restitution
};

extern Params param;