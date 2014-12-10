//
//  Camera.cpp
//  Project2
//
//  Created by Bhagya Sharma on 10/20/14.
//  Copyright (c) 2014 Bhagya Sharma. All rights reserved.
//

#include "Camera.h"
Camera::Camera() {
    e = Vector3();
    d = Vector3();
    up = Vector3();
}

Camera::Camera(Vector3 eVector, Vector3 dVector, Vector3 upVector) {
    initialEVector = e = eVector;
    initialDVector = d = dVector;
    initialUpVector = up = upVector;
    updateAxisVectors();
}

Matrix4 Camera::getGLMatrix() {
    //z-axis
    Vector3 z = e - d;
    z.normalize();
    
    //xaxis
    Vector3 x;
    x = x.cross(up, z);
    x.normalize();
    
    //y-axis
    Vector3 y;
    y = y.cross(z, x);
    
    Vector3 v = Vector3(0,0,0);
    rotationMatrix = Matrix4::create(x, y, z, v);
    rotationMatrix = Matrix4::transpose(rotationMatrix); //inverse rotation
    
    translationMatrix = Matrix4::translate(-1 * e.getX(), -1 * e.getY(), e.getZ()*-1); //inverse translation
    
    cameraMatrix = rotationMatrix * translationMatrix;
    //cameraMatrix.transpose(); Dexter does this in his Scene Graph
    return cameraMatrix;
}

void Camera::updateAxisVectors() {
    forward = d-e;
    forward.normalize();
    
    Vector3 copy = forward;
    copy.scale(-1.0);
    backward = copy;
    
    right = right.cross(forward, up);
    right.normalize();
    
    copy = right;
    copy.scale(-1.0);
    left = copy;
    
    upAxis = upAxis.cross(right, forward);
    upAxis.normalize();
    
    copy = upAxis;
    copy.scale(-1.0);
    down = copy;
}

void Camera::lookAt(int rotation, int direction) {
    Vector3 lookAt = d - e;
    Matrix4 M;
    double angle;
    Vector3 dir;
    if(rotation) {
        angle = 10;
    }
    else {
        angle = -10;
    }
    
    if(direction) {
        dir = right;
    }
    else {
        dir = upAxis;
    }
    M = Matrix4::rotate(angle, dir);
    Vector4 lA = Vector4(lookAt.getX(), lookAt.getY(), lookAt.getZ(), 1);
    Vector4 temp = M * lA;
    Vector3 newLookAt = temp.getVector3();
    d = e + newLookAt;
    updateAxisVectors();
}
void Camera::moveForward() {
    Vector3 copy = forward;
    copy.scale(1.5);
    e = e + copy;
    d = d + copy;
    updateAxisVectors();
}

void Camera::moveBackward() {
    Vector3 copy = backward;
    copy.scale(1.5);
    e = e + copy;
    d = d + copy;
    updateAxisVectors();
}

void Camera::reset() {
    e = initialEVector;
    d = initialDVector;
    updateAxisVectors();
}

/** getters */
Vector3 Camera::getPos() {
    return e;
}

