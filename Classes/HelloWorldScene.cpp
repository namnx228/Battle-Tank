#include "HelloWorldScene.h"
 
using namespace cocos2d;

int bulletcount = 0;
 
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
 
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	size = CCDirector::sharedDirector()->getWinSize();
	moveSize = size.width/4;

	//Add tiled map
    _tileMap = new CCTMXTiledMap();
    _tileMap->initWithTMXFile("map.tmx");
    _background = _tileMap->layerNamed("background");
	_tree = _tileMap->layerNamed("tree");
	_tileMap->removeAllChildren();
	this->addChild(_background, -5);
	this->addChild(_tree, 5);
//	_background->removeTileAt( ccp(3.0, 3.0) );

	//Add tank
	Player_1.appearance = CCSprite::create("dame3.png");
	this->addChild(Player_1.appearance,1);
	Player_1.appearance->setPosition(ccp(size.width/4,Player_1.appearance->getContentSize().height/2));
	Player_1.time = 0; 
	tankSize = Player_1.appearance->getContentSize().width;

	//Add Citadel
	ourCitadel.appearance = CCSprite::create("thanh.png");
	this->addChild(ourCitadel.appearance,1);
	ourCitadel.appearance->setPosition(ccp(size.width/2,ourCitadel.appearance->getContentSize().height/2));
	ourCitadel.hp = 50; 

	enemyCitadel.appearance = CCSprite::create("thanh.png");
	this->addChild(enemyCitadel.appearance,1);
	enemyCitadel.appearance->setPosition(ccp(size.width/2,size.height - enemyCitadel.appearance->getContentSize().height/2));
	enemyCitadel.hp = 50; 
	


	//Add virtual control
	smallcircle = CCSprite::create("nho.png");
	this->addChild(smallcircle,15);
    smallcircle->setVisible(false);

	bigcircle = CCSprite::create("to.png");
	this->addChild(bigcircle,10);
	bigcircle->setVisible(false);	

	m_IsTouchMoved = false;
	IsHolding = false;
	goc = 0;
	win = false;
	setTouchEnabled(true);
	scheduleUpdate();
	return true;
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint pointTouched = touch->getLocationInView();
	pointTouched = CCDirector::sharedDirector()->convertToGL(pointTouched);

	if (pointTouched.x <= moveSize)
	{
		smallcircle->setVisible(true);
		bigcircle->setVisible(true);
		smallcircle->setPosition(ccp(pointTouched.x, pointTouched.y));
		bigcircle->setPosition(ccp(pointTouched.x, pointTouched.y));
	}
	else
		IsHolding = true;

}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint pointTouched = touch->getLocationInView();
	pointTouched = CCDirector::sharedDirector()->convertToGL(pointTouched);

	if (pointTouched.x <= moveSize)
	{
		smallcircle->setVisible(false);
		bigcircle->setVisible(false);
	}
	else
		IsHolding = false;
	m_IsTouchMoved = false;
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint pointTouched = touch->getLocationInView();
	pointTouched = CCDirector::sharedDirector()->convertToGL(pointTouched);

	if (pointTouched.x <= moveSize)
	{
		smallcircle->setVisible(true);
		bigcircle->setVisible(true);
		smallcircle->setPosition(pointTouched);
		m_IsTouchMoved = true;
		IsHolding = false;
	}
	else 
	{
		smallcircle->setVisible(false);
		bigcircle->setVisible(false);
		m_IsTouchMoved = false;
	}

}

void HelloWorld::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint pointTouched = touch->getLocationInView();
	pointTouched = CCDirector::sharedDirector()->convertToGL(pointTouched);
	if (pointTouched.x <= moveSize) 
	{
		smallcircle->setVisible(false);
		bigcircle->setVisible(false);
	}
	m_IsTouchMoved = false;
}

void HelloWorld::update(float pDt)
{
	if (m_IsTouchMoved)
	{
		CCPoint pos = Player_1.appearance->getPosition();
		CCPoint pos_big = bigcircle->getPosition();
		CCPoint pos_small = smallcircle->getPosition();
		m_DirectionVector = ccpSub(smallcircle->getPosition(), bigcircle->getPosition());
		m_DirectionVector = ccpNormalize(m_DirectionVector);		

		// phai tren
		if((pos_small.x >= pos_big.x)&&(pos_small.y > pos_big.y))
		{
			if (abs(m_DirectionVector.x) >= abs(m_DirectionVector.y)) 
			{
				m_DirectionVector.y = 0;
				goc=180;
				if (pos.x + tankSize/2 + m_DirectionVector.x  > size.width)
					m_DirectionVector.x = 0;
			}	
			else 
			{
				m_DirectionVector.x = 0;
				goc=90;
				if (pos.y + tankSize/2 + m_DirectionVector.y > size.height)
					m_DirectionVector.y = 0;
			}
		}

		//trai tren
		if((pos_small.x < pos_big.x)&&(pos_small.y >= pos_big.y))
		{
			if (abs(m_DirectionVector.x) >= abs(m_DirectionVector.y)) 
			{
				m_DirectionVector.y = 0;
				goc=0;
				if (pos.x - tankSize/2 - m_DirectionVector.x < 0)
					m_DirectionVector.x = 0;
			}	
			else 
			{
				m_DirectionVector.x = 0;
				goc=90;
				if (pos.y + tankSize/2 + m_DirectionVector.y > size.height)
					m_DirectionVector.y = 0;
			}
		}

		//trai duoi
		if((pos_small.x <= pos_big.x)&&(pos_small.y < pos_big.y))
		{
			if (abs(m_DirectionVector.x) >= abs(m_DirectionVector.y)) 
			{
				m_DirectionVector.y = 0;
				goc=0;
				if (pos.x - tankSize/2 - m_DirectionVector.x < 0)
					m_DirectionVector.x = 0;
			}	
			else 
			{
				m_DirectionVector.x = 0;
				goc=270;
				if (pos.y - tankSize/2 - m_DirectionVector.y < 0)
					m_DirectionVector.y = 0;
			}
		}

		//phai duoi
		if((pos_small.x > pos_big.x)&&(pos_small.y <= pos_big.y))
		{
			if (abs(m_DirectionVector.x) >= abs(m_DirectionVector.y)) 
			{
				m_DirectionVector.y = 0;
				goc=180;
				if (pos.x + tankSize/2 + m_DirectionVector.x  > size.width)
					m_DirectionVector.x = 0;
			}	
			else 
			{
				m_DirectionVector.x = 0;
				goc=270;
				if (pos.y - tankSize/2 - m_DirectionVector.y < 0)
					m_DirectionVector.y = 0;
			}
		}
		CCPoint nextPos = ccpAdd(pos, m_DirectionVector);
		CCPoint checkPoint;
		CCPoint checkPoint_1 = ccpAdd(nextPos, ccp(-tankSize/2, tankSize/2));
		CCPoint checkPoint_2 = ccpAdd(nextPos, ccp(tankSize/2, tankSize/2));
		CCPoint checkPoint_3 = ccpAdd(nextPos, ccp(tankSize/2, -tankSize/2));
		CCPoint checkPoint_4 = ccpAdd(nextPos, ccp(-tankSize/2, -tankSize/2));

		Player_1.appearance->setRotation(goc);	
		if (check(checkPoint_1) && check(checkPoint_2)  && check(checkPoint_3) && check(checkPoint_4))
		{
			Player_1.appearance->setPosition(nextPos);
		}
		
		
	}

	if (Player_1.time >0) Player_1.time -=1;
	if (IsHolding)
	{
		if (Player_1.time == 0) 
		{
		Bullet dame;
		dame.appearance = CCSprite::create("dame.png");
		this->addChild(dame.appearance,4);
		dame.direction = Player_1.appearance->getRotation();
		CCPoint pos = Player_1.appearance->getPosition();
		dame.appearance->setPosition(pos);
		dame.appearance->setTag(bulletcount++);
		stack.push_back(dame);

		if ( dame.direction == 0) dame.appearance->runAction(CCSequence::create(CCMoveBy::create(2, ccp(-300, 0)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)), NULL));
		if ( dame.direction == 90) dame.appearance->runAction(CCSequence::create(CCMoveBy::create(2, ccp(0, 300)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)), NULL));
		if ( dame.direction == 180) dame.appearance->runAction(CCSequence::create(CCMoveBy::create(2, ccp(300, 0)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)), NULL));
		if ( dame.direction == 270) dame.appearance->runAction(CCSequence::create(CCMoveBy::create(2, ccp(0, -300)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)), NULL));		
		Player_1.time = 50;
		}
	}

	for(int i=0;i<stack.size();i++)
	{	
		
		CCPoint pos = stack[i].appearance->getPosition();
		CCPoint tileCoord = tileCoordForPosition(pos);
		if (!check(pos)) 
			{
				CCString *type = getType(pos, _background);
				if (type->compare("Water") == 0 || type->compare("Brick") == 0 || type->compare("Wall") == 0)
				{
					if (type->compare("Brick") == 0) 
						{
							_background->removeTileAt( tileCoord );
							stack[i].appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));
						}
					if (type->compare("Wall") == 0) 
						{
							stack[i].appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));
						}
				}
			}	
		if (!win && stack[i].appearance->boundingBox().intersectsRect(enemyCitadel.appearance->boundingBox()))
			{
				enemyCitadel.hp-=10;
				if (enemyCitadel.hp == 0)
				{
					enemyCitadel.appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));
					win = true;
					/*CCSprite *gameover = CCSprite::create("gameover.png");
					this->addChild(gameover, 1000);
					gameover->setPosition(ccp(size.width/2, 0));
					gameover->runAction(CCMoveBy::create(2, ccp(0, 300)));*/
				}
				stack[i].appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));				
			}
	}
}

CCPoint HelloWorld::tileCoordForPosition(CCPoint position)
{
    int x = position.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;

	if (x >= _background->getLayerSize().width) 
		x = _background->getLayerSize().width - 1;

	if (y >= _background->getLayerSize().height) 
		y =_background->getLayerSize().height - 1;

	if (x < 0) x=0;
	if (y < 0) y=0;

    return ccp(x, y);
}

CCString *HelloWorld::getType(CCPoint pos, CCTMXLayer *layer)
{
	CCPoint tileCoord = tileCoordForPosition(pos);
	int tileGid = _background->tileGIDAt(tileCoord);
	if (tileGid == 0) return NULL;
	CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
	CCString *type = new CCString();
	*type = *properties->valueForKey("Type");
	return type;
}

bool HelloWorld::ok_go(CCString *type)
{
	if (type->compare("Water") == 0 || type->compare("Brick") == 0 || type->compare("Wall") == 0) 
		return false;
	return true;
}

bool HelloWorld::check(CCPoint pos)
{
	CCString *type = getType(pos, _background);
	if (type == NULL) return true;
	return ok_go(type);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::removeObject( CCNode* sender ) 
{ 
	for (int i = 0; i < stack.size(); i++)
	{
		if(stack[i].appearance->getTag() == sender->getTag())
		{
			stack.erase(stack.begin()+i);
			break;
		}
	}
	sender->removeFromParentAndCleanup(true);
}