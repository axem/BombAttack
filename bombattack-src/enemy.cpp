/***************************************************************************
 *   Copyright                                                             *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "enemy.h"
#include <cstdlib>

bool Enemy::preloaded=false;
Enemy::Animation * Enemy::deathup=0;
Enemy::Animation * Enemy::deathdown=0;
Enemy::Animation * Enemy::deathleft=0;
Enemy::Animation * Enemy::deathright=0;

void Enemy::preloadRes()
{
    if(!preloaded)
    {
        int ms=100;
        deathup = new Animation();
        deathdown = new Animation();
        deathleft = new Animation();
        deathright = new Animation();

        deathup->addRes(strs("deathup1.png","deathup2.png",0),ms);
        deathdown->addRes(strs("deathdown1.png","deathdown2.png",0),ms);
        deathleft->addRes(strs("deathleft1.png","deathleft2.png",0),ms);
        deathright->addRes(strs("deathright1.png","deathright2.png",0),ms);

        preloaded=true;
    }
}

Enemy::Enemy()
: nextEnemyMove(NONE), wakeup(false), alive(true)
{
    int ms=100;
    up.addRes(strs("deathup1.png","deathup2.png",0),ms);
    down.addRes(strs("deathdown1.png","deathdown2.png",0),ms);
    left.addRes(strs("deathleft1.png","deathleft2.png",0),ms);
    right.addRes(strs("deathright1.png","deathright2.png",0),ms);
    setAnim(down);
    moveMe(Enemy::DOWN);

}

Enemy::~Enemy()
{
}

void Enemy::onWalkDone(move_type& e)
{
    e=randomMove();
}

Enemy::move_type Enemy::randomMove()
{
    move_type e=UP;
    bool used[4]={ false, false, false, false };
    do
    {
        if(nextEnemyMove == NONE)
        {
            int i=rand() % 4;
            switch(i)
            {
                case 0: e=UP;    break;
                case 1: e=DOWN;  break;
                case 2: e=LEFT;  break;
                case 3: e=RIGHT; break;
                default: break;
            };
        }

        if(nextEnemyMove != NONE) 
        {
            e=nextEnemyMove;
            nextEnemyMove=NONE;
        }

        switch(e)
        {
            case UP:      used[0]=true; break;
            case DOWN:    used[1]=true; break;
            case LEFT:    used[2]=true; break;
            case RIGHT:   used[3]=true; break;
            default: break;
        };
    } while( !goodMove(e) && !( used[0] &&  used[1] && used[2] && used[3] ) );


    if(!goodMove(e)  && used[0] &&  used[1] && used[2] && used[3]) return NONE;
    return e;
}


void Enemy::modifyRect(RectFloat & r)
{
    static PointFloat collisionOffset=normalize(15,5);
    r.first.first+=collisionOffset.first;
    r.first.second+=collisionOffset.second;
    r.second.first-=collisionOffset.first;
    r.second.second-=collisionOffset.second;
}

void Enemy::unloadRes()
{
    if(preloaded)
    {
        delete deathup;
        delete deathdown;
        delete deathleft;
        delete deathright;
        deathup=0;
        deathdown=0;
        deathleft=0;
        deathright=0;
        preloaded=false;
    }
}

void Enemy::setNextEnemyMove(move_type m)
{
    nextEnemyMove=m;
}

Enemy::Animation * Enemy::getAnim()
{
    if(Object::getAnim()==&up)    return deathup;
    if(Object::getAnim()==&down)  return deathdown;
    if(Object::getAnim()==&left)  return deathleft;
    if(Object::getAnim()==&right) return deathright;
    return Object::getAnim();
}



