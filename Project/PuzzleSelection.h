#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class PuzzleSelection : public cocos2d::Layer
{
public:
    PuzzleSelection();
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(PuzzleSelection);

private:
    void initBackdrop();
    void initControlMenu();
    void initHelpMenu();
    void initMenuPane();
    void initSliders();
    void initPreviewImage();
    void initArrows();
    void initRewardsPane();

    void updateDisplayedImage() const;
    void updateImageLabels() const;
    void updateRewardsLabel() const;

    bool rightArrowClick(Ref* const sender) const;
    bool leftArrowClick(Ref* const sender) const;

    void puzzleTip(Ref* const sender) const;

    void gotoMainMenu(Ref* const sender) const;
    void gotoPuzzleGame(Ref* const sender) const;

    void setDefaultPuzzle() const;
    int calculatePuzzleValue() const;

    std::vector<std::string> &puzzlesRef;

    cocos2d::Label *xSliderLabel;
    cocos2d::Label *ySliderLabel;

    cocos2d::ui::Slider *xSegmentsSlider;
    cocos2d::ui::Slider *ySegmentsSlider;

    cocos2d::Sprite *displayedImage;
    cocos2d::Label *imageName;
    cocos2d::Label *imageNumber;
    cocos2d::Label *puzzleValue;

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};