#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>
#include "SimpleAudioEngine.h"


using namespace cocos2d;
using namespace std;

class Tank
{
public:
	CCSprite *appearance;
	long time;
};

class HelloWorld : public cocos2d::CCLayer
{
	CCSprite* bigcircle;
	CCSprite* smallcircle;
	Tank Player_1;
	CCSprite* BrickWall;
	CCSprite* Water;
	CCSprite* Tree;
	float goc, moveSize;
	bool m_IsTouchMoved;
	CCPoint m_DirectionVector;
	CCSize size;
	bool IsHolding;
	vector<CCSprite*> stack;

public:
	

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	void removeObject( CCNode* sender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
	
	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

	void update(float pDt);
};

#endif  // __HELLOWORLD_SCENE_H__