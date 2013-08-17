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
		m_stack[i].setEffect(enemyCitadel);
		m_stack[i].setEffect(ourCitadel);
		m_stack[i].setEffect(Player);
		for (int j=0; j<MAP.m_numDes; j++)
			m_stack[i].setEffect(des[j]);
		for (int j=0; j<MAP.m_numHun; j++)
			m_stack[i].setEffect(hun[j]);
		for (int j=0; j<MAP.m_numGru; j++)
			m_stack[i].setEffect(gru[j]);

		for (int j=i+1; j<m_stack.size(); j++)
			m_stack[i].setEffect(m_stack[j]);
	}
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

void Bullet::setEffect( Tank &tank)
{
	if (tank.m_alive && m_appearance->boundingBox().intersectsRect(tank.m_appearance->boundingBox()))
	{
		tank.m_hp -= m_dame;
		if (tank.m_hp <= 0)
		{
			tank.m_appearance->runAction( CCCallFuncN::create(SCENE, callfuncN_selector(HelloWorld::removeObject)));
			tank.m_alive = false;
		}
		m_appearance->runAction( CCCallFuncN::create(SCENE, callfuncN_selector(HelloWorld::removeObject)));
	}
}

void Bullet::setEffect(Citadel &citadel)
{
	if (citadel.m_alive && m_appearance->boundingBox().intersectsRect(citadel.m_appearance->boundingBox()))
	{
		citadel.m_hp -= m_dame;
		if (citadel.m_hp <= 0)
		{
			citadel.m_appearance->runAction( CCCallFuncN::create(SCENE, callfuncN_selector(HelloWorld::removeObject)));
			citadel.m_alive = false;
		}
		m_appearance->runAction( CCCallFuncN::create(SCENE, callfuncN_selector(HelloWorld::removeObject)));
	}
}

void Bullet::setEffect(Bullet &bullet1)
{
	if (m_appearance->boundingBox().intersectsRect(bullet1.m_appearance->boundingBox()))
	{
		m_appearance->runAction( CCCallFuncN::create(SCENE, callfuncN_selector(HelloWorld::removeObject)));
		bullet1.m_appearance->runAction( CCCallFuncN::create(SCENE, callfuncN_selector(HelloWorld::removeObject)));
	}
}