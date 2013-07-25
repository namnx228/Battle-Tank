#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"


using namespace cocos2d;


class HelloWorld : public cocos2d::CCLayer
{
	CCSprite* bigcircle;
	CCSprite* smallcircle;
	CCSprite* tank;
	CCSprite* BrickWall;
	CCSprite* Water;
	CCSprite* Tree;
	bool m_IsTouchMoved;
	float goc;
	CCPoint m_DirectionVector;
public:
	

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
	
	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

	void update(float pDt);
};

#endif  // __HELLOWORLD_SCENE_H__