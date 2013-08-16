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
