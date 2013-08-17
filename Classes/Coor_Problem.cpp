float toAngle(CCPoint direction)
{
	if (direction.x == 1 && direction.y == 0) return(90);
	if (direction.x == 0 && direction.y == 1) return(0);
	if (direction.x == -1 && direction.y == 0) return(270);
	if (direction.x == 0 && direction.y == -1) return(180);
}

CCPoint tileCoordForPosition(CCPoint pos)
{
    int x = pos.x / SCENE->_tileMap->getTileSize().width;
    int y = ((SCENE->_tileMap->getMapSize().height * SCENE->_tileMap->getTileSize().height) - pos.y) / SCENE->_tileMap->getTileSize().height;

	if (x >= SCENE->_background->getLayerSize().width) 
		x = SCENE->_background->getLayerSize().width - 1;

	if (y >= SCENE->_background->getLayerSize().height) 
		y = SCENE->_background->getLayerSize().height - 1;

	if (x < 0) x=0;
	if (y < 0) y=0;

    return ccp(x, y);
}

CCString *getType(CCPoint pos)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if (pos.x < 0 || pos.x >= size.width || pos.y < 0 || pos.y >= size.height)
		return NULL;
	CCPoint tileCoord = tileCoordForPosition(pos);
	int tileGid =SCENE->_background->tileGIDAt(tileCoord);
	if (tileGid == 0) return NULL;
	CCDictionary *properties = SCENE->_tileMap->propertiesForGID(tileGid);
	CCString *type = new CCString();
	*type = *properties->valueForKey("Type");
	return type;
}

bool check(CCPoint pos)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	//CCLog("%d %d",pos.x,pos.y);
	if (pos.x < 0 || pos.x > size.width || pos.y < 0 || pos.y > size.height) return false;
	CCString *type = getType(pos);
	if (type == NULL) return true;
	return false;
}

CCPoint getCentrePos(int x,int y)
{
    CCSize SIZE=SCENE->_tileMap->getTileSize();
	CCPoint pos=ccp(x*SIZE.width+0.5*SIZE.width,(SCENE->_tileMap->getMapSize().height-y-1)*SIZE.height+0.5*SIZE.height);
    return pos;
}
bool OK_GO(int x,int y)
{
	if((x<0)||(y<0)) return false;
    if(d[x][y]) return false;
	CCPoint pos=getCentrePos(x,y);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if (pos.x < 0 || pos.x >= size.width || pos.y < 0 || pos.y >= size.height)return false;
	CCString* type = getType(pos);
	if ((type == NULL)||type->compare("Brick")==0) return true;
	return false;
}

bool compare(float x,float y)
{
    if(abs(int(x)-int(y))<=16) return true;
    return false;
}

bool compareTile(float x,float y)
{
	if(abs(int(x)-int(y))==0) return true;
    return false;
}

