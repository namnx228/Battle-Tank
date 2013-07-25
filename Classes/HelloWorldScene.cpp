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

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

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
		
		//Add tank
		tank = CCSprite::create("dame3.png");
		CC_BREAK_IF(! tank);
		this->addChild(tank,2);
		tank->setPosition(ccp(size.width/2,size.height/2));

		//Add brick wall
		BrickWall = CCSprite::create("BrickWall.png");
		CC_BREAK_IF(! BrickWall);
		this->addChild(BrickWall,3);
		BrickWall->setPosition(ccp(size.width/4,size.height/2));

		//Add tree
		Tree = CCSprite::create("Tree.png");
		CC_BREAK_IF(! Tree);
		this->addChild(Tree,3);
		Tree->setPosition(ccp(size.width/2,size.height/4));

		//Add water
		Water = CCSprite::create("Water.png");
		CC_BREAK_IF(! Water);
		this->addChild(Water,3);
		Water->setPosition(ccp(size.width/4*3,size.height/3*2));		

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


void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint pointTouched = touch->getLocationInView();
	pointTouched = CCDirector::sharedDirector()->convertToGL(pointTouched);
	smallcircle->setVisible(true);
	bigcircle->setVisible(true);
	smallcircle->setPosition(ccp(pointTouched.x, pointTouched.y));
	bigcircle->setPosition(ccp(pointTouched.x, pointTouched.y));
    
	  
}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint pointTouched = touch->getLocationInView();
	pointTouched = CCDirector::sharedDirector()->convertToGL(pointTouched);

	smallcircle->setVisible(false);
	bigcircle->setVisible(false);

	m_IsTouchMoved = false;
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint pointTouched = touch->getLocationInView();
	pointTouched = CCDirector::sharedDirector()->convertToGL(pointTouched);

	smallcircle->setPosition(pointTouched);
	
	m_DirectionVector = ccpSub(smallcircle->getPosition(), bigcircle->getPosition());
	m_DirectionVector = ccpNormalize(m_DirectionVector);

	m_IsTouchMoved = true;

}

void HelloWorld::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint pointTouched = touch->getLocationInView();
	pointTouched = CCDirector::sharedDirector()->convertToGL(pointTouched);
	smallcircle->setVisible(false);
	bigcircle->setVisible(false);

	m_IsTouchMoved = false;
}

void HelloWorld::update(float pDt)
{
	if (m_IsTouchMoved)
	{
		goc = 0;
		CCPoint pos_big = bigcircle->getPosition();
		CCPoint pos_small = smallcircle->getPosition();
		
		// phai tren
		if((pos_small.x >= pos_big.x)&&(pos_small.y > pos_big.y))
		{
			if (abs(m_DirectionVector.x) > abs(m_DirectionVector.y)) 
			{
				m_DirectionVector.y = 0;
				goc=180;
			}	
			else 
			{
				m_DirectionVector.x = 0;
				goc=90;
			}
		}
		
		//trai tren
		if((pos_small.x < pos_big.x)&&(pos_small.y >= pos_big.y))
		{
			if (abs(m_DirectionVector.x) > abs(m_DirectionVector.y)) 
			{
				m_DirectionVector.y = 0;
				goc=0;
			}	
			else 
			{
				m_DirectionVector.x = 0;
				goc=90;
			}
		}

		//trai duoi
		if((pos_small.x <= pos_big.x)&&(pos_small.y < pos_big.y))
		{
			if (abs(m_DirectionVector.x) > abs(m_DirectionVector.y)) 
			{
				m_DirectionVector.y = 0;
				goc=0;
			}	
			else 
			{
				m_DirectionVector.x = 0;
				goc=270;
			}
		}

		//phai duoi
		if((pos_small.x > pos_big.x)&&(pos_small.y <= pos_big.y))
		{
			if (abs(m_DirectionVector.x) > abs(m_DirectionVector.y)) 
			{
				m_DirectionVector.y = 0;
				goc=180;
			}	
			else 
			{
				m_DirectionVector.x = 0;
				goc=270;
			}
		}

		////////////////////////////////
		tank->setRotation(goc);	
		CCPoint pos = tank->getPosition();
		CCSprite *tmp = CCSprite::create("dame3.png");
		this->addChild(tmp, 5);
		tmp->setRotation(goc);	
		tmp->setVisible(false);
		tmp->setPosition(ccpAdd(pos, m_DirectionVector));
		if (! tmp->boundingBox().intersectsRect(BrickWall->boundingBox()) 
			&& ! tmp->boundingBox().intersectsRect(Water->boundingBox()))
		{
			tank->setPosition(ccpAdd(pos, m_DirectionVector));
		}
	}
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

