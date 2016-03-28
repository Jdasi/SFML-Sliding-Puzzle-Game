#pragma once

#include "cocos2d.h"

class MySpriteAction : public cocos2d::ActionInstant
{
public:
    static MySpriteAction *create(cocos2d::Sprite *sprite, cocos2d::SpriteFrame *frame);

    virtual void update(float time) override;
    virtual MySpriteAction *clone() const override;

    CC_CONSTRUCTOR_ACCESS:
        MySpriteAction() {}
        virtual ~MySpriteAction() {}

private:
    CC_DISALLOW_COPY_AND_ASSIGN(MySpriteAction);

    cocos2d::SpriteFrame *frame = nullptr;
    
    cocos2d::Sprite *sprite = nullptr;
};