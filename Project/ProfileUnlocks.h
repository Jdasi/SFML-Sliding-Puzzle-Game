#pragma once

#include "GameUnlock.h"

#include "cocos2d.h"

class ProfileUnlocks : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(ProfileUnlocks);

private:
    void initBackdrop();
    void initMenu();
    void initPreviewImages();

    bool imageClick(cocos2d::Touch *touch, cocos2d::Event *event);

    void contextAction(cocos2d::Ref *sender);
    void updateActionButton();

    void gotoMainMenu(cocos2d::Ref *sender);

    int fadedOpacity = 150;
    int selectedOpacity = 255;

    static std::vector<GameUnlock> &unlocksRef;
    std::vector<cocos2d::Sprite*> previewImages;

    int currentSelection;

    cocos2d::MenuItemSprite *actionButton;

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};