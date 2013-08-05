#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>

using namespace cocos2d;
using namespace std;

class Bullet
{
public:
	CCSprite *appearance;
	float direction;
};


class Tank
{
public:
	CCSprite *appearance;
	long time;
};

class citadel
{
public:
	CCSprite *appearance;
	long hp;
};

class HelloWorld : public cocos2d::CCLayer
{
private:
    CCTMXTiledMap *_tileMap;
	CCTMXLayer *_background, *_tree;
	Tank Player_1;
	CCSize size;
	CCSprite* bigcircle;
	CCSprite* smallcircle;
	bool m_IsTouchMoved;
	CCPoint m_DirectionVector;
	float moveSize;
	bool IsHolding, win;
	float goc;
	float tankSize;
	citadel ourCitadel, enemyCitadel;
	vector<Bullet> stack;

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	//Touch function
	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

	void removeObject( CCNode* sender);
	void update(float pDt);
	bool ok_go(CCString *type);
	bool check(CCPoint pos);
	CCString *getType(CCPoint pos, CCTMXLayer *layer);

	CCPoint tileCoordForPosition(CCPoint position);
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
