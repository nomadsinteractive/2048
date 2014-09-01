/*
 * GestureDetector.cpp
 *
 *  Created on: 2014-7-25
 *      Author: daiguozhou
 */

#include "GestureDetector.h"

GestureDetector::GestureDetector() : mResult(Unknown) {
}

void GestureDetector::beginPoint(const cocos2d::Point& point) {
    mResult = Unknown;
    mPoints.clear();
    mPoints.push_back(point);
}

void GestureDetector::movePoint(const cocos2d::Point& point) {
    mPoints.push_back(point);
}

GestureDetector::GestureType GestureDetector::endPoint(const cocos2d::Point& point) {
    mPoints.push_back(point);
    if (mPoints.size() < 3) {
        return Unknown;
    }

    GestureType currentGesture = Unknown;
    size_t pointCount = mPoints.size();
    double dx = mPoints[pointCount - 1].x - mPoints[0].x;
    double dy = mPoints[pointCount - 1].y - mPoints[0].y;
    if (abs(dx) > abs(dy)) {
        if (dx > 0) {
            currentGesture = MoveRight;
        } else {
            currentGesture = MoveLeft;
        }
    } else {
        if (dy > 0) {
            currentGesture = MoveUp;
        } else {
            currentGesture = MoveDown;
        }
    }

    if (mResult == Unknown) {
        mResult = currentGesture;
    }

    if (mResult != currentGesture) {
        mResult = Unknown;
    }

    return mResult;
}
