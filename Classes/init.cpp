bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	MAP.m_Tilemap = "map.tmx";
	MAP.m_numDes = 1;
	MAP.m_numGru = 1;
	MAP.m_numHun = 1;
	MAP.m_posDes[0] = ccp(0, 0);
	MAP.m_posGru[0] = ccp(7, 0);
	MAP.m_posHun[0] = ccp(29, 0);
	MAP.m_tankDame = 2;
	MAP.m_tankHp = 10;
	MAP.m_tankSpeed = 1;


	size = CCDirector::sharedDirector()->getWinSize();
	moveSize = size.width/4;
	m_IsTouchMoved = false;
	IsHolding = false;
	setTouchEnabled(true);
	scheduleUpdate();

	SCENE=this;
    //Add Constant
	constant_init();
	//Add tiled map
    _tileMap = new CCTMXTiledMap();
	_tileMap->initWithTMXFile(MAP.m_Tilemap);
    _background = _tileMap->layerNamed("background");
	_tree = _tileMap->layerNamed("tree");
	_tileMap->removeAllChildren();
	this->addChild(_background, -5);
	this->addChild(_tree, 5);
//	_background->removeTileAt( ccp(3.0, 3.0) );

	//Add tank
	Player.initTank(ccp(7,19),0,"tank1.png",MAP.m_tankDame,MAP.m_tankSpeed,MAP.m_tankHp);

	//Add Citadel
	ourCitadel.m_appearance = CCSprite::create("thanh.png");
	this->addChild(ourCitadel.m_appearance,1);
	ourCitadel.m_appearance->setPosition(ccp(size.width/2,ourCitadel.m_appearance->getContentSize().height/2));
	ourCitadel.m_hp = 10; 

	enemyCitadel.m_appearance = CCSprite::create("thanh.png");
	this->addChild(enemyCitadel.m_appearance,1);
	enemyCitadel.m_appearance->setPosition(ccp(size.width/2,size.height - enemyCitadel.m_appearance->getContentSize().height/2));
	enemyCitadel.m_hp = 10; 
	

	//Add virtual control
	smallcircle = CCSprite::create("nho.png");
	this->addChild(smallcircle,15);
    smallcircle->setVisible(false);

	bigcircle = CCSprite::create("to.png");
	this->addChild(bigcircle,10);
	bigcircle->setVisible(false);	
    //add AI 
    //Destroyer
	for (int i=0; i<MAP.m_numDes; i++)
	{
		des[i].AI_Des_Init(MAP.m_posDes[i],0,"tank2.png",1,1,3);
	}
	//Hunter
	for (int i=0; i<MAP.m_numHun; i++)
		hun[i].AI_Hun_Init(MAP.m_posHun[i],0,"tank2.png",1,1,3);
	//Gruader
	for (int i=0; i<MAP.m_numGru; i++)
		gru[i].AI_Gru_Init(MAP.m_posGru[i],0,"tank2.png",1,1,3);

	//prepare item
	item[0].m_appearance = CCSprite::create("increaseHp.png");
	this->addChild(item[0].m_appearance, 5);
	item[0].m_appearance->setVisible(false);
	item[0].initItem(0, 0, 20, 0);

	item[1].m_appearance = CCSprite::create("speedUpItem.png");
	this->addChild(item[1].m_appearance, 5);
	item[1].m_appearance->setVisible(false);
	item[1].initItem(0, 1, 0, 0);

	item[2].m_appearance = CCSprite::create("increaseDame.png");
	this->addChild(item[2].m_appearance, 5);
	item[2].m_appearance->setVisible(false);
	item[2].initItem(10, 0, 0, 0);

	return true;
}

void Destroyer::AI_Des_Init(CCPoint pos,float goc,string photo,float dame,float speed,float hp)
{
    initTank(pos,0,photo.c_str(),dame,speed,hp);
    Way="";
	Waycount=-1;
	findWay(pos.x,pos.y);
	Keep_Going=0;
}

void Destroyer::AI_Des_Revive(CCPoint pos,float goc,string photo,float dame,float speed,float hp)
{
 	if (m_alive) return;
	if (m_timeRevive == 0)
		AI_Des_Init(pos, goc, photo, dame, speed, hp);
	else m_timeRevive -= 1;	
}

void Hunter::AI_Hun_Init(CCPoint pos,float goc,string photo,float dame,float speed,float hp)
{
    initTank(pos,0,photo.c_str(),dame,speed,hp);
    m_timedelay=0;
	Keep_Going=0;
	m_timeFire=200;
}

void Hunter::AI_Hun_Revive(CCPoint pos,float goc,string photo,float dame,float speed,float hp)
{
	if (m_alive) return;
	if (m_timeRevive == 0)
		AI_Hun_Init(pos, goc, photo, dame, speed, hp);
	else m_timeRevive -= 1;	
}

void Gruarder::AI_Gru_Init(CCPoint pos,float goc,string photo,float dame,float speed,float hp)
{
	initTank(pos,0,photo.c_str(),dame,speed,hp);
	Keep_Going=0;
	m_timeFire=1000;m_timedelay=0;
	Way="";Waycount=-1;
	loss=true;
}

void Gruarder::AI_Gru_Revive(CCPoint pos,float goc,string photo,float dame,float speed,float hp)
{
	if (m_alive) return;
	if (m_timeRevive == 0)
		AI_Gru_Init(pos, goc, photo, dame, speed, hp);
	else m_timeRevive -= 1;	
}

void Tank::initTank(CCPoint pos,float goc,const char* photo,float dame,float speed,float hp)
{
    m_appearance=CCSprite::create(photo);
	m_appearance->setPosition(getCentrePos(pos.x,pos.y));
	m_appearance->setRotation(goc);
	m_time = 0;
	m_dame = dame;
	m_speed = speed;
	m_hp = hp;
	m_timeRevive = 600;
	m_direction = ccp(0, 1);
	SCENE->addChild(this->m_appearance,1);
	OK_Fire=false;
	m_alive=true;
	start=pos;
}

void Tank::Revive(CCPoint pos,float goc,const char* photo,float dame,float speed,float hp)
{
	if (m_alive) return;
	if (m_timeRevive == 0)
		initTank(pos, goc, photo, dame, speed, hp);
	else m_timeRevive -= 1;	
}