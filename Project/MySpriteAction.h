#pragma once
#include "cocos2d.h"

class MySpriteAction : public cocos2d::ActionInstant
{
public:
    // similar to scenes, we need a static function that returns a new instance of our custom action
    static MySpriteAction *create(cocos2d::Sprite *sprite, cocos2d::SpriteFrame *frame);

    // the update function will control the changing of the sprite's image
    virtual void update(float time) override;
    virtual MySpriteAction *clone() const override;

    CC_CONSTRUCTOR_ACCESS:
        MySpriteAction() {}
        virtual ~MySpriteAction() {}

private:
    CC_DISALLOW_COPY_AND_ASSIGN(MySpriteAction);

    // the sprite frame we want to change to
    cocos2d::SpriteFrame *frame = nullptr;
    
    // the sprite's image we're going to change
    cocos2d::Sprite *sprite = nullptr;
};