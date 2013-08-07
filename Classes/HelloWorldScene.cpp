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
	m_IsTouchMoved = false;
	IsHolding = false;
	win = false;
	lose = false;
	setTouchEnabled(true);
	scheduleUpdate();


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
	Player1.m_appearance = CCSprite::create("tank1.png");
	this->addChild(Player1.m_appearance,1);
	Player1.m_appearance->setPosition(ccp(size.width/4,Player1.m_appearance->getContentSize().height/2));
	Player1.initTank();

	//Add Citadel
	ourCitadel.m_appearance = CCSprite::create("thanh.png");
	this->addChild(ourCitadel.m_appearance,1);
	ourCitadel.m_appearance->setPosition(ccp(size.width/2,ourCitadel.m_appearance->getContentSize().height/2));
	ourCitadel.m_hp = 50; 

	enemyCitadel.m_appearance = CCSprite::create("thanh.png");
	this->addChild(enemyCitadel.m_appearance,1);
	enemyCitadel.m_appearance->setPosition(ccp(size.width/2,size.height - enemyCitadel.m_appearance->getContentSize().height/2));
	enemyCitadel.m_hp = 50; 
	

	//Add virtual control
	smallcircle = CCSprite::create("nho.png");
	this->addChild(smallcircle,15);
    smallcircle->setVisible(false);

	bigcircle = CCSprite::create("to.png");
	this->addChild(bigcircle,10);
	bigcircle->setVisible(false);	

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
		CCPoint bigPos = bigcircle->getPosition();
		CCPoint smallPos = smallcircle->getPosition();
		m_DirectionVector = ccpSub(smallPos, bigPos);	
		m_DirectionVector = normalize(m_DirectionVector, bigPos, smallPos);
		Player1.m_direction = m_DirectionVector;
		move(Player1);
	}
	
	fire(Player1, IsHolding);
	checkBullet();
}

void HelloWorld::checkBullet()
{
	for(int i=0;i<m_stack.size();i++)
	{	
		CCPoint pos = m_stack[i].m_appearance->getPosition();
		CCPoint tileCoord = tileCoordForPosition(pos);
		if (!check(pos)) 
			{
				CCString *type = getType(pos);
				if (type->compare("Water") == 0 || type->compare("Brick") == 0 || type->compare("Wall") == 0)
				{
					if (type->compare("Brick") == 0) 
						{
							_background->removeTileAt( tileCoord );
							m_stack[i].m_appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));
						}

					if (type->compare("Wall") == 0) 
							m_stack[i].m_appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));
				}
			}	

		if (!win && !lose && m_stack[i].m_appearance->boundingBox().intersectsRect(enemyCitadel.m_appearance->boundingBox()))
			{
				enemyCitadel.m_hp -= m_stack[i].m_dame;
				if (enemyCitadel.m_hp <= 0)
				{
					enemyCitadel.m_appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));
					win = true;
				}
				m_stack[i].m_appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));				
			}

		if (!win && !lose && m_stack[i].m_appearance->boundingBox().intersectsRect(ourCitadel.m_appearance->boundingBox()))
			{
				ourCitadel.m_hp -= m_stack[i].m_dame;
				if (ourCitadel.m_hp <= 0)
				{
					ourCitadel.m_appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));
					lose = true;
				}
				m_stack[i].m_appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));				
			}
	}
}

void HelloWorld::fire(Tank &player, bool ok)
{
	if ((ok) && (player.m_time == 0)) 
	{
		Bullet dame;
		dame.m_appearance = CCSprite::create("dame.png");
		dame.m_dame = player.m_dame;
		dame.m_speed = player.m_speed;
		this->addChild(dame.m_appearance,4);
		CCPoint pos = player.m_appearance->getPosition();
		dame.m_appearance->setPosition(pos);
		dame.m_appearance->setTag(bulletcount++);
		m_stack.push_back(dame);

		if ( toAngle(player.m_direction) == 0) dame.m_appearance->runAction(CCSequence::create(CCMoveBy::create(2, ccp(0, 300)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)), NULL));
		if ( toAngle(player.m_direction) == 90) dame.m_appearance->runAction(CCSequence::create(CCMoveBy::create(2, ccp(300, 0)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)), NULL));
		if ( toAngle(player.m_direction) == 180) dame.m_appearance->runAction(CCSequence::create(CCMoveBy::create(2, ccp(0, -300)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)), NULL));
		if ( toAngle(player.m_direction) == 270) dame.m_appearance->runAction(CCSequence::create(CCMoveBy::create(2, ccp(-300, 0)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)), NULL));		
		player.m_time = 50;	
	}
	if (player.m_time) player.m_time -= 1;	
	
}

void HelloWorld::move(Tank player)
{
	player.m_appearance->setRotation(toAngle(player.m_direction));
	
	CCPoint pos = player.m_appearance->getPosition();
	
	CCPoint nextPos;
	nextPos.x = pos.x + player.m_direction.x*player.m_speed;
	nextPos.y = pos.y + player.m_direction.y*player.m_speed;
	float tankSize = player.m_appearance->getContentSize().width;

	player.m_checkPoint1 = ccpAdd(nextPos, ccp(-tankSize/2, tankSize/2));
	player.m_checkPoint2 = ccpAdd(nextPos, ccp(tankSize/2, tankSize/2));
	player.m_checkPoint3 = ccpAdd(nextPos, ccp(tankSize/2, -tankSize/2));
	player.m_checkPoint4 = ccpAdd(nextPos, ccp(-tankSize/2, -tankSize/2));

	if (check(player.m_checkPoint1) && check(player.m_checkPoint2)  && check(player.m_checkPoint3) && check(player.m_checkPoint4))
	{
		player.m_appearance->setPosition(nextPos);
	}
}

void Tank::initTank()
{
	m_time = 0;
	m_dame = 10;
	m_speed = 1;
	m_hp = 40;
	m_direction = ccp(0, 1);
}

float HelloWorld::toAngle(CCPoint direction)
{
	if (direction.x == 1 && direction.y == 0) return(90);
	if (direction.x == 0 && direction.y == 1) return(0);
	if (direction.x == -1 && direction.y == 0) return(270);
	if (direction.x == 0 && direction.y == -1) return(180);
}

CCPoint HelloWorld::tileCoordForPosition(CCPoint pos)
{
    int x = pos.x / _tileMap->getTileSize().width;
    int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y) / _tileMap->getTileSize().height;

	if (x >= _background->getLayerSize().width) 
		x = _background->getLayerSize().width - 1;

	if (y >= _background->getLayerSize().height) 
		y = _background->getLayerSize().height - 1;

	if (x < 0) x=0;
	if (y < 0) y=0;

    return ccp(x, y);
}

CCString *HelloWorld::getType(CCPoint pos)
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
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if (pos.x < 0 || pos.x >= size.width || pos.y < 0 || pos.y >= size.height)
		return false;
	CCString *type = getType(pos);
	if (type == NULL) return true;
	return ok_go(type);
}

void HelloWorld::removeObject( CCNode* sender ) 
{ 
	for (int i = 0; i < m_stack.size(); i++)
	{
		if(m_stack[i].m_appearance->getTag() == sender->getTag())
		{
			m_stack.erase(m_stack.begin()+i);
			break;
		}
	}
	sender->removeFromParentAndCleanup(true);
}


CCPoint HelloWorld::normalize(CCPoint m_DirectionVector, CCPoint bigPos, CCPoint smallPos)
{
	// phai tren
	if((smallPos.x >= bigPos.x)&&(smallPos.y > bigPos.y))
		if (abs(m_DirectionVector.x) >= abs(m_DirectionVector.y)) 
			return(ccp(1, 0));
		else 
			return(ccp(0, 1));

	//trai tren
	if((smallPos.x < bigPos.x)&&(smallPos.y >= bigPos.y))
		if (abs(m_DirectionVector.x) >= abs(m_DirectionVector.y)) 
			return(ccp(-1, 0));
		else 
			return(ccp(0, 1));

	//trai duoi
	if((smallPos.x <= bigPos.x)&&(smallPos.y < bigPos.y))
		if (abs(m_DirectionVector.x) >= abs(m_DirectionVector.y)) 
			return(ccp(-1, 0));
		else 
			return(ccp(0, -1));

	//phai duoi
	if((smallPos.x > bigPos.x)&&(smallPos.y <= bigPos.y))
		if (abs(m_DirectionVector.x) >= abs(m_DirectionVector.y)) 
			return(ccp(1, 0));
		else 
			return(ccp(0, -1));
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}