#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class PuzzleSelection : public cocos2d::Layer
{
public:
    PuzzleSelection();
    ~PuzzleSelection() = default;

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
    void initAnimSwitch();

    bool rightArrowClick(Ref* const sender) const;
    bool leftArrowClick(Ref* const sender) const;
    void updateImageAndLabels(int currentImageID) const;

    void updateDisplayedImage() const;
    void updateImageLabels() const;
    void updateRewardsLabel() const;

    void puzzleTip(Ref* const sender) const;
    void sanityCheckPuzzles();
    void setDefaultPuzzle() const;
    int calculatePuzzleValue() const;

    bool flipAnimSwitch(cocos2d::Touch* const touch, cocos2d::Event* const event) const;
    void updateAnimStatus() const;
    void updateAnimSwitch() const;

    void gotoMainMenu(Ref* const sender) const;
    void gotoPuzzleGame(Ref* const sender) const;

    std::vector<std::string> &puzzlesRef;

    cocos2d::Label *xSliderLabel;
    cocos2d::Label *ySliderLabel;

    cocos2d::ui::Slider *xSegmentsSlider;
    cocos2d::ui::Slider *ySegmentsSlider;

    cocos2d::Sprite *displayedImage;
    cocos2d::Label *imageName;
    cocos2d::Label *imageNumber;
    cocos2d::Label *puzzleValue;

    cocos2d::Sprite *animSwitch;

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};