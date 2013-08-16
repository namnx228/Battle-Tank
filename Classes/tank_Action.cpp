#include"Coor_Problem.cpp"
#include"AI.cpp"
#include"bullet.cpp"
void Tank::fire(bool ok)
{
	if ((ok) && (m_time == 0)) 
	{
		m_appearance->setRotation(toAngle(m_direction));
		Bullet dame;
		dame.m_appearance = CCSprite::create("dame.png");
		dame.m_dame = m_dame + m_item.m_dame;
		SCENE->addChild(dame.m_appearance,4);
		CCPoint pos = m_appearance->getPosition();
		pos.x = pos.x + m_direction.x*(tankSize/2+5);
		pos.y = pos.y + m_direction.y*(tankSize/2+5);

		dame.m_appearance->setPosition(pos);
		dame.m_appearance->setTag(bulletcount++);
		m_stack.push_back(dame);
		dame.m_appearance->runAction(CCSequence::create(CCMoveBy::create(4, ccp(500*m_direction.x, 500*m_direction.y)), CCCallFuncN::create(SCENE, callfuncN_selector(HelloWorld::removeObject)), NULL));
		m_time = 50;	
	}
	if (m_item.m_time == 0) m_item.initItem(0, 0, 0, 0);
	if (m_time) m_time -= 1;	
	if (m_item.m_time) m_item.m_time -= 1;
	OK_Fire=false;
}

void Tank::move()
{
	m_appearance->setRotation(toAngle(m_direction));
	
	CCPoint pos = m_appearance->getPosition();
	
	CCPoint nextPos;
	nextPos.x = pos.x + m_direction.x*(m_speed + m_item.m_speed);
	nextPos.y = pos.y + m_direction.y*(m_speed + m_item.m_speed);
	
	for (int i=0; i<numberOfItem; i++)
		if (SCENE->item[i].m_ok && m_appearance->boundingBox().intersectsRect(SCENE->item[i].m_appearance->boundingBox()))
	{
		m_item = SCENE->item[i];
		SCENE->item[i].m_appearance->setVisible(false);
		SCENE->item[i].m_ok = false;
		if (i == 0) m_hp += SCENE->item[i].m_hp;
	}

	m_checkPoint1 = ccpAdd(nextPos, ccp(-tankSize/2, tankSize/2));
	m_checkPoint2 = ccpAdd(nextPos, ccp(tankSize/2, tankSize/2));
	m_checkPoint3 = ccpAdd(nextPos, ccp(tankSize/2, -tankSize/2));
	m_checkPoint4 = ccpAdd(nextPos, ccp(-tankSize/2, -tankSize/2));
	if (SCENE->ourCitadel.m_alive && m_appearance->boundingBox().intersectsRect(SCENE->ourCitadel.m_appearance->boundingBox())) return;
	if (SCENE->enemyCitadel.m_alive && m_appearance->boundingBox().intersectsRect(SCENE->enemyCitadel.m_appearance->boundingBox())) return;
	if (check(m_checkPoint1) && check(m_checkPoint2)  && check(m_checkPoint3) && check(m_checkPoint4))
	{
		//CCLog("%f %f",nextPos.x,nextPos.y);
		m_appearance->setPosition(nextPos);
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
