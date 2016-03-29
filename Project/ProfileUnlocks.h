#pragma once

#include "GameUnlock.h"

#include "cocos2d.h"

enum class ContextAction
{
    select,
    unlock,
    null
};

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

    void performContextAction(cocos2d::Ref *sender);
    void updateActionButton();
    void updateBackdrop();

    void gotoMainMenu(cocos2d::Ref *sender);

    static std::vector<GameUnlock> &unlocksRef;
    std::vector<cocos2d::Sprite*> previewImages;

    cocos2d::Sprite *backdrop;

    int fadedOpacity;
    int selectedOpacity;
    int currentSelection;

    cocos2d::MenuItemSprite *actionButton;
    ContextAction action;

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};