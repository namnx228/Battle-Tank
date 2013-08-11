#include "HelloWorldScene.h"
 
using namespace cocos2d;

int bulletcount = 0;
float tankSize;
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
	tileSize = _tileMap->getTileSize().width;

	//Add tank
	Player1.m_appearance = CCSprite::create("tank1.png");
	this->addChild(Player1.m_appearance,1);
	Player1.m_appearance->setPosition(ccp(size.width/4,Player1.m_appearance->getContentSize().height/2));
	Player1.initTank();
	tankSize = Player1.m_appearance->getContentSize().width;

	Player2.m_appearance = CCSprite::create("tank1.png");
	this->addChild(Player2.m_appearance,1);
	Player2.m_appearance->setPosition(ccp(size.width/4,size.height - Player2.m_appearance->getContentSize().height/2));
	Player2.m_appearance->setRotation(180);
	Player2.initTank();
	Player2.m_timedelay = 0;

	//Add Citadel
	ourCitadel.m_appearance = CCSprite::create("thanh.png");
	this->addChild(ourCitadel.m_appearance,1);
	ourCitadel.m_appearance->setPosition(ccp(size.width/2,ourCitadel.m_appearance->getContentSize().height/2));
	ourCitadel.m_hp = 50; 
	ourCitadel.m_ok = true;

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

	//Add item
	speedUp.m_appearance = CCSprite::create("speedUpItem.png");
	this->addChild(speedUp.m_appearance, 5);
	speedUp.m_appearance->setPosition(ccp(size.width/4, size.height*3/4));
	speedUp.m_ok = true;
	speedUp.initItem(0, 1, 0, 1200, 0);
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
	if (Player1.m_ok)
	{
		if ( m_IsTouchMoved)
		{
			CCPoint bigPos = bigcircle->getPosition();
			CCPoint smallPos = smallcircle->getPosition();
			m_DirectionVector = ccpSub(smallPos, bigPos);	
			m_DirectionVector = normalize(m_DirectionVector, bigPos, smallPos);
			Player1.m_direction = m_DirectionVector;
			move(Player1);
		}
		fire(Player1, IsHolding);
	}

	if (Player2.m_ok)
	{
		if (Player2.m_timedelay == 0 && Player1.m_ok)
		{
			bfs(Player2, Player1.m_appearance->getPosition());
			Player2.m_timedelay = 300;
		}
		if (Player2.m_timedelay > 0) Player2.m_timedelay -= 1;

		bool ok_fire = false;
		CCPoint oldDirection = Player2.m_direction;
		ok_fire = ok_fire || (ourCitadel.m_ok && checkFire(Player2, ourCitadel.m_appearance));
		ok_fire = ok_fire || (Player1.m_ok && checkFire(Player2, Player1.m_appearance));
		fire(Player2, ok_fire);
		Player2.m_direction = oldDirection;
		if (!ok_fire && Player2.count > 1)
		{
			setDirection(Player2);
			move(Player2);
		}
	}
	checkBullet();
}

void HelloWorld::checkBullet()
{
	for(int i=0; i<m_stack.size(); i++)
	{	
		CCPoint pos = m_stack[i].m_appearance->getPosition();
		CCPoint tileCoord = tileCoordForPosition(pos);
		if (!check(pos)) 
			{
				CCString *type = getType(pos);
				if (type != NULL && (type->compare("Brick") == 0 || type->compare("Wall") == 0))
				{
					if (type->compare("Brick") == 0) 
							_background->removeTileAt( tileCoord );
					m_stack[i].m_appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));
				}
			}
		setEffect(m_stack[i], enemyCitadel);
		setEffect(m_stack[i], ourCitadel);
		setEffect(m_stack[i], Player1);
		setEffect(m_stack[i], Player2);
	}
}

void HelloWorld::fire(Tank &player, bool ok)
{
	if ((ok) && (player.m_time == 0)) 
	{
		player.m_appearance->setRotation(toAngle(player.m_direction));
		Bullet dame;
		dame.m_appearance = CCSprite::create("dame.png");
		dame.m_dame = player.m_dame;
		dame.m_speed = player.m_speed;
		this->addChild(dame.m_appearance,4);
		CCPoint pos = player.m_appearance->getPosition();
		pos.x = pos.x + player.m_direction.x*(tankSize/2+5);
		pos.y = pos.y + player.m_direction.y*(tankSize/2+5);

		dame.m_appearance->setPosition(pos);
		dame.m_appearance->setTag(bulletcount++);
		m_stack.push_back(dame);
		dame.m_appearance->runAction(CCSequence::create(CCMoveBy::create(2, ccp(300*player.m_direction.x, 300*player.m_direction.y)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)), NULL));
		player.m_time = 50;	
	}
	if (player.m_item.m_time == 0) player.m_item.initItem(0, 0, 0, -1, 0);
	if (player.m_time) player.m_time -= 1;	
	if (player.m_item.m_time) player.m_item.m_time -= 1;
}

void HelloWorld::move(Tank &player)
{
	player.m_appearance->setRotation(toAngle(player.m_direction));
	
	CCPoint pos = player.m_appearance->getPosition();
	
	CCPoint nextPos;
	nextPos.x = pos.x + player.m_direction.x*(player.m_speed + player.m_item.m_speed);
	nextPos.y = pos.y + player.m_direction.y*(player.m_speed + player.m_item.m_speed);

	if (speedUp.m_ok && player.m_appearance->boundingBox().intersectsRect(speedUp.m_appearance->boundingBox()))
	{
		player.m_item = speedUp;
		speedUp.m_appearance->setVisible(false);
		speedUp.m_ok = false;
	}

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
	m_item.m_speed = 0;
}

void HelloWorld::setDirection(AI &ai)
{
	int tmp = 0;
	ai.m_checkPoint1 = tileCoordForPosition(ccpAdd(ai.m_appearance->getPosition(), ccp(-tankSize/2, tankSize/2)));
	ai.m_checkPoint2 = tileCoordForPosition(ccpAdd(ai.m_appearance->getPosition(), ccp(tankSize/2, tankSize/2)));
	ai.m_checkPoint3 = tileCoordForPosition(ccpAdd(ai.m_appearance->getPosition(), ccp(tankSize/2, -tankSize/2)));
	ai.m_checkPoint4 = tileCoordForPosition(ccpAdd(ai.m_appearance->getPosition(), ccp(-tankSize/2, -tankSize/2)));

	if (ai.m_checkPoint1.x != ai.go[ai.count].x || ai.m_checkPoint1.y != ai.go[ai.count].y) tmp++;
	if (ai.m_checkPoint2.x != ai.go[ai.count].x || ai.m_checkPoint2.y != ai.go[ai.count].y) tmp++;
	if (ai.m_checkPoint3.x != ai.go[ai.count].x || ai.m_checkPoint3.y != ai.go[ai.count].y) tmp++;
	if (ai.m_checkPoint4.x != ai.go[ai.count].x || ai.m_checkPoint4.y != ai.go[ai.count].y) tmp++;

	if (tmp == 4) ai.count--;
	if (ai.count > 1)
		ai.m_direction = ccp(ai.go[ai.count - 1].x - ai.go[ai.count].x, ai.go[ai.count - 1].y - ai.go[ai.count].y);

	if (ai.m_direction.x == 0)
		ai.m_direction.y *= -1;
}

bool HelloWorld::checkFire(AI &ai, CCSprite *target)
{
	float subX = ai.m_appearance->getPosition().x - target->getPosition().x;
	float subY = ai.m_appearance->getPosition().y - target->getPosition().y;

	if (abs(subX) < 10 && abs(subY) < 300)
	{
		if (subY > 0)
			ai.m_direction = ccp(0, -1);
		else
			ai.m_direction = ccp(0, 1);
		if (!canFire(ai.m_appearance->getPosition(), ai.m_direction, (int)abs(subY)/tileSize)) return false;
		return true;
	}

	if (abs(subY) < 10 && abs(subX) < 300)
	{
		if (subX > 0)
			ai.m_direction = ccp(-1, 0);
		else
			ai.m_direction = ccp(1, 0);
		if (!canFire(ai.m_appearance->getPosition(), ai.m_direction, (int)abs(subX)/tileSize)) return false;
		return true;
	}
	return false;
}

bool HelloWorld::canFire(CCPoint pos, CCPoint direction, int tiles)
{
	int tmp = 0;
	for (int i=0; i<tiles; i++)
	{
		pos.x += direction.x * tileSize;
		pos.y += direction.y * tileSize;
		CCString *string = getType(pos);
		if (string != NULL && string->compare("Wall") == 0) return false;
		if (string != NULL && string->compare("Brick") == 0) tmp++;
	}
	if (tmp < 3)
		return true;
	return false;
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
	if (pos.x < 0 || pos.x >= size.width || pos.y < 0 || pos.y >= size.height)
		return NULL;
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

void HelloWorld::setEffect(Bullet &bullet, Tank &tank)
{
	if (tank.m_ok && bullet.m_appearance->boundingBox().intersectsRect(tank.m_appearance->boundingBox()))
	{
		tank.m_hp -= bullet.m_dame;
		if (tank.m_hp <= 0)
		{
			tank.m_appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));
			tank.m_ok = false;
		}
		bullet.m_appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));
	}
}

void HelloWorld::setEffect(Bullet &bullet, Citadel &citadel)
{
	if (citadel.m_ok && bullet.m_appearance->boundingBox().intersectsRect(citadel.m_appearance->boundingBox()))
	{
		citadel.m_hp -= bullet.m_dame;
		if (citadel.m_hp <= 0)
		{
			citadel.m_appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));
			citadel.m_ok = false;
		}
		bullet.m_appearance->runAction( CCCallFuncN::create(this, callfuncN_selector(HelloWorld::removeObject)));
	}
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

void HelloWorld::bfs(AI &ai, CCPoint target)
{
	CCPoint dir[5];
	dir[1] = ccp(1, 0); dir[2] = ccp(0, 1); dir[3] = ccp(0, -1); dir[4] = ccp(-1, 0);

	CCPoint pos = ai.m_appearance->getPosition();
	if (ai.m_direction.x > 0|| ai.m_direction.y > 0) 
		pos = ccpAdd(pos, ccp(-tankSize/2, -tankSize/2));
	else 
		pos = ccpAdd(pos, ccp(tankSize/2, tankSize/2));

	memset(free, 0, sizeof(free));
	CCPoint s = tileCoordForPosition(pos);
	CCPoint f = tileCoordForPosition(target);

	free[(int)s.x][(int)s.y] = 1;
	q[1] = s;
	int last = 1;
	int first = 1;
	ai.count = 0;

	while (first <= last)
	{
		CCPoint u = q[first++];
		for (int i=1; i<5; i++)
		{
			CCPoint v;
			v.x = u.x + dir[i].x;
			v.y = u.y + dir[i].y;
			if (check(ccp(v.x*_tileMap->getTileSize().width, size.height-1 - v.y*32)) &&v.x >= 0 && v.x < 32 && v.y >=0 && v.y < 20 && free[(int)v.x][(int)v.y] == 0)
			{
				q[++last] = v;
				free[(int)v.x][(int)v.y] = 1;
				trace[(int)v.x][(int)v.y] = u;
			}
		}
	}
	
	while (f.x != s.x || f.y != s.y)
	{
		ai.go[++ai.count] = f;
		f = trace[(int)f.x][(int)f.y];
	}
	ai.go [++ai.count] = s;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}