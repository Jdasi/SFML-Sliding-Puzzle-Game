#pragma once

#include "GameProfile.h"
#include "GameUnlock.h"

#include "cocos2d.h"

class ProfileUnlocks : public cocos2d::Layer
{
    enum class ContextAction
    {
        select,
        unlock,
        null
    };

public:
    ProfileUnlocks();
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(ProfileUnlocks);

private:
    void initBackdrop();
    void initStarDisplay();
    void initMenu();
    void initPreviewImages();
    void initLabels();

    bool imageClick(cocos2d::Touch *touch, cocos2d::Event *event);

    void performContextAction(cocos2d::Ref *sender);
    void updateActionButton();
    void updateBackdrop() const;
    void updateSelectionRect();
    void updateContextHintLabel();
    void updateNumStarsLabel() const;

    void gotoMainMenu(cocos2d::Ref *sender) const;

    std::vector<GameUnlock> &unlocksRef = GameProfile::getUnlocks();
    std::vector<cocos2d::Sprite*> previewImages;

    cocos2d::Sprite *backdrop;
    cocos2d::Sprite *selectionRect;
    cocos2d::MenuItemSprite *actionButton;
    cocos2d::Label *contextHintLabel;
    cocos2d::Label *numStars;

    int fadedOpacity;
    int selectedOpacity;
    int currentSelection;
    ContextAction action;

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};