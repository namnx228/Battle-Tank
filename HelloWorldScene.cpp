#include "HelloWorldScene.h"

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "SIZE_TANK" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".
        CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
        CC_BREAK_IF(! pLabel);

        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width / 2, size.height - 50));

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("HelloWorld.png");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);
		
		tank = CCSprite::create("dame3.png");
		CC_BREAK_IF(! tank);
		this->addChild(tank,10);
		tank->setPosition(ccp(size.width/2,size.height/2));

		smallcircle = CCSprite::create("nho.png");
        CC_BREAK_IF(! smallcircle);
		this->addChild(smallcircle,15);
        smallcircle->setVisible(false);
		///////////////////////////////////
	    bigcircle = CCSprite::create("to.png");
        CC_BREAK_IF(! bigcircle);
		this->addChild(bigcircle,10);
	    bigcircle->setVisible(false);	
		scheduleUpdate();
		setTouchEnabled(true);
        bRet = true;
    } while (0);

	m_IsTouchMoved = false;

    return bRet;
}

bool HelloWorld::checkLimit(CCPoint pointTouched,float Limit)
{
	return (pointTouched.x<=Limit);
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint pointTouched = touch->getLocationInView();
	pointTouched = CCDirector::sharedDirector()->convertToGL(pointTouched);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	IF_IN_LIMIT=checkLimit(pointTouched,Limitcontrol);
	if (IF_IN_LIMIT)
	{
		smallcircle->setVisible(true);
		bigcircle->setVisible(true);
		smallcircle->setPosition(ccp(pointTouched.x, pointTouched.y));
		bigcircle->setPosition(ccp(pointTouched.x, pointTouched.y));
	}
}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint pointTouched = touch->getLocationInView();
	pointTouched = CCDirector::sharedDirector()->convertToGL(pointTouched);
	 CCSize size = CCDirector::sharedDirector()->getWinSize();
	if (IF_IN_LIMIT)
	{
		smallcircle->setVisible(false);
		bigcircle->setVisible(false);
	}
	m_IsTouchMoved = IF_IN_LIMIT=false;
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint pointTouched = touch->getLocationInView();
	pointTouched = CCDirector::sharedDirector()->convertToGL(pointTouched);
	 CCSize size = CCDirector::sharedDirector()->getWinSize();
	bool tmp=IF_IN_LIMIT;
	IF_IN_LIMIT=checkLimit(pointTouched,Limitcontrol);

	if (IF_IN_LIMIT)
	{
		if (!tmp)
		{
			bigcircle->setVisible(true);
			smallcircle->setVisible(true);
			bigcircle->setPosition(pointTouched);
		}
		smallcircle->setPosition(pointTouched);
		m_DirectionVector = ccpSub(smallcircle->getPosition(), bigcircle->getPosition());
		m_DirectionVector = ccpNormalize(m_DirectionVector);
	}
	else
	{
		smallcircle->setVisible(false);
		bigcircle->setVisible(false);
	}
	m_IsTouchMoved = true;

}

void HelloWorld::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint pointTouched = touch->getLocationInView();
	pointTouched = CCDirector::sharedDirector()->convertToGL(pointTouched);
	smallcircle->setVisible(false);
	bigcircle->setVisible(false);

	m_IsTouchMoved = IF_IN_LIMIT=false;
}

void HelloWorld::update(float pDt)
{
	CCPoint vitri=tank->getPosition();
	float SIZE_TANK=tank->getContentSize().width/2;
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	float gioihantren=size.height,gioihanphai=size.width;
	if (IF_IN_LIMIT)
	{
		
		CCPoint pos_big = bigcircle->getPosition();
		CCPoint pos_small = smallcircle->getPosition();
		
		// phai tren
		if((pos_small.x > pos_big.x)&&(pos_small.y > pos_big.y))
		{
			if (abs(m_DirectionVector.x) > abs(m_DirectionVector.y)) 
			{
				m_DirectionVector.y = 0;
				goc=180;
				if(vitri.x+SIZE_TANK>=gioihanphai) m_DirectionVector.x=0;

			}	
			else 
			{
				m_DirectionVector.x = 0;
				goc=90;
				if (vitri.y+SIZE_TANK>=gioihantren) m_DirectionVector.y=0;
			}
		}
		
		
		//trai tren
		if((pos_small.x < pos_big.x)&&(pos_small.y > pos_big.y))
		{
			if (abs(m_DirectionVector.x) > abs(m_DirectionVector.y)) 
			{
				m_DirectionVector.y = 0;
				goc=0;
				if(vitri.x-SIZE_TANK<=0) m_DirectionVector.x=0;
			}	
			else 
			{
				m_DirectionVector.x = 0;
				goc=90;
				if (vitri.y+SIZE_TANK>=gioihantren) m_DirectionVector.y=0;
			}
		}
		//trai duoi
		if((pos_small.x < pos_big.x)&&(pos_small.y < pos_big.y))
		{
			if (abs(m_DirectionVector.x) > abs(m_DirectionVector.y)) 
			{
				m_DirectionVector.y = 0;
				goc=0;
				if(vitri.x-SIZE_TANK<=0) m_DirectionVector.x=0;
			}	
			else 
			{
				m_DirectionVector.x = 0;
				goc=270;
				if (vitri.y-SIZE_TANK<=0) m_DirectionVector.y=0;
			}
		}
		//phai duoi
		if((pos_small.x > pos_big.x)&&(pos_small.y < pos_big.y))
		{
			if (abs(m_DirectionVector.x) > abs(m_DirectionVector.y)) 
			{
				m_DirectionVector.y = 0;
				goc=180;
				if(vitri.x+SIZE_TANK>=gioihanphai) m_DirectionVector.x=0;
			}	
			else 
			{
				m_DirectionVector.x = 0;
				goc=270;
				if (vitri.y-SIZE_TANK<=0) m_DirectionVector.y=0;
			}
		}
		tank->setRotation(goc);	
		CCPoint pos = tank->getPosition();
		tank->setPosition(ccpAdd(pos, m_DirectionVector));
	}
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

