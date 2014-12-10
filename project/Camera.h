//
//  Camera.h
//  Project2
//
//  Created by Bhagya Sharma on 10/20/14.
//  Copyright (c) 2014 Bhagya Sharma. All rights reserved.
//

#ifndef __Project2__Camera__
#define __Project2__Camera__

#include <stdio.h>
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

class Camera{
private:
    Vector3 e;
    Vector3 d;
    Vector3 up;
    Matrix4 cameraMatrix;
    Matrix4 rotationMatrix;
    Matrix4 translationMatrix;
    Vector3 forward;
    Vector3 backward;
    Vector3 right;
    Vector3 left;
    Vector3 upAxis;
    Vector3 down;
    void updateAxisVectors();
    
public:
    Camera();
    Camera(Vector3 eVector, Vector3 dVector, Vector3 upVector);
    Matrix4 getGLMatrix();
    void lookAt(int rotation, int direction);
    void moveForward();
    void moveBackward();
    Vector3 getPos();
};
#endif /* defined(__Project2__Camera__) */
