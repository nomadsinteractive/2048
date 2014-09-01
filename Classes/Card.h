/*
 * Card.h
 *
 *  Created on: 2014-7-22
 *      Author: daiguozhou
 */

#ifndef __CARD_H__
#define __CARD_H__

#include "2d/CCSprite.h"
#include "math/Vec2.h"

namespace cocos2d {
    class Label;
    class LayerColor;
};

class Card : public cocos2d::Sprite {
public:
    static Card* createCard(int initNumber, int width, int height, const cocos2d::Point& position);
    Card();
    virtual ~Card();

    int getNumber() const;
    void setNumber(int number);
    void runNewNumberAnimation();
private:
    bool initCard(int initNumber, int width, int height, const cocos2d::Point& position);
private:
    int mNumber;
    cocos2d::LayerColor* mCard;
    cocos2d::Label* mCardNumber;
};


#endif /* __CARD_H__ */
