void AI::setDirection()
{
	int tmp = 0;
	m_checkPoint1 = tileCoordForPosition(ccpAdd(m_appearance->getPosition(), ccp(-tankSize/2, tankSize/2)));
	m_checkPoint2 = tileCoordForPosition(ccpAdd(m_appearance->getPosition(), ccp(tankSize/2, tankSize/2)));
	m_checkPoint3 = tileCoordForPosition(ccpAdd(m_appearance->getPosition(), ccp(tankSize/2, -tankSize/2)));
	m_checkPoint4 = tileCoordForPosition(ccpAdd(m_appearance->getPosition(), ccp(-tankSize/2, -tankSize/2)));

	if (m_checkPoint1.x != go[count].x || m_checkPoint1.y != go[count].y) tmp++;
	if (m_checkPoint2.x != go[count].x || m_checkPoint2.y != go[count].y) tmp++;
	if (m_checkPoint3.x != go[count].x || m_checkPoint3.y != go[count].y) tmp++;
	if (m_checkPoint4.x != go[count].x || m_checkPoint4.y != go[count].y) tmp++;

	if (tmp == 4) count--;
	if (count > 1)
		m_direction = ccp(go[count-1].x - go[count].x, go[count].y - go[count-1].y);

}

bool checkBrick(int x,int y)
{
	return (( 13<=x )&&( x<=16 )&&( y<=2 ));
}

bool AI::Saw(const Tank &Player,int & value)
{
    if (!Player.m_alive) return false;
	CCPoint pos_player=Player.m_appearance->getPosition() , pos_AI=m_appearance->getPosition();
	CCPoint pos_Tile_player=tileCoordForPosition(pos_player), pos_Tile_AI=tileCoordForPosition(pos_AI);
    if(compare(pos_player.x,pos_AI.x)&&compareTile(pos_Tile_player.x,pos_Tile_AI.x))
    {
       value=(pos_AI.y>=pos_player.y)? 0:2;
	  // return nothing(getCentrePos(pos_AI.x,pos_AI.y),getCentrePos(pos_player.x,pos_player.y),3);
	   return nothing(pos_AI,pos_player,3);
    }
    if(compare(pos_player.y,pos_AI.y)&&compareTile(pos_Tile_player.y,pos_Tile_AI.y))
    {  
	   value=(pos_AI.x>=pos_player.x)? 1:3; 
	   //return nothing(getCentrePos(pos_AI.x,pos_AI.y),getCentrePos(pos_player.x,pos_player.y),3);
	    return nothing(pos_AI,pos_player,3);
	}
	return false;
}

bool AI::Know_Where_Citadel(const Citadel& ourCitadel,int& value)
{
     CCPoint pos_Citadel=ourCitadel.m_appearance->getPosition() , pos_AI=m_appearance->getPosition();
     CCSize SIZE=ourCitadel.m_appearance->getContentSize();
     float y1=pos_Citadel.y-SIZE.width/2,y2=pos_Citadel.y+SIZE.width/2 , x1=pos_Citadel.x-SIZE.height/2, x2=pos_Citadel.x+SIZE.height/2;
     if((x1<=pos_AI.x)&&(pos_AI.x<=x2))
     {
            value=0;
            return nothing(pos_AI,ccp(pos_AI.x,y2),100);
     }
     if((pos_AI.y<=y2)&&(pos_AI.y>=y1))
     { 
			
			if (pos_AI.x<x1) 
			{
					value=3; return nothing(pos_AI,ccp(x1,pos_AI.y),100);
			}
			else {
					value=1; return nothing(pos_AI,ccp(x2,pos_AI.y),100);
				 }
     }
	 return false;
}

bool AI::nothing( CCPoint p1, CCPoint p2,int n_brick)
{
        p1=CCPoint(tileCoordForPosition(p1));
        p2=CCPoint(tileCoordForPosition(p2));
		if(p1.equals(p2)) return false;
		CCPoint sub=ccpSub(p2,p1);
		if (abs(sub.x+sub.y)>12) return false;
		//CCLog("Begin");
        CCPoint DIR=ccpNormalize(sub);
		//CCLog("end");
        for(int x=p1.x+DIR.x,y=p1.y+DIR.y ;(x!=p2.x)||(y!=p2.y) ; x+=DIR.x,y+=DIR.y)
        {
			//CCLog("end");
            CCPoint pos=getCentrePos(x,y);
            CCString *type=getType(pos);
			if(type==NULL) continue;
			if(!(type->compare("Wall"))) return false;
			if(!(type->compare("Brick")))
            {
                n_brick--;
                if(n_brick<0) return false;
            }
        }
		//CCLog("end");
	return true;
}

void AI::bfs(CCPoint target)
{
	CCPoint pos = m_appearance->getPosition();
	if (m_direction.x > 0|| m_direction.y > 0) 
		pos = ccpAdd(pos, ccp(-tankSize/2, -tankSize/2));
	else 
		pos = ccpAdd(pos, ccp(tankSize/2, tankSize/2));

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	memset(d, 0, sizeof(d));
	CCPoint s = tileCoordForPosition(pos);
	CCPoint f = tileCoordForPosition(target);
	d[(int)s.x][(int)s.y] = 1;
	q[1] = s;
	int last = 1;
	int first = 1;
	count = 0;

	while (first <= last)
	{
		CCPoint u = q[first];
		first++;
		for (int i=0; i<4; i++)
		{
			CCPoint v;
			v.x = u.x + dir[i].x;
			v.y = u.y + dir[i].y;
			
			if ( check(ccp(v.x*SCENE->_tileMap->getTileSize().width, size.height-1 - v.y*32)) &&v.x >= 0 && v.x < 32 && v.y >=0 && v.y < 20 && d[(int)v.x][(int)v.y] == 0)
			{
				last++;
				q[last] = v;
				d[(int)v.x][(int)v.y] = 1;
				trace[(int)v.x][(int)v.y] = u;
			}
		}
	}
	while (f.x != s.x || f.y != s.y)
	{
		go[++count] = f;
		f = trace[(int)f.x][(int)f.y];
	}
	go [++count] = s;
}


void AI::findWay(int x,int y)
{
    memset(d,0,sizeof(d));
	dfs(x,y);
}

void AI::dfs(int x,int y)
{
	d[x][y]=1;
	CCString *type;
	CCPoint pos=getCentrePos(x,y);
	int vitri[4];
	vitri[0]=0; vitri[3]=2;
	if (pos.x>SCENE->ourCitadel.m_appearance->getPositionX())
	{ vitri[1]=1; vitri[2]=3;	}
	else { vitri[1]=3;vitri[2]=1;}
	For(i,0,3)
	{
		int newx=x+DIR[vitri[i]].x, newy=y+DIR[vitri[i]].y;
		if(OK_GO(newx,newy))
		{
			
			type=getType(getCentrePos(newx,newy));
			if((type!=NULL)&&(type->compare("Brick")==0)) 
            {
                if((bricklimit==3)||(checkBrick(newx,newy))) continue;
                Way=Way+'F';
                bricklimit++;
            }else bricklimit=0;
			Way=Way+char(vitri[i]+48);
			dfs(newx,newy);
			Way=Way+char((vitri[i]+2)%4+48);
		}
	}
}

void Gruarder::findWay_Gru(int x,int y)
{
    memset(d,0,sizeof(d));
	dfs_Gru(x,y);

}

void Gruarder::dfs_Gru(int x,int y)
{
	d[x][y]=1;
	CCString *type;
	CCSize SIZE=SCENE->_tileMap->getMapSize();
	int vitri[4];
	vitri[0]=0; vitri[3]=2;
	CCPoint pos=getCentrePos(x,y);
	if (pos.x>SCENE->ourCitadel.m_appearance->getPositionX())
	{ vitri[1]=1; vitri[2]=3;	}
	else { vitri[1]=3;vitri[2]=1;}
	For(i,0,3)
	{
		int newx=x+DIR[vitri[i]].x, newy=y+DIR[vitri[i]].y;
		if(OK_GO(newx,newy)&&(newy<=SIZE.height/3))
		{
			
			type=getType(getCentrePos(newx,newy));
			if((type!=NULL)&&(type->compare("Brick")==0)) 
            {
                if((bricklimit==3)||(checkBrick(newx,newy))) continue;
                Way=Way+'F';
                bricklimit++;
            }else bricklimit=0;
			Way=Way+char(vitri[i]+48);
			dfs_Gru(newx,newy);
			Way=Way+char((vitri[i]+2)%4+48);
		}
	}
}

bool Gruarder::scanEnemy(const Tank& Player)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	return (Player.m_appearance->getPositionY() >= size.height*2/3);
}

void Destroyer::AI_Des_Action(const Tank& Player,const Citadel& ourCitadel)
{
	CCPoint Pos=m_appearance->getPosition();
	CCPoint pos2=tileCoordForPosition(Pos);
	if(checkBrick(int(pos2.x),int(pos2.y))) CCLog("Loi o day:%d",Waycount);
	int pos;
    // tan cong
	if((Know_Where_Citadel(ourCitadel,pos))||(Saw(Player,pos)))
	{
       m_appearance->setRotation(toAngle(dir[pos]));
	   m_direction=dir[pos];
       fire(true);
	   return;
    }
    //di truyen
	if (!m_time)
	if(Keep_Going>0) 
		{
				
			Keep_Going-=m_speed+m_item.m_speed;
			pos=int(Way[Waycount])-48;
			m_direction=dir[pos];
			//m_appearance->setRotation(toAngle(dir[pos]));
			//m_appearance->setPosition(ccpAdd(Pos,dir[pos]));		
			move();
	    }
	else 
		{
			Waycount++;
			if(Way[Waycount]=='F') 
				{
					pos = int(Way[Waycount+1])-48;
					m_direction=dir[pos];
					fire(true);
					return;
				}
			pos=int(Way[Waycount])-48;
			//m_appearance->setRotation(toAngle(dir[pos]));
			m_direction=dir[pos];
			//m_appearance->setPosition(ccpAdd(Pos,dir[pos]));
			move();
			Keep_Going=SCENE->_tileMap->getTileSize().width-m_speed-m_item.m_speed;
		}
	fire(OK_Fire);
}

void Hunter::AI_Hun_Action(const Tank& Player,const Citadel& ourCitadel)
{
		int pos;
		if(!m_timeFire && !Know_Where_Citadel(SCENE->enemyCitadel,pos))
		{
			m_timeFire=300;
			fire(true);
			return;
		}
		if(m_timeFire) m_timeFire--;
		if (m_timedelay == 0 && Player.m_alive)
		{
			bfs(Player.m_appearance->getPosition());
			m_timedelay = 300;
		}
		if (m_timedelay > 0) m_timedelay -= 1;

		OK_Fire =Saw(Player,pos)||Know_Where_Citadel(ourCitadel,pos);
		if (OK_Fire)
			{
				m_direction=dir[pos];
				fire(OK_Fire);
				return;		
			}

		fire(OK_Fire);
		if (count > 1 && !m_time)
		{
			setDirection();
			move();
		}
}
void Gruarder::AI_Gru_Action(const Tank& Player,const Citadel& ourCitadel)
{
	if (Player.m_alive && scanEnemy(Player))
	{
		AI_Hun_Action(Player,ourCitadel);
		m_timeFire=1000;
		loss=true;
	}
	else
	{
		if(Way.length()-1==Waycount) loss=true;
		if (loss)
		{
			loss=!goHome();
			if(!loss) 
			{
				CCPoint pos=tileCoordForPosition(m_appearance->getPosition());
				Way="";
				Waycount=-1;
				findWay_Gru(pos.x,pos.y);
				m_timedelay=0; Keep_Going=0;
			}
		}
		else AI_Des_Action(Player,ourCitadel);
	}
}

bool Gruarder::goHome()
{
	int pos;
	
		if(!m_timeFire && !Know_Where_Citadel(SCENE->enemyCitadel,pos))
		{
			m_timeFire=300;
			fire(true);
			return(false);
		}
		if(m_timeFire) m_timeFire--;
		if (m_timedelay == 0)
		{
			bfs(getCentrePos(start.x,start.y));
			m_timedelay = 10000;
		}
		if (m_timedelay > 0) m_timedelay -= 1;
		OK_Fire =  Know_Where_Citadel(SCENE->ourCitadel,pos);
		if (OK_Fire)
			{
				m_direction=dir[pos];
				fire(OK_Fire);
				return(false);		
			}

		fire(OK_Fire);
		if (count > 1 && !m_time)
		{
			setDirection();
			move();
		}
		return (m_appearance->getPosition().equals(getCentrePos(start.x,start.y)));
}