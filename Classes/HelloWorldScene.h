#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <vector>

using namespace cocos2d;
using namespace std;

class Item
{
public:
	CCSprite *m_appearance;
	float m_dame, m_speed, m_hp, m_time;
	float m_delaySpeed;
	bool m_ok;
	Item()
	{m_dame = 0; m_speed = 0; m_hp = 0; m_time = 0; m_delaySpeed = 0; m_ok = false;}

	void initItem(float dame, float speed, float hp, float delaySpeed)
	{m_dame = dame; m_speed = speed; m_hp = hp; m_time = 1200; m_delaySpeed = delaySpeed; m_ok = true;}
};

class Citadel
{
public:
	CCSprite *m_appearance;
	long m_hp;

	bool m_alive;
};

class Tank
{
public:
	CCSprite *m_appearance;
	float m_time, m_dame, m_speed, m_hp;
	bool m_alive,OK_Fire;
	CCPoint m_direction, m_checkPoint1, m_checkPoint2, m_checkPoint3, m_checkPoint4,start;
	Item m_item;

	void initTank(CCPoint,float,const char* ,float,float,float);
	void move();
	void fire(bool);
};

class AI : public Tank
{
public:
	
	int count;
	CCPoint go[400];
	string Way;
	int Waycount;
	int bricklimit;
	int Keep_Going;
	void findWay(int,int);
	bool Saw(const Tank&,int&);
	bool Know_Where_Citadel(const Citadel&,int&);
	bool nothing( CCPoint, CCPoint,int);
	friend bool OK_GO(int,int);
	void dfs(int,int);
    friend CCPoint getCentrePos(int,int);
	friend void swap(CCPoint&,CCPoint&);
	void bfs(CCPoint);
	void setDirection();
	//CCPoint trace[32][22];
};


class Bullet
{
public:
	CCSprite *m_appearance;
	float m_dame, m_speed;
	void setEffect(Tank&);
	void setEffect(Citadel&);
	void setEffect(Bullet &bullet1);
};

class Destroyer:public AI
{
public:
    void AI_Des_Init(CCPoint,float,string,float,float,float);
    void AI_Des_Action(const Tank&,const Citadel&);
};

class Hunter:public Destroyer
{
public:
    void AI_Hun_Init(CCPoint,float,string,float,float,float);
    void AI_Hun_Action(const Tank&,const Citadel&);
    int m_timedelay,m_timeFire;
};

class Gruarder:public Hunter
{
public:
	bool loss;
	void AI_Gru_Init(CCPoint,float,string,float,float,float);
	void AI_Gru_Action(const Tank&, const Citadel&);
	bool scanEnemy(const Tank&),goHome();
	void findWay_Gru(int,int);
	void dfs_Gru(int,int);
};


class HelloWorld : public cocos2d::CCLayer
{
    public:
	CCTMXTiledMap *_tileMap;
	CCTMXLayer *_background, *_tree;
	Tank Player;
	Destroyer AI_1;
	Hunter AI_2;
	Gruarder AI_3;
	Citadel ourCitadel, enemyCitadel;
	Item item[3];

	CCSize size;
	CCSprite *bigcircle, *smallcircle;
	CCPoint m_DirectionVector;
	float moveSize;
	bool IsHolding, m_IsTouchMoved;

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
	//void move(Tank &player);
//	void fire(Tank &player, bool ok);
	void checkBullet();
	void removeObject( CCNode* sender);

	CCPoint normalize(CCPoint m_DirectionVector, CCPoint bigPos, CCPoint smallPos);

//	void setEffect(Bullet &bullet, Tank &tank);
	//void setEffect(Bullet &bullet, Citadel &citadel);

	//Function for AI
//	void bfs(AI &ai, CCPoint target); 
	//void setDirection(AI &ai);
	void update(float pDt);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};
// ham toan cuc
//bool ok_go(CCString *type);
bool check(CCPoint pos);
float toAngle(CCPoint direction);
CCString *getType(CCPoint pos);
CCPoint tileCoordForPosition(CCPoint position);
void constant_init();
#endif // __HELLOWORLD_SCENE_H__
