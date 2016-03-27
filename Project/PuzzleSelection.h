#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class PuzzleSelection : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(PuzzleSelection);

private:
    void initBackdrop();
    void initMenu();
    void initSliders();
    void initPreviewImage();
    void initArrows();
    void initRewardsPane();

    void updateDisplayedImage();
    void updateImageLabels();
    void updateRewardsLabel();

    bool rightArrowClick(cocos2d::Ref *sender);
    bool leftArrowClick(cocos2d::Ref *sender);

    void gotoMainMenu(cocos2d::Ref *sender);
    void gotoPuzzleGame(cocos2d::Ref *sender);

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };

    cocos2d::Label *xSliderLabel;
    cocos2d::Label *ySliderLabel;

    cocos2d::ui::Slider *xSegmentsSlider;
    cocos2d::ui::Slider *ySegmentsSlider;

    cocos2d::Sprite *displayedImage;
    cocos2d::Label *imageName;
    cocos2d::Label *imageNumber;
    cocos2d::Label *puzzleValue;
};