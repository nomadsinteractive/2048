/*
 * GestureDetector.h
 *
 *  Created on: 2014-7-25
 *      Author: daiguozhou
 */

#ifndef __GESTURE_DETECTOR_H__
#define __GESTURE_DETECTOR_H__

#include <vector>

#include "math/Vec2.h"

class GestureDetector {
public:
    enum GestureType {
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        Unknown
    };

    GestureDetector();
    void beginPoint(const cocos2d::Point& point);
    void movePoint(const cocos2d::Point& point);
    GestureType endPoint(const cocos2d::Point& point);

private:
    std::vector<cocos2d::Point> mPoints;
    GestureType mResult;
};


#endif /* __GESTURE_DETECTOR_H__ */
