#pragma once

#include "cocos2d.h"

class PuzzleComplete : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(PuzzleComplete);

private:
    void initBackdrop();
    void initMenu();

    void rewardPlayer();
    void initPreviewImage();

    void gotoMainMenu(cocos2d::Ref *sender);
    void gotoPuzzleSelection(cocos2d::Ref *sender);

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};