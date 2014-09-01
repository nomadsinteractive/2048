#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "2d/CCLayer.h"
#include "base/CCRefPtr.h"
#include "base/CCPlatformMacros.h"
#include "math/Vec2.h"

#include "GestureDetector.h"

namespace cocos2d {
    class Event;
    class Label;
    class Ref;
    class Scene;
    class Size;
    class Touch;
};

class Card;

class GameScene : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();

    CREATE_FUNC(GameScene);

    GameScene();
    virtual ~GameScene();

    virtual bool init();

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
    void onReStart(cocos2d::Ref* sender);
    void onExit(Ref* sender);

    void createEmptyCards(const cocos2d::Vec2& visibleOrigin, const cocos2d::Size& visibleSize);
    cocos2d::Point cardPosition(int i, int j, const cocos2d::Vec2& visbleOrigin);

    bool hasEmptyCard();
    void generateCard(bool withAnimation);

    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();

    void updateScoreLabel();
    void updateBestScore();

    void doCheck();

    bool checkWin();
    bool checkGameOver();

    bool isWin();
    bool isGameOver();

    void restartGame();

    void debugPrintCards();
private:
    bool mIsGameOver;
    int mLargestNumber;
    int mScore;
    int mBestScoreCache;
    cocos2d::RefPtr<cocos2d::Label> mScoreLabel;

    int mCellSize;
    const int mCellMargin;
    const int mBottomTopMargin;

    cocos2d::RefPtr<Card> mCards[4][4];

    GestureDetector mGestureDetector;
};

#endif // __GAME_SCENE_H__
