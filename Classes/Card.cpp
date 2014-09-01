/*
 * Card.cpp
 *
 *  Created on: 2014-7-22
 *      Author: daiguozhou
 */

#include "cocos2d.h"

#include "Card.h"

USING_NS_CC;

// static
Card* Card::createCard(int initNumber, int width, int height, const Point& position) {
    Card* card = new Card();
    if (card != nullptr && card->initCard(initNumber, width, height, position)) {
        card->autorelease();
        return card;
    }
    CC_SAFE_DELETE(card);
    return nullptr;
}

Card::Card() : mNumber(0), mCard(nullptr), mCardNumber(nullptr) {
}

Card::~Card() {
}

bool Card::initCard(int initNumber, int width, int height, const Point& position) {
    if (!Sprite::init()) {
        return false;
    }

    mNumber = initNumber;

    mCard = LayerColor::create(Color4B(200,190,180,255), width, height);

    const Size& size = mCard->getContentSize();
    const char* numberStr =
            (initNumber > 0) ?
                    __String::createWithFormat("%i", initNumber)->getCString() :
                    "";
    mCardNumber = Label::createWithSystemFont(numberStr, "Consolas", 100);
    mCardNumber->setPosition(Vec2(size.width / 2.0f, size.height / 2.0f));
    mCardNumber->setTag(8);
    mCard->addChild(mCardNumber);

    mCard->ignoreAnchorPointForPosition(false);
    mCard->setAnchorPoint(Vec2(0.5f, 0.5f));
    mCard->setPosition(position);
    this->addChild(mCard);

    return true;
}

int Card::getNumber() const {
    return mNumber;
}

void Card::setNumber(int number) {
    if (mNumber == number) {
        return;
    }

    mNumber = number;

    float fontSize = 100.0f;
    if (mNumber >= 1024) {
        fontSize = 40.0f;
    } else if (mNumber >= 128) {
        fontSize = 60.0f;
    } else if (mNumber >= 16) {
        fontSize = 90.0f;
    }
    mCardNumber->setSystemFontSize(fontSize);

    const char* numberStr =
            (mNumber > 0) ?
                    __String::createWithFormat("%i", mNumber)->getCString() :
                    "";
    mCardNumber->setString(numberStr);

    switch (mNumber) {
    case 0:
        mCard->setColor(Color3B(200, 190, 180));
        break;
    case 2:
        mCard->setColor(Color3B(240, 230, 220));
        break;
    case 4:
        mCard->setColor(Color3B(240, 220, 200));
        break;
    case 8:
        mCard->setColor(Color3B(240, 180, 120));
        break;
    case 16:
        mCard->setColor(Color3B(240, 140, 90));
        break;
    case 32:
        mCard->setColor(Color3B(240, 120, 90));
        break;
    case 64:
    case 128:
        mCard->setColor(Color3B(240, 90, 60));
        break;
    case 256:
    case 512:
        mCard->setColor(Color3B(240, 200, 70));
        break;
    case 1024:
    case 2048:
        mCard->setColor(Color3B(0, 130, 0));
        break;
    default:
        break;
    }
}

void Card::runNewNumberAnimation() {
    auto action = ScaleBy::create(0.10f,0.50f);
    mCard->runAction(Sequence::create(action, action->reverse(), nullptr));
}
