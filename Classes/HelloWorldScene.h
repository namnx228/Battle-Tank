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
};

class Item
{
public:
	CCSprite *m_appearance;
	float m_dame, m_speed, m_hp, m_time;
	float m_delaySpeed;
	bool m_ok;

	void initItem(float dame, float speed, float hp, float time, float delaySpeed)
	{m_dame = dame; m_speed = speed; m_hp = hp; m_time = time; m_delaySpeed = delaySpeed;}
};

class Tank
{
public:
	CCSprite *m_appearance;
	float m_time, m_dame, m_speed, m_hp;
	bool m_ok;
	CCPoint m_direction, m_checkPoint1, m_checkPoint2, m_checkPoint3, m_checkPoint4;
	Item m_item;

	void initTank();
};

class Citadel
{
public:
	CCSprite *m_appearance;
	long m_hp;

	bool m_ok;
};

class AI : public Tank
{
public:
	int count, m_timedelay;
	CCPoint go[400];
};

class HelloWorld : public cocos2d::CCLayer
{
private:
	CCTMXTiledMap *_tileMap;
	CCTMXLayer *_background, *_tree;
	Tank Player1;
	AI Player2;
	Citadel ourCitadel, enemyCitadel;
	Item speedUp;

	CCSize size;
	CCSprite *bigcircle, *smallcircle;
	CCPoint m_DirectionVector;
	float moveSize, tankSize, tileSize;
	bool IsHolding, m_IsTouchMoved;
	vector<Bullet> m_stack;

	int free[32][22];
	CCPoint q[1000], trace[32][22];

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

	//Function for player
	void move(Tank &player);
	void fire(Tank &player, bool ok);
	void checkBullet();
	void removeObject( CCNode* sender);

	CCPoint normalize(CCPoint m_DirectionVector, CCPoint bigPos, CCPoint smallPos);
	bool ok_go(CCString *type);
	bool check(CCPoint pos);
	float toAngle(CCPoint direction);
	CCString *getType(CCPoint pos);
	CCPoint tileCoordForPosition(CCPoint position);
	

	void setEffect(Bullet &bullet, Tank &tank);
	void setEffect(Bullet &bullet, Citadel &citadel);

	//Function for AI
	void bfs(AI &ai, CCPoint target); 
	void setDirection(AI &ai);
	bool canFire(CCPoint pos1, CCPoint pos2, int tiles);
	bool checkFire(AI &ai, CCSprite *target);

	void update(float pDt);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
