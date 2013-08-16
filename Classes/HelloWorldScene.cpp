#include "HelloWorldScene.h"
#include<cmath>
#include <algorithm>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define For(i,a,b) for(int i=a;i<=b;i++)
 
using namespace cocos2d;
int bulletcount = 0, itemTime = 0;
int numberOfItem = 3;
float tileSize = 32, tankSize = 26;

int d[40][30];//Warning
int dx[4]={0,1,-1,0}, dy[4]={1,0,0,-1};
vector<Bullet> m_stack;
HelloWorld *SCENE;
CCPoint DIR[4],dir[4],trace[40][40],q[1000];
int vitri[4];



#include"init.cpp"
#include"Touched.cpp"
#include"tank_Action.cpp"

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
 
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
 
    // add layer as a child to scene
    scene->addChild(layer);
 
    // return the scene
    return scene;
}

void HelloWorld::update(float pDt)
{	
	srand (time(NULL));
	if (itemTime == 0 && ourCitadel.m_alive && enemyCitadel.m_alive)
	{
		for (int i=0; i<numberOfItem; i++)
		{
			while (true)
			{
				int x = rand() % 30;
				int y = rand() % 20;
				CCString *type = getType(ccp((x+0.5)*tileSize, (y+0.5)*tileSize));
				if (type == NULL || type->compare("brick") == 0) 
				{
					item[i].m_appearance->setPosition(ccp((x+0.5)*tileSize, (y+0.5)*tileSize));

					if (!item[i].m_appearance->boundingBox().intersectsRect(ourCitadel.m_appearance->boundingBox()) && 
						!item[i].m_appearance->boundingBox().intersectsRect(enemyCitadel.m_appearance->boundingBox()))
					{
						item[i].m_appearance->setVisible(true);
						item[i].m_ok = true;
			  			break;
					}
				}
			}

		}
		itemTime = 3600;
	}

	if (itemTime) itemTime -= 1;

	//CCLog("%d",tankSize);
	if (Player.m_alive)
	{
		if ( m_IsTouchMoved)
		{
			CCPoint bigPos = bigcircle->getPosition();
			CCPoint smallPos = smallcircle->getPosition();
			m_DirectionVector = ccpSub(smallPos, bigPos);	
			m_DirectionVector = normalize(m_DirectionVector, bigPos, smallPos);
			Player.m_direction = m_DirectionVector;
			Player.move();
		}
		Player.fire(IsHolding);
	}
    if (ourCitadel.m_alive)
    {
       		if (AI_1.m_alive) AI_1.AI_Des_Action(Player,ourCitadel);
		if(AI_2.m_alive) AI_2.AI_Hun_Action(Player,ourCitadel);
		if(AI_3.m_alive) AI_3.AI_Gru_Action(Player,ourCitadel);
    }
     
	checkBullet();
}

void constant_init()
{
	DIR[0]=ccp(0,1); DIR[1]=ccp(-1,0); DIR[2]=ccp(0,-1); DIR[3]=ccp(1,0);
	vitri[0]=0; vitri[1]=1; vitri[2]=3; vitri[3]=2;
	dir[0]=ccp(0,-1); dir[1]=ccp(-1,0); dir[2]=ccp(0,1); dir[3]=ccp(1,0);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}
