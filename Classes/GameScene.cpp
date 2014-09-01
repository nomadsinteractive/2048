/*
 * GameScene.cpp
 *
 *  Created on: 2014-7-21
 *      Author: daiguozhou
 */

#include "cocos2d.h"

#include "Card.h"
#include "GameScene.h"

USING_NS_CC;

static const char* KEY_BEST_SCORE = "best_score";

static const char* MOVE_FAILED_SOUND = "MoveFailed.ogg";
static const char* MOVE_SUCESS_SOUND = "MoveSucess.ogg";

Scene* GameScene::createScene() {
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

GameScene::GameScene() :
    mIsGameOver(false), mLargestNumber(0), mScore(0), mBestScoreCache(0), mScoreLabel(nullptr),
    mCellSize(0), mCellMargin(20), mBottomTopMargin(40) {
}

GameScene::~GameScene() {
}

bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }

    mBestScoreCache = UserDefault::getInstance()->getIntegerForKey(KEY_BEST_SCORE, 0);

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto background = LayerColor::create(Color4B(180, 170, 160, 255));
    this->addChild(background);

    MenuItemFont::setFontName("Consolas");
    MenuItemFont::setFontSize(80);
    auto menuItemRestart = MenuItemFont::create("RESTART",
            CC_CALLBACK_1(GameScene::onReStart, this));
    auto menuItemExit = MenuItemFont::create("EXIT",
            CC_CALLBACK_1(GameScene::onExit, this));
    auto menu = Menu::create(menuItemExit, menuItemRestart, nullptr);
    menu->setColor(Color3B(0, 255, 255));
    menu->alignItemsVertically();
    const float xPos = origin.x + visibleSize.width
            - menuItemRestart->getContentSize().width / 2.0f - 20.0f;
    menu->setPosition(
            Point(xPos, origin.y + visibleSize.height / 2.0f + 150.0f));
    this->addChild(menu);

    auto scoreTitle = Label::createWithSystemFont("SCORE", "Consolas", 80);
    scoreTitle->setPosition(
            Point(xPos, origin.y + visibleSize.height / 2.0f - 50.0f));
    this->addChild(scoreTitle);

    mScore = 0;
    mScoreLabel = Label::createWithSystemFont("0", "Consolas", 80);
    mScoreLabel->setPosition(
            Point(xPos, origin.y + visibleSize.height / 2.0f - 150.0f));
    this->addChild(mScoreLabel);

    // enable touch event
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    this->setTouchEnabled(true);

    createEmptyCards(origin, visibleSize);

    generateCard(false);
    generateCard(false);

    return true;
}

bool GameScene::onTouchBegan(Touch* touch, Event* event) {
    mGestureDetector.beginPoint(touch->getLocation());
    return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event) {
    mGestureDetector.movePoint(touch->getLocation());
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
    if (isWin() || isGameOver()) {
        return;
    }

    GestureDetector::GestureType gesture = mGestureDetector.endPoint(touch->getLocation());
    bool gestureValid = true;
    bool moved = false;
    switch (gesture) {
    case GestureDetector::MoveLeft:
        moved = moveLeft();
        break;
    case GestureDetector::MoveRight:
        moved = moveRight();
        break;
    case GestureDetector::MoveUp:
        moved = moveUp();
        break;
    case GestureDetector::MoveDown:
        moved = moveDown();
        break;
    case GestureDetector::Unknown:
    default:
        gestureValid = false;
        break;
    }

    if (gestureValid) {
        if (moved) {
            generateCard(true);
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                    MOVE_SUCESS_SOUND, false);
        } else {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                    MOVE_FAILED_SOUND, false);
        }
        doCheck();
    }
}

bool GameScene::moveLeft() {
    bool isMoved = false;
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            for (int x1 = x + 1; x1 < 4; ++x1) {
                if (mCards[x1][y]->getNumber() > 0) {
                    if (mCards[x][y]->getNumber() == 0) {
                        mCards[x][y]->setNumber(mCards[x1][y]->getNumber());
                        mCards[x1][y]->setNumber(0);
                        --x;
                        isMoved = true;
                    } else if (mCards[x][y]->getNumber() == mCards[x1][y]->getNumber()) {
                        int newNumber = mCards[x1][y]->getNumber() * 2;
                        if (mLargestNumber < newNumber) {
                            mLargestNumber = newNumber;
                        }
                        mCards[x][y]->setNumber(newNumber);
                        mCards[x1][y]->setNumber(0);
                        mScore += newNumber;
                        updateScoreLabel();
                        isMoved = true;
                    }
                    break;
                }
            }
        }
    }
    return isMoved;
}

bool GameScene::moveRight() {
    bool isMoved = false;
    for (int y = 0; y < 4; ++y) {
        for (int x = 3; x > -1; --x) {
            for (int x1 = x - 1; x1 > -1; --x1) {
                if (mCards[x1][y]->getNumber() > 0) {
                    if (mCards[x][y]->getNumber() == 0) {
                        mCards[x][y]->setNumber(mCards[x1][y]->getNumber());
                        mCards[x1][y]->setNumber(0);
                        ++x;
                        isMoved = true;
                    } else if (mCards[x][y]->getNumber() == mCards[x1][y]->getNumber()) {
                        int newNumber = mCards[x1][y]->getNumber() * 2;
                        if (newNumber > mLargestNumber) {
                            mLargestNumber = newNumber;
                        }
                        mCards[x][y]->setNumber(newNumber);
                        mCards[x1][y]->setNumber(0);
                        mScore += newNumber;
                        updateScoreLabel();
                        isMoved = true;
                    }
                    break;
                }
            }
        }
    }
    return isMoved;
}

bool GameScene::moveUp() {
    bool isMoved = false;
    for (int x = 0; x < 4; ++x) {
        for (int y = 3; y > -1; --y) {
            for (int y1 = y - 1; y1 > -1; --y1) {
                if (mCards[x][y1]->getNumber() > 0) {
                    if (mCards[x][y]->getNumber() == 0) {
                        mCards[x][y]->setNumber(mCards[x][y1]->getNumber());
                        mCards[x][y1]->setNumber(0);
                        isMoved = true;
                    } else if (mCards[x][y]->getNumber() == mCards[x][y1]->getNumber()) {
                        int newNumber = mCards[x][y1]->getNumber() * 2;
                        if (newNumber > mLargestNumber) {
                            mLargestNumber = newNumber;
                        }
                        mCards[x][y]->setNumber(newNumber);
                        mCards[x][y1]->setNumber(0);
                        mScore += newNumber;
                        updateScoreLabel();
                        isMoved = true;
                    }
                    break;
                }
            }
        }
    }
    return isMoved;
}

bool GameScene::moveDown() {
    bool isMoved = false;
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y <4; ++y) {
            for (int y1 = y + 1; y1 < 4; ++y1) {
                if (mCards[x][y1]->getNumber() > 0) {
                    if (mCards[x][y]->getNumber() == 0) {
                        mCards[x][y]->setNumber(mCards[x][y1]->getNumber());
                        mCards[x][y1]->setNumber(0);
                        isMoved = true;
                    } else if (mCards[x][y]->getNumber() == mCards[x][y1]->getNumber()) {
                        int newNumber = mCards[x][y1]->getNumber() * 2;
                        if (newNumber > mLargestNumber) {
                            mLargestNumber = newNumber;
                        }
                        mCards[x][y]->setNumber(newNumber);
                        mCards[x][y1]->setNumber(0);
                        mScore += newNumber;
                        updateScoreLabel();
                        isMoved = true;
                    }
                    break;
                }
            }
        }
    }
    return isMoved;
}

void GameScene::updateScoreLabel() {
    mScoreLabel->setString(__String::createWithFormat("%i", mScore)->getCString());
}

void GameScene::doCheck() {

    if (!checkWin()) {
        checkGameOver();
    }
}

bool GameScene::checkWin() {
    if (isWin()) {
        updateBestScore();

        const Point& visibleOrigin = Director::getInstance()->getVisibleOrigin();
        const Size& visibleSize = Director::getInstance()->getVisibleSize();
        const Point& center = Point(visibleOrigin.x + visibleSize.width / 2.0f,
                                    visibleOrigin.y + visibleSize.height / 2.0f);

        auto sucessLayer = LayerColor::create(Color4B(0, 0, 0, 220));

        auto winGameLabel =
                Label::createWithSystemFont("YOU WIN", "Consolas", 100);
        winGameLabel->setPosition(Point(center.x, center.y + 180));
        sucessLayer->addChild(winGameLabel);

        char score[64];
        sprintf(score, "SCORE %d", mScore);
        auto bestScoreLabel = Label::createWithSystemFont(score, "Consolas", 80);
        bestScoreLabel->setColor(Color3B(128, 0, 128));
        bestScoreLabel->setPosition(Point(center.x, center.y + 20.0f));
        sucessLayer->addChild(bestScoreLabel);

        sprintf(score, "BEST  %d", mBestScoreCache);
        auto scoreLabel = Label::createWithSystemFont(score, "Consolas", 80);
        scoreLabel->setColor(Color3B(80, 160, 100));
        scoreLabel->setPosition(Point(center.x, center.y - 100.0f));
        sucessLayer->addChild(scoreLabel);

        getParent()->addChild(sucessLayer, 1);

        // setup touch event listener for sucessLayer, it swallows all touch events
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        touchListener->onTouchBegan =
                [](Touch* touch, Event* event) -> bool { return true; };
        touchListener->onTouchMoved = [] (Touch* touch, Event* event) {};
        touchListener->onTouchCancelled = [] (Touch* touch, Event* event) {};
        touchListener->onTouchEnded =
                [sucessLayer] (Touch* touch, Event* event) {
                    sucessLayer->removeFromParent();
                };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
                touchListener, sucessLayer);

        return true;
    }

    return false;
}

bool GameScene::checkGameOver() {
    if (isGameOver()) {
        updateBestScore();

        auto gameOverLayer = LayerColor::create(Color4B(0, 0, 0, 220));

        const Point& visibleOrigin = Director::getInstance()->getVisibleOrigin();
        const Size& visibleSize = Director::getInstance()->getVisibleSize();
        const Point& center = Point(visibleOrigin.x + visibleSize.width / 2.0f,
                                    visibleOrigin.y + visibleSize.height / 2.0f);

        auto gameOverLabel = Label::createWithSystemFont("GAME OVER", "Consolas", 80);
        gameOverLabel->setColor(Color3B(255, 255, 0));
        gameOverLabel->setPosition(Point(center.x, center.y + 180.0f));
        gameOverLayer->addChild(gameOverLabel);

        char score[64];
        sprintf(score, "BEST  %d", mBestScoreCache);
        auto bestScoreLabel = Label::createWithSystemFont(score, "Consolas", 80);
        bestScoreLabel->setColor(Color3B(128, 0, 128));
        bestScoreLabel->setPosition(Point(center.x, center.y + 55.0f));
        gameOverLayer->addChild(bestScoreLabel);

        sprintf(score, "SCORE %d", mScore);
        auto scoreLabel = Label::createWithSystemFont(score, "Consolas", 80);
        scoreLabel->setColor(Color3B(80, 160, 100));
        scoreLabel->setPosition(Point(center.x, center.y - 55.0f));
        gameOverLayer->addChild(scoreLabel);

        MenuItemFont::setFontName("Consolas");
        MenuItemFont::setFontSize(80);
        auto menuItemRestart = MenuItemFont::create("RESTART",
                [gameOverLayer, this] (Ref* sender) {
            gameOverLayer->removeFromParent();
            restartGame();
        });
        auto menu = Menu::create(menuItemRestart, nullptr);
        menu->setColor(Color3B(0, 255, 255));
        menu->setPosition(Point(center.x, center.y - 180.0f));
        gameOverLayer->addChild(menu);

        getParent()->addChild(gameOverLayer, 1);

        // setup touch event listener for gameOverLayer, it does nothing but swallow the touch events
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        touchListener->onTouchBegan =
                [] (Touch* touch, Event* event) -> bool { return true; };
        touchListener->onTouchMoved = [] (Touch* touch, Event* event) {};
        touchListener->onTouchCancelled = [] (Touch* touch, Event* event) {};
        touchListener->onTouchEnded = [] (Touch* touch, Event* event) {};
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
                touchListener, gameOverLayer);

        return true;
    }

    return false;
}

void GameScene::updateBestScore() {
    if (mScore > mBestScoreCache) {
        mBestScoreCache = mScore;
        UserDefault::getInstance()->setIntegerForKey(KEY_BEST_SCORE, mBestScoreCache);
    }
}

bool GameScene::isWin() {
    return mLargestNumber == 2048;
}

bool GameScene::isGameOver() {
    if (isWin()) {
        return false;
    }

    if (mIsGameOver) {
        return true;
    }

    // do the check
    mIsGameOver = true;
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (mCards[x][y]->getNumber() == 0
                || (x < 3 && mCards[x][y]->getNumber() == mCards[x + 1][y]->getNumber())
                || (x > 0 && mCards[x][y]->getNumber() == mCards[x - 1][y]->getNumber())
                || (y < 3 && mCards[x][y]->getNumber() == mCards[x][y + 1]->getNumber())
                || (y > 0 && mCards[x][y]->getNumber() == mCards[x][y - 1]->getNumber())) {
                mIsGameOver = false;
                break;
            }
        }
    }
    return mIsGameOver;
}

void GameScene::restartGame() {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            mCards[x][y]->setNumber(0);
        }
    }

    mIsGameOver = false;
    mLargestNumber = 0;
    mScore = 0;
    updateScoreLabel();

    generateCard(false);
    generateCard(false);
}

void GameScene::createEmptyCards(const Vec2& visibleOrigin, const Size& visibleSize) {
    mCellSize = (visibleSize.height - 3 * mCellMargin - mBottomTopMargin * 2) / 4;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            auto card =
                    Card::createCard(0, mCellSize, mCellSize, cardPosition(i, j, visibleOrigin));
            mCards[i][j] = card;
            addChild(card);
        }
    }
}

cocos2d::Point GameScene::cardPosition(int i, int j, const Vec2& visbleOrigin) {
    float x = visbleOrigin.x + mBottomTopMargin + mCellSize / 2.0f + i * (mCellSize + mCellMargin);
    float y = visbleOrigin.y + mBottomTopMargin + mCellSize / 2.0f + j * (mCellSize + mCellMargin);
    return Point(x, y);
}

bool GameScene::hasEmptyCard() {
    bool result = false;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (mCards[i][j]->getNumber() == 0) {
                result = true;
                break;
            }
        }
    }
    return result;
}

void GameScene::generateCard(bool withAnimation) {
    while (true) {
        int i = CCRANDOM_0_1() * 4;
        int j = CCRANDOM_0_1() * 4;
        if (mCards[i][j]->getNumber() == 0) {
            mCards[i][j]->setNumber((CCRANDOM_0_1() * 10 < 1) ? 4 : 2);
            if (withAnimation) {
                mCards[i][j]->runNewNumberAnimation();
            }
            break;
        }
        if (!hasEmptyCard()) {
            break;
        }
    }
}

void GameScene::onReStart(Ref* sender) {
    restartGame();
}

void GameScene::onExit(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameScene::debugPrintCards() {
    for (int y = 3; y > -1; --y) {
        cocos2d::log("%d %d %d %d",
                mCards[0][y]->getNumber(), mCards[1][y]->getNumber(),
                mCards[2][y]->getNumber(), mCards[3][y]->getNumber());
    }
}
