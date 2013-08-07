#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>

using namespace cocos2d;
using namespace std;

class Bullet
{
public:
	CCSprite *m_appearance;
	float m_dame, m_speed;
	int id;
};

class Tank
{
public:
	CCSprite *m_appearance;
	float m_time, m_dame, m_speed, m_hp;
	CCPoint m_direction, m_checkPoint1, m_checkPoint2, m_checkPoint3, m_checkPoint4;

	void initTank();
};

class citadel
{
public:
	CCSprite *m_appearance;
	long m_hp;
};

class HelloWorld : public cocos2d::CCLayer
{
private:
	Tank Player1;
	CCSize size;
	CCSprite* bigcircle;
	CCSprite* smallcircle;
	CCPoint m_DirectionVector;
	float moveSize;
	bool IsHolding, m_IsTouchMoved;
	CCTMXTiledMap *_tileMap;
	CCTMXLayer *_background, *_tree;
	bool win, lose;
	citadel ourCitadel, enemyCitadel;
	vector<Bullet> m_stack;

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
	void move(Tank player);
	void fire(Tank &player, bool ok);
	void checkBullet();
	void removeObject( CCNode* sender);
	CCPoint normalize(CCPoint m_DirectionVector, CCPoint bigPos, CCPoint smallPos);
	bool ok_go(CCString *type);
	bool check(CCPoint pos);
	float toAngle(CCPoint direction);
	CCString *getType(CCPoint pos);
	CCPoint tileCoordForPosition(CCPoint position);

	void update(float pDt);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

bool ok_go(CCString *type);
bool check(CCPoint pos);
float toAngle(CCPoint direction);
CCString *getType(CCPoint pos);
CCPoint tileCoordForPosition(CCPoint position);

#endif // __HELLOWORLD_SCENE_H__
